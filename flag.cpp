#include "flag.h"

Flag::Flag(float initialPosX, float initialPosY, float initialPosZ, int initialCube, Team * team, QObject *parent) : Cuboid(parent) {
  this->initialPosX = initialPosX;
  this->initialPosY = initialPosY;
  this->initialPosZ = initialPosZ;
  this->initialCube = initialCube;

  this->setPosX(initialPosX);
  this->setPosY(initialPosY);
  this->setPosZ(initialPosZ);

  this->setCube(initialCube);

  this->team = team;

  this->setColorRED(team->getColorRED());
  this->setColorGREEN(team->getColorGREEN());
  this->setColorBLUE(team->getColorBLUE());

  this->moved = false;

  this->setWidth(FLAG_WIDTH);
  this->setHeight(FLAG_HEIGHT);
  this->setLength(FLAG_LENGTH);
}

bool Flag::hasBeenMoved(){
  return this->moved;
}

void Flag::respawn(){
  this->setPosX(this->initialPosX);
  this->setPosY(this->initialPosY);
  this->setPosZ(this->initialPosZ);

  this->setCube(this->initialCube);

  this->modifiedProperties.insert("posX", QString::number(getPosX()));
  this->modifiedProperties.insert("posY", QString::number(getPosY()));
  this->modifiedProperties.insert("posZ", QString::number(getPosZ()));
  this->modifiedProperties.insert("cube", QString::number(getCube()));
}

Player * Flag::getOwner(){
  return this->owner;
}

void Flag::setOwner(Player * p){
  this->owner = p;
  this->moved = true;
}

Team * Flag::getTeam(){
  return this->team;
}
