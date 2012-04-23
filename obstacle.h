#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "cuboid.h"

#define OBSTACLE_LIFE 15
#define OBSTACLE_MIN_SIZE 20

class Obstacle : public Cuboid
{
    Q_OBJECT
public:
    Obstacle(QObject *parent = 0);

    virtual bool takeHit(int power = 1);

private:
    int life;

};

#endif // OBSTACLE_H
