#include "gameobject.h"

int GameObject::idCounter = 0;

GameObject::GameObject(QObject *parent) : QObject(parent){
    this->id = idCounter;
    idCounter++;
}
