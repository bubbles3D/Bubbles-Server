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
  if(teamPoints != 0){
    this->teamPoints += teamPoints;

    this->modifiedProperties.insert("points");
  }
}

void Team::removeTeamPoints(int teamPoints){
  if(teamPoints != 0){
    this->teamPoints -= teamPoints;
    if(!this->modifiedProperties.contains("points")) this->modifiedProperties.insert("points");
  }
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

QSet<QString> & Team::getModifiedProperties(){
  return this->modifiedProperties;
}

QList<Player*> & Team::getPlayers(){
  return this->players;
}
