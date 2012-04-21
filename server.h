#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QtNetwork>

#include <QJson/Parser>
#include <QJson/Serializer>

#include "player.h"
#include "gameengine.h"
#include "logger.h"
#include "gamedirector.h"

class GameEngine;
class GameDirector;
class Player;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    static Server * getServer();
    static void initServer(QObject *parent = 0, int port = 9001);

    void init();

    QList<Player*> getPlayers();

    void sendUpdateToPlayers();
    void sendInitToAllPlayers();

    GameEngine & getGameEngine();
    GameDirector & getGameDirector();

    void addObjectToClear(int id);

signals:

public slots:

private:
    static Server * server;
    explicit Server(QObject *parent = 0, int port = 9001);

    int port;

    GameDirector *gd;

    QHash<QString, Player*> players;

    QList<int> toClear;

    void sendToAllPlayers(QByteArray & packet);
    void sendToPlayer(Player & p, QByteArray & packet);

    void sendInitToPlayer(Player & p);

    void processRequest(Player &p, QString req);

    QByteArray forgeInit();
    QByteArray forgeUpdate();

    void forgePlayersInfo(QVariantMap & packet, bool force = false);
    void forgeProjectilesInfo(QVariantMap & packet, bool force = false);
    void forgeFieldInfo(QVariantMap & packet);
    void forgeToClearInfo(QVariantMap & packet);
    void forgeGameInfo(QVariantMap & packet);
    void forgeTeamsInfo(QVariantMap & packet, bool force = false);
    void forgeFlagsInfo(QVariantMap & packet, bool force = false);


private slots:
    void playerConnected();
    void dataReceived();
    void playerDisconnected();
};

#endif // SERVER_H
