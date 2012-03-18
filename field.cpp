#include "field.h"
#include "server.h"

Field::Field(QObject *parent) :
    QObject(parent){

    maxX = MAX_FIELD_X;
    maxY = MAX_FIELD_Y;
    maxZ = MAX_FIELD_Z;

    srand(time(NULL));

    regenerateMap();
}

void Field::setRespawnPos(Player & p){
    bool b = true;

    while(b){
	p.posX = rand()%this->maxX - p.width * RATIO;
	p.posZ = rand()%this->maxZ - p.width * RATIO;
	p.posY = p.width * RATIO;

	p.oldPosX = p.posX;
	p.oldPosY = p.posY;
	p.oldPosZ = p.posZ;

	b = Server::getServer()->getGameEngine().colideObject(&p);
    }
}

void Field::regenerateMap(){
    foreach(Obstacle *o, this->obstacles){
	o->deleteLater();
    }

    this->obstacles.clear();

    int nbObjects = (MAX_FIELD_X / 1000) * (MAX_FIELD_Z / 1000) * (MAX_FIELD_Z / 1000) * OBJECTS_DENSITY;

    for(int i = 0; i < nbObjects; i++){
	Obstacle * o = new Obstacle(this);
	o->posX = rand()%this->maxX;
	o->posY = rand()%this->maxY;
	o->posZ = rand()%this->maxY;

	o->width = (rand() % (OBJECTS_MAX_WIDTH - OBJECTS_MIN_WIDTH)) + OBJECTS_MIN_WIDTH;
	o->height = (rand() % (OBJECTS_MAX_HEIGHT - OBJECTS_MIN_HEIGHT)) + OBJECTS_MIN_HEIGHT;
	o->length = (rand() % (OBJECTS_MAX_DEPTH - OBJECTS_MIN_DEPTH)) + OBJECTS_MIN_DEPTH;

	//o->posY = o->height * RATIO / 2;

	this->obstacles << o;
    }
}
