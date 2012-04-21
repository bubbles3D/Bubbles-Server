#include "flag.h"

Flag::Flag(float initialPosX, float initialPosY, float initialPosZ, int initialCube, Team * team, QObject *parent) : Cuboid(parent) {
  this->initialPosX = initialPosX;
  this->initialPosY = initialPosY;
  this->initialPosZ = initialPosZ;
  this->initialCube = initialCube;

  this->team = team;

  this->setColorRED(team->colorRED);
  this->setColorGREEN(team->colorGREEN);
  this->setColorBLUE(team->colorBLUE);

  this->moved = false;

  this->setWidth(FLAG_WIDTH);
  this->setHeight(FLAG_HEIGHT);
  this->setLength(FLAG_LENGTH);
}

bool Flag::hasBeenMoved(){
  return this->moved;
}

void Flag::respawn(){
  this->setPosX(initialPosX);
  this->setPosY(initialPosY);
  this->setPosZ(initialPosZ);

  this->setCube(initialCube);
}

Player * Flag::getOwner(){
  return this->owner;
}

Team * Flag::getTeam(){
  return this->team;
}
