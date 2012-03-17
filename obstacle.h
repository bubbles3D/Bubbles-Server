#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "cuboid.h"

class Obstacle : public Cuboid
{
    Q_OBJECT
public:
    Obstacle(QObject *parent = 0);

    virtual void takeHit(int power = 1);
};

#endif // OBSTACLE_H
