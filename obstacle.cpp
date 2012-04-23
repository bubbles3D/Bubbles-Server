#include "obstacle.h"

Obstacle::Obstacle(QObject *parent) : Cuboid(parent) {
  this->life = OBSTACLE_LIFE;
}


bool Obstacle::takeHit(int power){
  this->life -= power;

  if(this->life > 0){
    this->setWidth(((this->getWidth() - OBSTACLE_MIN_SIZE) * this->life / (this->life + power)) + OBSTACLE_MIN_SIZE);
    this->setHeight(((this->getHeight() - OBSTACLE_MIN_SIZE) * this->life / (this->life + power)) + OBSTACLE_MIN_SIZE);
    this->setLength(((this->getLength() - OBSTACLE_MIN_SIZE) * this->life / (this->life + power)) + OBSTACLE_MIN_SIZE);
    return false;
  } else {
    return true;
  }
}
