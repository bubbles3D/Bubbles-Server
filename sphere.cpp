#include "sphere.h"

Sphere::Sphere(QObject *parent) : GameObject(parent) {
}

double Sphere::getRadius(){
    return this->getWidth() / 2;
}
