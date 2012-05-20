#include "gamedirector.h"

GameDirector::GameDirector(QObject *parent) : QObject(parent) {
  this->g = new GameEngine(this);

  this->gameTimer = new QTimer(this);
  this->gameTimer->setSingleShot(true);
  connect(this->gameTimer, SIGNAL(timeout()), this, SLOT(stopGame()));

  this->pauseTimer = new QTimer(this);
  this->pauseTimer->setSingleShot(true);
  connect(this->pauseTimer, SIGNAL(timeout()), this, SLOT(newGame()));

  this->gameType = 3;
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

  Logger::log("New Game !! (" + QString::number(GAME_TIME) + " sec)", Logger::GAME_INFO_2);
}

void GameDirector::stopGame(){
  this->pauseTimer->start(PAUSE_TIME * 1000);

  this->startedTime.start();

  this->g->stop();

  Logger::log("Stop Game !! (" + QString::number(PAUSE_TIME) + " sec)", Logger::GAME_INFO_2);
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

QList<Team*> & GameDirector::getTeams(){
  return this->teams;
}

QList<Flag*> GameDirector::getFlags(){
  return this->flags;
}

void GameDirector::addPlayerToGame(Player * p){
  if(this->gameType == 2 || this->gameType == 3) { //TDM ou CTF
    if(this->teams[0]->getPlayers().size() > this->teams[1]->getPlayers().size()){
      p->setTeam(this->teams[1]);
    } else if(this->teams[1]->getPlayers().size() > this->teams[0]->getPlayers().size()){
      p->setTeam(this->teams[0]);
    } else {
      p->setTeam(this->teams[rand()%2]);
    }
  } else {
    p->setTeam(NULL);
  }

  p->reset();
}

void GameDirector::reset(){
  foreach(Player * p, Server::getServer()->getPlayers()){
    p->setTeam(NULL);
    p->setFlag(NULL);

    foreach(Projectile *pr, p->projectiles){
      Server::getServer()->addObjectToClear(pr->id);
      delete pr;
    }
    p->projectiles.clear();
  }

  foreach(Team * t, this->teams){
    delete t;
  }

  this->teams.clear();

  foreach(Flag * f, this->flags){
    Server::getServer()->addObjectToClear(f->id);
    delete f;
  }

  this->flags.clear();
}

void GameDirector::setTeams(){
  Team * t1 = new Team("RED TEAM", 1, 255, 0, 0, 1, this);
  Team * t2 = new Team("BLUE TEAM", 2, 0, 0, 255, 5, this);
  this->teams << t1;
  this->teams << t2;

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

  Field & f = Server::getServer()->getGameEngine().getField();

  Flag * f1 = new Flag(f.maxX / 2, 25, f.maxZ / 2, 1, this->teams[0], this);
  Flag * f2 = new Flag(f.maxX / 2, f.maxY - 25, f.maxZ / 2, 5, this->teams[1], this);

  this->flags << f1;
  this->flags << f2;
}
