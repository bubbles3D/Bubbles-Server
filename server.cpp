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
	p.name = req.split(' ')[1];

	sendInitToPlayers();
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
			p.keyForwards = m["state"].toBool();
		    } else if(m["name"].toString() == "DOWN"){
			p.keyBackwards = m["state"].toBool();
		    } else if(m["name"].toString() == "RIGHT"){
			p.keyRight = m["state"].toBool();
		    } else if(m["name"].toString() == "LEFT"){
			p.keyLeft = m["state"].toBool();
		    } else if(m["name"].toString() == "MOUSE"){
			QVariantMap mouse =  m["state"].toMap();

			p.dirX = mouse["x"].toFloat();
			p.dirY = mouse["y"].toFloat();
			p.dirZ = mouse["z"].toFloat();
		    } else {
			qDebug() << "Key name unknown for player : " + p.name;
		    }
		}
	    }
	}

	if(!result["shoot"].toMap().isEmpty()){
	    QVariantMap m = result["shoot"].toMap();

	    if(!m["name"].toString().isEmpty()){
		if(m["name"].toString() == "MOUSE"){
		    QVariantMap mouse =  m["state"].toMap();

		    p.dirX = mouse["x"].toFloat();
		    p.dirY = mouse["y"].toFloat();
		    p.dirZ = mouse["z"].toFloat();

		    p.fire();
		} else {
		    qDebug() << "Key name unknown for bullet : " + p.name;
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
	ob.insert("x", o->posX);
	ob.insert("y", o->posY);
	ob.insert("z", o->posZ);
	ob.insert("w", o->width);
	ob.insert("h", o->height);
	ob.insert("l", o->length);

	obs << ob;
    }

    fld.insert("obstacles", obs);

    packet.insert("field", fld);
}

void Server::forgePlayersInfo(QVariantMap & packet){

    QVariantList pls;

    foreach(Player *p, this->players){
	QVariantMap player;
	player.insert("name", p->name);
	player.insert("id", p->id);
    player.insert("x", (double)p->posX);
    player.insert("y", (double)p->posY);
    player.insert("z", (double)p->posZ);
    player.insert("vx", (double)p->dirX);
    player.insert("vy", (double)p->dirY);
    player.insert("vz", (double)p->dirZ);
	player.insert("life", p->life);
	player.insert("deaths", p->deads);
	player.insert("kills", p->kills);
    player.insert("w", (double)p->width);
    player.insert("h", (double)p->height);
    player.insert("l", (double)p->length);
	player.insert("cube", p->cube);

	pls << player;
    }

    packet.insert("players", pls);
}

void Server::forgeProjectilesInfo(QVariantMap & packet){
    QVariantList pro;

    foreach(Player *p, this->players){
	foreach(Projectile *pr, p->projectiles){
	    QVariantMap projectile;
	    projectile.insert("player", p->name);
	    projectile.insert("id", pr->id);
        projectile.insert("x", (double)pr->posX);
        projectile.insert("y", (double)pr->posY);
        projectile.insert("z", (double)pr->posZ);
        projectile.insert("vx", (double)pr->dirX);
        projectile.insert("vy", (double)pr->dirY);
        projectile.insert("vz", (double)pr->dirZ);
        projectile.insert("w", (double)pr->width);
        projectile.insert("h", (double)pr->height);
        projectile.insert("l", (double)pr->length);

	    pro << projectile;
	}
    }

    packet.insert("bullets", pro);
}

void Server::sendToPlayers(QByteArray packet){
    foreach(Player *p, players){
	p->socket->write(packet);
	p->socket->flush();
    }
}

QList<Player*> Server::getPlayers(){
    return this->players.values();
}


void Server::sendUpdateToPlayers(){
    QByteArray r;

    r = forgeUpdate();

    sendToPlayers(r);

    //qDebug() << " : Requette envoyée : \n" + r;
}

void Server::sendInitToPlayers(){
    QByteArray r;

    r = forgeInit();

    sendToPlayers(r);

    //qDebug() << " : Requette envoyée : \n" + r;
}

GameEngine & Server::getGameEngine(){
    return *this->g;
}









