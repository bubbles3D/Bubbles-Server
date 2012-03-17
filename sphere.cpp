#include "sphere.h"

Sphere::Sphere(QObject *parent) : GameObject(parent) {
}

double Sphere::getRealDiameter(){
    return this->width * RATIO;
}
