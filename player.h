#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QtNetwork>

#include "sphere.h"
#include "projectile.h"


#define MIN_DIAMETER 0.3
#define MAX_DIAMETER 2.0
#define MAX_LIFE 20
#define START_LIFE 10
#define SPEED 1200
#define PLAYER_DIAMETER_RATIO 20

class Server;
class Projectile;

class Player : public Sphere
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = 0);

    QAbstractSocket * socket;
    QIPv6Address ip;
    int port;

    QString getName();
    void setName(QString name);

    bool getKeyForwards();
    void setKeyForwards(bool keyForwards);
    bool getKeyBackwards();
    void setKeyBackwards(bool keyBackwards);
    bool getKeyLeft();
    void setKeyLeft(bool keyLeft);
    bool getKeyRight();
    void setKeyRight(bool keyRight);

    int getSpeed();
    void setSpeed(int speed);

    int getLife();
    void setLife(int life);

    int getDeads();
    void setDeads(int deads);
    int getKills();
    void setKills(int kills);

    int getCube();
    void setCube(int cube);

    QString request;
    bool pendingRequest;

    bool takeHit(int power = 1);
    void hitPlayer(Player & p, int power = 1);

    void respawn();

    void fire();

    QList<Projectile *> projectiles;

signals:

public slots:

private:

    void setFloorDistance();

    QString name;

    bool keyForwards;
    bool keyBackwards;
    bool keyLeft;
    bool keyRight;

    int speed;

    int life;

    int deads;
    int kills;

    int cube; //Sur quelle face du cube le joueur est.

};

#endif // PLAYER_H
