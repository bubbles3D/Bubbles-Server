#include "team.h"

Team::Team(QObject *parent) : QObject(parent) {
}

int Team::getPoints(){
  int res = this->teamPoints;

  foreach(Player * p, this->players){
    res += p->getKills();
  }

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
