#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QtNetwork>

#include <QJson/Parser>
#include <QJson/Serializer>

#include "player.h"
#include "gameengine.h"
#include "logger.h"

class GameEngine;
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

    GameEngine & getGameEngine();

signals:

public slots:

private:
    static Server * server;
    explicit Server(QObject *parent = 0, int port = 9001);

    int port;

    QHash<QString, Player*> players;

    GameEngine * g;

    void sendToAllPlayers(QByteArray & packet);
    void sendToPlayer(Player & p, QByteArray & packet);

    void sendInitToPlayer(Player & p);

    void processRequest(Player &p, QString req);

    QByteArray forgeInit();
    QByteArray forgeUpdate();
    void forgePlayersInfo(QVariantMap & packet);
    void forgeProjectilesInfo(QVariantMap & packet);
    void forgeFieldInfo(QVariantMap & packet);

private slots:
    void playerConnected();
    void dataReceived();
    void playerDisconnected();
};

#endif // SERVER_H
