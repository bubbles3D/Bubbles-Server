#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#define FPS 50

#include <QObject>
#include <QTimer>
#include <QTime>

#include "player.h"
#include "server.h"
#include "field.h"
#include "projectile.h"

class Server;
class Field;

class GameEngine : public QObject {
    Q_OBJECT
public:
    explicit GameEngine(Server* server);

    bool modif;

    void init();

    Field & getField();

    bool colideObject(GameObject* ob);

signals:

public slots:

private slots:
    void updateObjects();

private:
    Server* server;

    int fps;
    int msIdealUpdate;

    QTimer * timer;

    Field field;

    void updatePlayers();
    void updateProjectiles();
    void manageColisions();

    bool colide(GameObject *o1, GameObject *o2);
    bool colide(Sphere *s1, Sphere *s2);
    bool colide(Sphere *s, Cuboid *c);

    bool colidePlayerProjectile(Player *pl, Projectile *pr);
    bool colidePlayerPlayer(Player *p1, Player *p2);
    bool colideProjectileProjectile(Projectile *p1, Projectile *p2);

    bool colideProjectileObstacle(Projectile *p, Obstacle *o);
    bool colidePlayerObstacle(Player *p, Obstacle *o);
};

#endif // GAMEENGINE_H
