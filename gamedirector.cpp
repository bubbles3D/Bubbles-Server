#include "gamedirector.h"

GameDirector::GameDirector(QObject *parent) : QObject(parent) {
  this->g = new GameEngine(this);

  this->gameTimer = new QTimer(this);
  this->gameTimer->setSingleShot(true);
  connect(this->gameTimer, SIGNAL(timeout()), this, SLOT(stopGame()));

  this->pauseTimer = new QTimer(this);
  this->pauseTimer->setSingleShot(true);
  connect(this->pauseTimer, SIGNAL(timeout()), this, SLOT(newGame()));

  this->gameType = 1;
}

void GameDirector::startFun(){
  newGame();
}

GameEngine & GameDirector::getGameEngine(){
  return *(this->g);
}


void GameDirector::newGame(){
  reset();

  if(this->gameType == 3){
    this->gameType = 1;
    setDM();
  } else if(this->gameType == 1){
    this->gameType = 2;
    setTDM();
  } else if(this->gameType == 2){
    this->gameType = 3;
    setCTF();
  }

  this->g->getField().regenerateMap();

  foreach(Player * p, Server::getServer()->getPlayers()){
    p->reset();
  }

  this->gameTimer->start(GAME_TIME * 1000);

  this->startedTime.start();

  this->g->init();

  Server::getServer()->sendInitToAllPlayers();

  qDebug() << "New Game !! (" << GAME_TIME << " sec)";
}

void GameDirector::stopGame(){
  this->pauseTimer->start(PAUSE_TIME * 1000);

  this->startedTime.start();

  this->g->stop();

  qDebug() << "Stop Game !! ("  << PAUSE_TIME << " sec)";
}

float GameDirector::getRemainingGameTime(){
  if(this->gameTimer->isActive()){
    return GAME_TIME - (this->startedTime.elapsed() / 1000.0);
  } else {
    return 0;
  }
}

float GameDirector::getRemainingPauseTime(){
  if(this->pauseTimer->isActive()){
    return PAUSE_TIME - (this->startedTime.elapsed() / 1000.0);
  } else {
    return PAUSE_TIME;
  }
}

int GameDirector::getGameType(){
  return this->gameType;
}

QString GameDirector::getGameTypeName(){
  QString res = "UNKNOWN";

  switch(this->gameType){
  case 1 :
    res = "DM";
    break;
  case 2:
    res = "TDM";
    break;
  case 3:
    res = "CTF";
    break;
  }

  return res;
}

QList<Team*> GameDirector::getTeams(){
  return this->teams;
}

void GameDirector::reset(){
  foreach(Player * p, Server::getServer()->getPlayers()){
    p->setTeam(NULL);
  }

  foreach(Team * t, this->teams){
    delete t;
  }

  this->teams.clear();



}

void GameDirector::setTeams(){
  Team * t1 = new Team("RED TEAM", 1, 255, 0, 0, 0, this);
  Team * t2 = new Team("BLUE TEAM", 2, 0, 0, 255, 0, this);
  this->teams.append(t1);
  this->teams.append(t2);

  QList<Player*> players = Server::getServer()->getPlayers();

  bool red = rand() % 2 == 1 ? true : false; //Savoir quelle des deux equipes aura le plus de joueurs si impaire..
  int r;
  while(players.size() > 0){
    r = rand() % players.size();
    if(red){
      players.at(r)->setTeam(t1);
      players.removeAt(r);
      red = false;
    } else {
      players.at(r)->setTeam(t2);
      players.removeAt(r);
      red = true;
    }
  }
}

void GameDirector::setDM(){
  //Rien a faire, youpi =D
}

void GameDirector::setTDM(){
  setTeams();
}

void GameDirector::setCTF(){
  setTeams();
}
