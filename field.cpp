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

void Field::setRespawnPos(Player & p, int face){
  bool b = true;

  int f = face == 0 ? rand() % 6 + 1 : face;

  while(b){
    switch(f){
    case 1:
      p.setPosX(rand()%(this->maxX - (int)p.getRadius()));
      p.setPosZ(rand()%(this->maxZ - (int)p.getRadius()));
      p.setPosY(p.getRadius());
      p.setCube(1);
      break;
    case 2:
      p.setPosX(rand()%(this->maxX - (int)p.getRadius()));
      p.setPosY(rand()%(this->maxY - (int)p.getRadius()));
      p.setPosZ(p.getRadius());
      p.setCube(2);
      break;
    case 3:
      p.setPosZ(rand()%(this->maxX - (int)p.getRadius()));
      p.setPosY(rand()%(this->maxY - (int)p.getRadius()));
      p.setPosX(this->maxX - p.getRadius());
      p.setCube(3);
      break;
    case 4:
      p.setPosX(rand()%(this->maxX - (int)p.getRadius()));
      p.setPosY(rand()%(this->maxY - (int)p.getRadius()));
      p.setPosZ(this->maxZ - p.getRadius());
      p.setCube(4);
      break;
    case 5:
      p.setPosX(rand()%(this->maxX - (int)p.getRadius()));
      p.setPosZ(rand()%(this->maxZ - (int)p.getRadius()));
      p.setPosY(this->maxY - p.getRadius());
      p.setCube(5);
      break;
    case 6:
      p.setPosZ(rand()%(this->maxX - (int)p.getRadius()));
      p.setPosY(rand()%(this->maxY - (int)p.getRadius()));
      p.setPosX(p.getRadius());
      p.setCube(6);
      break;
    }

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
