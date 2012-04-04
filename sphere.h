#ifndef SPHERE_H
#define SPHERE_H

#include "gameobject.h"

class Sphere : public GameObject
{
    Q_OBJECT
public:
    Sphere(QObject *parent = 0);

    double getRadius();

private:
};

#endif // SPHERE_H
