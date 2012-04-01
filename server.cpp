#include "server.h"

Server * Server::server = NULL;
Server * Server::getServer(){
  return server;
}

void Server::initServer(QObject *parent, int port){
  Server::server = new Server(parent, port);

  Server::server->init();
}

Server::Server(QObject *parent, int port) : QTcpServer(parent), port(port){
  g = new GameEngine(this);
}

void Server::init(){
  if (!listen(QHostAddress::Any, this->port)) { // Démarrage du serveur sur toutes les IP disponibles

	// Si le serveur n'a pas été démarré correctement
	qDebug() << "Le serveur n'a pas pu etre demarre. Raison :" + errorString();
  } else {
	// Si le serveur a été démarré correctement

	//qDebug() << ;
	Logger::log("Le serveur a ete demarre sur ", Logger::INIT);

	QString adress = "127.0.0.1";
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	// use the first non-localhost IPv4 address
	for (int i = 0; i < ipAddressesList.size(); ++i) {
      if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
		adress = ipAddressesList.at(i).toString();
		qDebug() << adress + ":" + QString::number(serverPort());
      }
	}

	connect(this, SIGNAL(newConnection()), this, SLOT(playerConnected()));

	g->init();
  }
}

void Server::playerConnected()
{
  qDebug() << "Joueur connecte ! (" + QString::number(players.size() + 1) + " joueurs)";

  QTcpSocket *newPlayer = nextPendingConnection();

  Player * p = new Player();
  p->port = newPlayer->peerPort();
  p->socket = newPlayer;
  p->pendingRequest = false;

  players[newPlayer->peerAddress().toString() + ":" + QString::number(newPlayer->peerPort())] = p;

  connect(newPlayer, SIGNAL(readyRead()), this, SLOT(dataReceived()));
  connect(newPlayer, SIGNAL(disconnected()), this, SLOT(playerDisconnected()));
}

void Server::playerDisconnected(){
  qDebug() << "Joueur deconnecte ! (" + QString::number(players.size() - 1) + " joueurs)";

  QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
  if(socket == 0)
	return;

  Player *p = this->players[socket->peerAddress().toString() + ":" + QString::number(socket->peerPort())];
  this->players.remove(socket->peerAddress().toString() + ":" + QString::number(socket->peerPort()));
  delete p;
}

void Server::dataReceived()
{
  QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
  if(socket == 0)
	return;

  Player & p = *(players[socket->peerAddress().toString() + ":" + QString::number(socket->peerPort())]);

  QString buffer;

  if(socket->bytesAvailable()){
	buffer += socket->readAll();
	p.pendingRequest = true;
	p.request += buffer;

	//qDebug() << " : Requette reçue : \n" + p.request;

	if(p.request.indexOf("$$") >= 0){
      p.pendingRequest = false;

      //p.request.remove("$$");

      QStringList list1 = p.request.split("$$");

      for(int i = 0; i < list1.size() -1; i++){
		processRequest(p, list1.at(i));
      }
	}
  }
}

void Server::processRequest(Player &p, QString req){
  if(req.startsWith("init")){
    p.setName(req.split(' ')[1]);

    sendInitToPlayer(p);
  } else if(req.startsWith("update")){
	sendUpdateToPlayers();
  } else {
	QJson::Parser parser;

	bool ok;
	QByteArray json = req.toAscii();

	QVariantMap result = parser.parse(json, &ok).toMap();
	if (!ok) {
      qDebug() << "An error occurred during parsing request : " + p.request;
      return;
	}

	if(!result["modifications"].toList().isEmpty()){

      QVariantMap m;

      foreach (QVariant plugin, result["modifications"].toList()) {

		m = plugin.toMap();

		//MAJ touches joueur
		if(!m["name"].toString().isEmpty()){

          if(m["name"].toString() == "UP"){
            p.setKeyForwards(m["state"].toBool());
          } else if(m["name"].toString() == "DOWN"){
            p.setKeyBackwards(m["state"].toBool());
          } else if(m["name"].toString() == "RIGHT"){
            p.setKeyRight(m["state"].toBool());
          } else if(m["name"].toString() == "LEFT"){
            p.setKeyLeft(m["state"].toBool());
          } else if(m["name"].toString() == "MOUSE"){
			QVariantMap mouse =  m["state"].toMap();

            p.setDirX(mouse["x"].toFloat());
            p.setDirY(mouse["y"].toFloat());
            p.setDirZ(mouse["z"].toFloat());
          } else {
            qDebug() << "Key name unknown for player : " + p.getName();
          }
		}
      }
	}

	if(!result["shoot"].toMap().isEmpty()){
      QVariantMap m = result["shoot"].toMap();

      if(!m["name"].toString().isEmpty()){
		if(m["name"].toString() == "MOUSE"){
          QVariantMap mouse =  m["state"].toMap();

          p.setDirX(mouse["x"].toFloat());
          p.setDirY(mouse["y"].toFloat());
          p.setDirZ(mouse["z"].toFloat());

          p.fire();
		} else {
          qDebug() << "Key name unknown for bullet : " + p.getName();
		}
      }

	}


	this->g->modif = true;
  }

  p.request.remove(req + "$$");
}

QByteArray Server::forgeInit(){

  QVariantMap packet;

  forgeFieldInfo(packet);
  forgePlayersInfo(packet);
  forgeProjectilesInfo(packet);

  QJson::Serializer serializer;
  QByteArray json = serializer.serialize(packet);

  json.append("$$");

  return json;
}

QByteArray Server::forgeUpdate(){

  QVariantMap packet;

  forgePlayersInfo(packet);
  forgeProjectilesInfo(packet);

  QJson::Serializer serializer;
  QByteArray json = serializer.serialize(packet);

  json.append("$$");

  return json;
}

void Server::forgeFieldInfo(QVariantMap & packet){

  QVariantMap fld;

  fld.insert("width", this->g->getField().maxX);
  fld.insert("height", this->g->getField().maxZ);

  QVariantList obs;

  foreach(Obstacle *o, this->g->getField().obstacles){
	QVariantMap ob;

	ob.insert("id", o->id);
    ob.insert("x", (double)o->getPosX());
    ob.insert("y", (double)o->getPosY());
    ob.insert("z", (double)o->getPosZ());
    ob.insert("w", (double)o->getWidth());
    ob.insert("h", (double)o->getHeight());
    ob.insert("l", (double)o->getLength());

	obs << ob;
  }

  fld.insert("obstacles", obs);

  packet.insert("field", fld);
}

void Server::forgePlayersInfo(QVariantMap & packet){

  QVariantList pls;

  foreach(Player *p, this->players){
    //if(pr->getModifiedProperties().size() > 0 ) {

	QVariantMap player;

    player.insert("id", p->id);
    player.insert("name", p->getName());
    player.insert("x", (double)p->getPosX());
    player.insert("y", (double)p->getPosY());
    player.insert("z", (double)p->getPosZ());
    player.insert("vx", (double)p->getDirX());
    player.insert("vy", (double)p->getDirY());
    player.insert("vz", (double)p->getDirZ());
    player.insert("life", p->getLife());
    player.insert("deaths", p->getDeads());
    player.insert("kills", p->getKills());
    player.insert("w", (double)p->getWidth());
    player.insert("h", (double)p->getHeight());
    player.insert("l", (double)p->getLength());
    player.insert("cube", p->getCube());

    /*
    player.insert("id", p->id);
    if(p->getModifiedProperties().contains("name")) player.insert("name", p->getName());
    if(p->getModifiedProperties().contains("posX")) player.insert("x", (double)p->getPosX());
    if(p->getModifiedProperties().contains("posY")) player.insert("y", (double)p->getPosY());
    if(p->getModifiedProperties().contains("posZ")) player.insert("z", (double)p->getPosZ());
    if(p->getModifiedProperties().contains("dirX")) player.insert("vx", (double)p->getDirX());
    if(p->getModifiedProperties().contains("dirY")) player.insert("vy", (double)p->getDirY());
    if(p->getModifiedProperties().contains("dirZ")) player.insert("vz", (double)p->getDirZ());
    if(p->getModifiedProperties().contains("life")) player.insert("life", p->getLife());
    if(p->getModifiedProperties().contains("deaths")) player.insert("deaths", p->getDeads());
    if(p->getModifiedProperties().contains("kills")) player.insert("kills", p->getKills());
    if(p->getModifiedProperties().contains("width")) player.insert("w", (double)p->getWidth());
    if(p->getModifiedProperties().contains("height")) player.insert("h", (double)p->getHeight());
    if(p->getModifiedProperties().contains("length")) player.insert("l", (double)p->getLength());
    if(p->getModifiedProperties().contains("cube")) player.insert("cube", p->getCube());
    */

	pls << player;

    //}
  }

  packet.insert("players", pls);
}

void Server::forgeProjectilesInfo(QVariantMap & packet){
  QVariantList pro;

  foreach(Player *p, this->players){
    foreach(Projectile *pr, p->projectiles){
      //if(pr->getModifiedProperties().size() > 0 ) {

      QVariantMap projectile;

      projectile.insert("id", pr->id);
      projectile.insert("player", p->getName());
      projectile.insert("x", (double)pr->getPosX());
      projectile.insert("y", (double)pr->getPosY());
      projectile.insert("z", (double)pr->getPosZ());
      projectile.insert("vx", (double)pr->getDirX());
      projectile.insert("vy", (double)pr->getDirY());
      projectile.insert("vz", (double)pr->getDirZ());
      projectile.insert("w", (double)pr->getWidth());
      projectile.insert("h", (double)pr->getHeight());
      projectile.insert("l", (double)pr->getLength());

      /*
      projectile.insert("id", pr->id);
      if(pr->getModifiedProperties().contains("posX")) projectile.insert("x", (double)pr->getPosX());
      if(pr->getModifiedProperties().contains("posY")) projectile.insert("y", (double)pr->getPosY());
      if(pr->getModifiedProperties().contains("posZ")) projectile.insert("z", (double)pr->getPosZ());
      if(pr->getModifiedProperties().contains("dirX")) projectile.insert("vx", (double)pr->getDirX());
      if(pr->getModifiedProperties().contains("dirY")) projectile.insert("vy", (double)pr->getDirY());
      if(pr->getModifiedProperties().contains("dirZ")) projectile.insert("vz", (double)pr->getDirZ());
      if(pr->getModifiedProperties().contains("width")) projectile.insert("w", (double)pr->getWidth());
      if(pr->getModifiedProperties().contains("height")) projectile.insert("h", (double)pr->getHeight());
      if(pr->getModifiedProperties().contains("length")) projectile.insert("l", (double)pr->getLength());

      pr->getModifiedProperties().clear();
      */

      pro << projectile;

      //}
	}
  }

  packet.insert("bullets", pro);
}

void Server::sendToAllPlayers(QByteArray & packet){
  foreach(Player *p, players){
    sendToPlayer(*p, packet);
  }
}

void Server::sendToPlayer(Player & p, QByteArray & packet){
  p.socket->write(packet);
  p.socket->flush();
}

QList<Player*> Server::getPlayers(){
  return this->players.values();
}


void Server::sendUpdateToPlayers(){
  QByteArray r;

  r = forgeUpdate();

  sendToAllPlayers(r);

  //qDebug() << " : Requette envoyée : \n" + r;
}

void Server::sendInitToPlayer(Player & p){
  QByteArray r;

  r = forgeInit();

  sendToPlayer(p, r);

  sendUpdateToPlayers();

  //qDebug() << " : Requette envoyée : \n" + r;
}

GameEngine & Server::getGameEngine(){
  return *this->g;
}









