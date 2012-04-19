#include "gamedirector.h"

GameDirector::GameDirector(QObject *parent) : QObject(parent) {
  this->g = new GameEngine(this);

  this->gameTimer = new QTimer(this);
  this->gameTimer->setSingleShot(true);
  connect(this->gameTimer, SIGNAL(timeout()), this, SLOT(stopGame()));

  this->pauseTimer = new QTimer(this);
  this->pauseTimer->setSingleShot(true);
  connect(this->pauseTimer, SIGNAL(timeout()), this, SLOT(newGame()));


}

void GameDirector::startFun(){
  newGame();
}

GameEngine & GameDirector::getGameEngine(){
  return *(this->g);
}


void GameDirector::newGame(){
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
