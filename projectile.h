#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "sphere.h"
#include "player.h"

#define DEFAULT_SPEED 15000
#define PROJECTILE_DIAMETER_RATIO 30
#define DEFAULT_POWER 10

class Player;

class Projectile : public Sphere
{
    Q_OBJECT
public:
    Projectile(Player * playerOwner);
    virtual ~Projectile();

    void explode();


    Player * owner;
    float speed;

    int power;
};

#endif // PROJECTILE_H
