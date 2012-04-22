#include "field.h"
#include "server.h"

Field::Field(QObject *parent) :
  QObject(parent){

  maxX = MAX_FIELD_X;
  maxY = MAX_FIELD_Y;
  maxZ = MAX_FIELD_Z;

  srand(time(NULL));

  //regenerateMap();
}

void Field::setRespawnPos(Player & p){
  bool b = true;

  while(b){
    p.setPosX(rand()%(this->maxX - (int)p.getRadius()));
    p.setPosZ(rand()%(this->maxZ - (int)p.getRadius()));
    p.setPosY(p.getRadius());

    p.setOldPosX(p.getPosX());
    p.setOldPosY(p.getPosY());
    p.setOldPosZ(p.getPosZ());

	b = Server::getServer()->getGameEngine().colideObject(&p);
  }
}

void Field::regenerateMap(){
  foreach(Obstacle *o, this->obstacles){
    Server::getServer()->addObjectToClear(o->id);
	o->deleteLater();
  }

  this->obstacles.clear();

  int nbObjects = (MAX_FIELD_X / 1000) * (MAX_FIELD_Z / 1000) * (MAX_FIELD_Z / 1000) * OBJECTS_DENSITY;

  for(int i = 0; i < nbObjects; i++){
	Obstacle * o = new Obstacle(this);
    o->setPosX(rand()%this->maxX);
    o->setPosY(rand()%this->maxY);
    o->setPosZ(rand()%this->maxY);

    o->setWidth((rand() % (OBJECTS_MAX_WIDTH - OBJECTS_MIN_WIDTH)) + OBJECTS_MIN_WIDTH);
    o->setHeight((rand() % (OBJECTS_MAX_HEIGHT - OBJECTS_MIN_HEIGHT)) + OBJECTS_MIN_HEIGHT);
    o->setLength((rand() % (OBJECTS_MAX_DEPTH - OBJECTS_MIN_DEPTH)) + OBJECTS_MIN_DEPTH);

	this->obstacles << o;
  }
}
