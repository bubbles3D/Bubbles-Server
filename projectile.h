#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "sphere.h"
#include "player.h"

#define DEFAULT_SPEED 6000
#define PROJECTILE_DIAMETER_RATIO 6
#define DEFAULT_POWER 1

class Player;

class Projectile : public Sphere
{
    Q_OBJECT
public:
    Projectile(Player * playerOwner);

    void explode();


    Player * owner;
    float speed;

    int power;
};

#endif // PROJECTILE_H
