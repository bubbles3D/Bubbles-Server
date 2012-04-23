#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#define FPS 40

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
    explicit GameEngine(QObject* server);

    bool modif;

    void init();
    void stop();

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

    QTime time;

    Field field;

    void updatePlayers(int msSinceLastUpdate);
    void updateProjectiles(int msSinceLastUpdate);
    void manageColisions();

    void deleteDeleted();

    bool processColision(GameObject *o1, GameObject *o2);
    bool processColision(Sphere *s1, Sphere *s2);
    bool processColision(Sphere *s, Cuboid *c);

    bool colide(Sphere *s1, Sphere *s2);
    bool colide(Sphere *s, Cuboid *c);

    bool approximateColision(Sphere *s1, Sphere *s2);
    bool approximateColision(Sphere *s, Cuboid *c);

    bool colidePlayerProjectile(Player *pl, Projectile *pr);
    bool colidePlayerPlayer(Player *p1, Player *p2);
    bool colideProjectileProjectile(Projectile *p1, Projectile *p2);

    bool colideProjectileObstacle(Projectile *p, Obstacle *o);
    bool colidePlayerObstacle(Player *p, Obstacle *o);
    bool colidePlayerFlag(Player *p, Flag *f);

    float absF(float value);
};

#endif // GAMEENGINE_H
