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
  gd = new GameDirector(this);
}

void Server::init(){
  if (!listen(QHostAddress::Any, this->port)) { // Demarrage du serveur sur toutes les IP disponibles

    // Si le serveur n'a pas ete demarre correctement
	qDebug() << "Le serveur n'a pas pu etre demarre. Raison :" + errorString();
  } else {
    // Si le serveur a ete demarre correctement

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

    this->gd->startFun();
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

  this->gd->addPlayerToGame(p);

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

  this->toClear << p->id;
  foreach(Projectile * pr, p->projectiles){
    this->toClear << pr->id;
  }

  p->deleteLater();

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

    if(req.split(' ').size() > 2){
      p.setColorRED(req.split(' ')[2].toInt());
      p.setColorGREEN(req.split(' ')[3].toInt());
      p.setColorBLUE(req.split(' ')[4].toInt());
    }

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


    this->gd->getGameEngine().modif = true;
  }

  p.request.remove(req + "$$");
}

QByteArray Server::forgeInit(){

  QVariantMap packet;
  QByteArray json;

  forgeFieldInfo(packet);
  forgePlayersInfo(packet, true);
  forgeProjectilesInfo(packet, true);
  forgeGameInfo(packet);
  forgeTeamsInfo(packet, true);
  forgeFlagsInfo(packet, true);

  if(packet.size() > 0){
    QJson::Serializer serializer;
    json = serializer.serialize(packet);

    json.append("$$");
  }

  return json;


}

QByteArray Server::forgeUpdate(){

  QVariantMap packet;
  QByteArray json;

  forgePlayersInfo(packet);
  forgeProjectilesInfo(packet);
  forgeToClearInfo(packet);
  forgeTeamsInfo(packet);
  forgeFlagsInfo(packet);

  if(packet.size() > 0){
    QJson::Serializer serializer;
    json = serializer.serialize(packet);

    json.append("$$");
  }

  return json;
}

void Server::forgeFieldInfo(QVariantMap & packet){

  QVariantMap fld;

  fld.insert("width", this->gd->getGameEngine().getField().maxX);
  fld.insert("height", this->gd->getGameEngine().getField().maxZ);

  QVariantList obs;

  foreach(Obstacle *o, this->gd->getGameEngine().getField().obstacles){
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

void Server::forgePlayersInfo(QVariantMap & packet, bool force){

  QVariantList pls;

  foreach(Player *p, this->players){
    if(force || p->getModifiedProperties().size() > 0 ) {

      QVariantMap player;

      player.insert("id", p->id);
      if(force || p->getModifiedProperties().contains("name")) player.insert("name", p->getName());
      if(force || p->getModifiedProperties().contains("posX")) player.insert("x", (double)p->getPosX());
      if(force || p->getModifiedProperties().contains("posY")) player.insert("y", (double)p->getPosY());
      if(force || p->getModifiedProperties().contains("posZ")) player.insert("z", (double)p->getPosZ());
      if(force || p->getModifiedProperties().contains("dirX")) player.insert("vx", (double)p->getDirX());
      if(force || p->getModifiedProperties().contains("dirY")) player.insert("vy", (double)p->getDirY());
      if(force || p->getModifiedProperties().contains("dirZ")) player.insert("vz", (double)p->getDirZ());
      if(force || p->getModifiedProperties().contains("life")) player.insert("life", p->getLife());
      if(force || p->getModifiedProperties().contains("deaths")) player.insert("deaths", p->getDeads());
      if(force || p->getModifiedProperties().contains("kills")) player.insert("kills", p->getKills());
      if(force || p->getModifiedProperties().contains("width")) player.insert("w", (double)p->getWidth());
      if(force || p->getModifiedProperties().contains("height")) player.insert("h", (double)p->getHeight());
      if(force || p->getModifiedProperties().contains("length")) player.insert("l", (double)p->getLength());
      if(force || p->getModifiedProperties().contains("cube")) player.insert("cube", p->getCube());
      if(force || p->getModifiedProperties().contains("colorRED")) player.insert("r", p->getColorRED());
      if(force || p->getModifiedProperties().contains("colorGREEN")) player.insert("g", p->getColorGREEN());
      if(force || p->getModifiedProperties().contains("colorBLUE")) player.insert("b", p->getColorBLUE());
      if(force || p->getModifiedProperties().contains("team")) player.insert("team", p->getTeamNum());
      if(force || p->getModifiedProperties().contains("flag")) player.insert("flag", p->getFlagId());

      if(!force){ //On ne veut pas effacer les modifs en cas de force
        p->getModifiedProperties().clear();
      }

      if(player.size() > 1){ //Si on a donné autre chose que l'id..
        pls << player;
      }

    }
  }

  if(pls.size() > 0){
    packet.insert("players", pls);
  }
}

void Server::forgeProjectilesInfo(QVariantMap & packet, bool force){
  QVariantList pro;

  foreach(Player *p, this->players){
    foreach(Projectile *pr, p->projectiles){
      if(force || pr->getModifiedProperties().size() > 0 ) {

        QVariantMap projectile;

        projectile.insert("id", pr->id);
        if(force || pr->getModifiedProperties().contains("posX")) projectile.insert("x", (double)pr->getPosX());
        if(force || pr->getModifiedProperties().contains("posY")) projectile.insert("y", (double)pr->getPosY());
        if(force || pr->getModifiedProperties().contains("posZ")) projectile.insert("z", (double)pr->getPosZ());
        if(force || pr->getModifiedProperties().contains("dirX")) projectile.insert("vx", (double)pr->getDirX());
        if(force || pr->getModifiedProperties().contains("dirY")) projectile.insert("vy", (double)pr->getDirY());
        if(force || pr->getModifiedProperties().contains("dirZ")) projectile.insert("vz", (double)pr->getDirZ());
        if(force || pr->getModifiedProperties().contains("width")) projectile.insert("w", (double)pr->getWidth());
        if(force || pr->getModifiedProperties().contains("height")) projectile.insert("h", (double)pr->getHeight());
        if(force || pr->getModifiedProperties().contains("length")) projectile.insert("l", (double)pr->getLength());
        if(force || pr->getModifiedProperties().contains("colorRED")) projectile.insert("r", pr->getColorRED());
        if(force || pr->getModifiedProperties().contains("colorGREEN")) projectile.insert("g", pr->getColorGREEN());
        if(force || pr->getModifiedProperties().contains("colorBLUE")) projectile.insert("b", pr->getColorBLUE());

        if(!force){
          pr->getModifiedProperties().clear();
        }

        if(projectile.size() > 1){ //Si on a donné autre chose que l'id..
          pro << projectile;
        }

      }
	}
  }

  if(pro.size() > 0){
    packet.insert("bullets", pro);
  }
}


void Server::forgeToClearInfo(QVariantMap & packet){
  QVariantList pro;

  foreach(int i, this->toClear){
    pro << i;
  }

  this->toClear.clear();

  if(pro.size() > 0){
    packet.insert("toClear", pro);
  }
}


void Server::forgeGameInfo(QVariantMap & packet){
  QVariantMap gameInf;

  gameInf.insert("gameTime", this->gd->getRemainingGameTime());
  gameInf.insert("pauseTime", this->gd->getRemainingPauseTime());
  gameInf.insert("gameType", this->gd->getGameType());

  packet.insert("game", gameInf);
}

void Server::forgeTeamsInfo(QVariantMap & packet, bool force){
  QVariantList teamList;

  foreach(Team * t, this->gd->getTeams()){
    QVariantMap team;

    team.insert("num", t->getNum());
    if(force) team.insert("cR", t->getColorRED());
    if(force) team.insert("cG", t->getColorGREEN());
    if(force) team.insert("cB", t->getColorBLUE());
    if(force) team.insert("name", t->getName());
    if(force || t->getModifiedProperties().contains("points")) team.insert("pts", t->getPoints());

    if(!force){
      t->getModifiedProperties().clear();
    }

    if(team.size() > 1){
      teamList << team;
      qDebug() << "CACACACA  1";
    }
  }

  if(!teamList.isEmpty()){
    packet.insert("teams", teamList);
    qDebug() << "CACACACA  2";
  }
}

void Server::forgeFlagsInfo(QVariantMap & packet, bool force){
  QVariantList flagsList;

  foreach(Flag * f, this->gd->getFlags()){
    QVariantMap flag;

    flag.insert("id", f->id);
    if(force || f->getModifiedProperties().contains("colorRED")) flag.insert("cR", f->getColorRED());
    if(force || f->getModifiedProperties().contains("colorRED")) flag.insert("cG", f->getColorGREEN());
    if(force || f->getModifiedProperties().contains("colorRED")) flag.insert("cB", f->getColorBLUE());
    if(force) flag.insert("team", f->getTeam()->getNum());
    if(force || f->getModifiedProperties().contains("posX")) flag.insert("x", f->getPosX());
    if(force || f->getModifiedProperties().contains("posY")) flag.insert("y", f->getPosY());
    if(force || f->getModifiedProperties().contains("posZ")) flag.insert("z", f->getPosZ());
    if(force || f->getModifiedProperties().contains("cube")) flag.insert("cube", f->getCube());

    if(!force){
      f->getModifiedProperties().clear();
    }

    if(flag.size() > 1){
      flagsList << flag;
    }
  }

  if(!flagsList.isEmpty()){
    packet.insert("flags", flagsList);
  }
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

  if(r.size() > 0){
    sendToAllPlayers(r);

    qDebug() << " : Requette envoyee : \n" + r;
  }
}

void Server::sendInitToAllPlayers(){
  QByteArray r;

  r = forgeInit();

  if(r.size() > 0 && ! this->players.isEmpty()){
    sendToAllPlayers(r);

    qDebug() << " : Requette envoyee : \n" + r;
  }
}

void Server::sendInitToPlayer(Player & p){
  QByteArray r;

  r = forgeInit();

  sendToPlayer(p, r);

  sendUpdateToPlayers();

  qDebug() << " : Requette envoyee : \n" + r;
}

GameEngine & Server::getGameEngine(){
  return this->gd->getGameEngine();
}

GameDirector & Server::getGameDirector(){
  return *this->gd;
}


void Server::addObjectToClear(int id){
  this->toClear << id;
}









