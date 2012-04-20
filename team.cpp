#include "team.h"

Team::Team(QString name, int num, int colorRED, int colorGREEN, int colorBLUE, int faceRespawn, QObject *parent) : QObject(parent) {
  this->name = name;
  this->num = num;
  this->colorRED = colorRED;
  this->colorGREEN = colorGREEN;
  this->colorBLUE = colorBLUE;
  this->faceRespawn = faceRespawn;

  this->teamPoints = 0;
}

int Team::getPoints(){
  int res = this->teamPoints;

  /*foreach(Player * p, this->players){
    res += p->getKills();
  }*/

  return res;
}

void Team::addTeamPoints(int teamPoints){
  this->teamPoints += teamPoints;
}

void Team::removeTeamPoints(int teamPoints){
  this->teamPoints -= teamPoints;
}

void Team::addPlayer(Player* p){
  this->players << p;
}

void Team::removePlayer(Player* p){
  this->players.removeOne(p);
}

QString Team::getName(){
  return this->name;
}

int Team::getNum(){
  return this->num;
}

int Team::getColorRED(){
  return this->colorRED;
}

int Team::getColorGREEN(){
  return this->colorGREEN;
}

int Team::getColorBLUE(){
  return this->colorBLUE;
}
