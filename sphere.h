#ifndef SPHERE_H
#define SPHERE_H

#include "gameobject.h"

class Sphere : public GameObject
{
    Q_OBJECT
public:
    Sphere(QObject *parent = 0);

    double getRealDiameter();
};

#endif // SPHERE_H
