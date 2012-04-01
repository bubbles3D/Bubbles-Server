#include "player.h"

#include "server.h"

Player::Player(QObject *parent) : Sphere(parent) {
  setKeyForwards(false);
  setKeyBackwards(false);
  setKeyLeft(false);
  setKeyRight(false);
  setSpeed(SPEED);
  setDirX(1);
  setDirY(0);
  setDirZ(0);
  setLife(START_LIFE);
  setDeads(0);
  setKills(0);
  setWidth(PLAYER_DIAMETER_RATIO);
  setHeight(PLAYER_DIAMETER_RATIO);
  setLength(PLAYER_DIAMETER_RATIO);
  setPosY(getHeight() * RATIO);
  setCube(1);

  this->respawn();
}

bool Player::takeHit(int power){
  bool death = false;

  this->setLife(this->getLife() - power);

  if(this->getLife() <= 0){
    this->setLife(START_LIFE);

    this->setDeads(this->getDeads() + 1);

	respawn();

	death = true;
  }

  double diameterDiff = MAX_DIAMETER - MIN_DIAMETER;

  this->setWidth(((diameterDiff * this->getLife() / MAX_LIFE) + MIN_DIAMETER) * PLAYER_DIAMETER_RATIO);
  this->setHeight(this->getWidth());
  this->setLength(this->getWidth());

  setFloorDistance();

  return death;
}

void Player::hitPlayer(Player & p, int power){
  if(this->getLife() < MAX_LIFE){
    this->setLife(this->getLife() + 1);

	double diameterDiff = MAX_DIAMETER - MIN_DIAMETER;

    this->setWidth(((diameterDiff * this->getLife() / MAX_LIFE) + MIN_DIAMETER) * PLAYER_DIAMETER_RATIO);
    this->setHeight(this->getWidth());
    this->setLength(this->getWidth());

    setFloorDistance();
  }

  if(p.takeHit(power)){
    this->setKills(this->getKills() + 1);
  }
}

void Player::respawn(){
  Server::getServer()->getGameEngine().getField().setRespawnPos(*this);
}

void Player::fire(){
  Projectile * p = new Projectile(this);

  this->projectiles.push_back(p);
}

void Player::setFloorDistance(){
  switch(this->getCube()){
  case 1:
    this->setPosY(this->getWidth() * RATIO);
    break;

  case 2:
    this->setPosZ(this->getWidth() * RATIO);
    break;

  case 3:
    this->setPosX(Server::getServer()->getGameEngine().getField().maxX - (this->getWidth() * RATIO));
    break;

  case 4:
    this->setPosZ(Server::getServer()->getGameEngine().getField().maxX - (this->getWidth() * RATIO));
    break;

  case 5:
    this->setPosY(Server::getServer()->getGameEngine().getField().maxX - (this->getWidth() * RATIO));
    break;

  case 6:
    this->setPosX(this->getWidth() * RATIO);
    break;

  }
}

QString Player::getName(){
  return this->name;
}

void Player::setName(QString name){
  this->name = name;
}

bool Player::getKeyForwards(){
  return this->keyForwards;
}

void Player::setKeyForwards(bool keyForwards){
  this->keyForwards = keyForwards;
}

bool Player::getKeyBackwards(){
  return this->keyBackwards;
}

void Player::setKeyBackwards(bool keyBackwards){
  this->keyBackwards = keyBackwards;
}

bool Player::getKeyLeft(){
  return this->keyLeft;
}

void Player::setKeyLeft(bool keyLeft){
  this->keyLeft = keyLeft;
}

bool Player::getKeyRight(){
  return this->keyRight;
}

void Player::setKeyRight(bool keyRight){
  this->keyRight = keyRight;
}

int Player::getSpeed(){
  return this->speed;
}

void Player::setSpeed(int speed){
  this->speed = speed;
}

int Player::getLife(){
  return this->life;
}

void Player::setLife(int life){
  this->life = life;
}

int Player::getDeads(){
  return this->deads;
}

void Player::setDeads(int deads){
  this->deads = deads;
}

int Player::getKills(){
  return this->kills;
}

void Player::setKills(int kills){
  this->kills = kills;
}

int Player::getCube(){
  return this->cube;
}

void Player::setCube(int cube){
  this->cube = cube;
}
