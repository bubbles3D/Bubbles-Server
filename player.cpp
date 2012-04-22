#include "player.h"

#include "server.h"

Player::Player(QObject *parent) : Sphere(parent) {
  this->reset();
}

bool Player::takeHit(int power){
  bool death = false;

  this->setLife(this->getLife() - power);

  if(this->getLife() <= 0){

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
    this->addKill();
  }
}

void Player::respawn(){
  this->setLife(START_LIFE);

  this->setWidth(PLAYER_DIAMETER_RATIO);
  this->setHeight(PLAYER_DIAMETER_RATIO);
  this->setLength(PLAYER_DIAMETER_RATIO);

  this->setSpeed(SPEED);

  if(this->flag != NULL){
    this->flag->setPosX(this->getPosX());
    this->flag->setPosY(this->getPosY());
    this->flag->setPosZ(this->getPosZ());
    this->flag->setCube(this->getCube());
    this->setFlag(NULL);
  }

  Server::getServer()->getGameEngine().getField().setRespawnPos(*this);
}


void Player::reset(){
  setKeyForwards(false);
  setKeyBackwards(false);
  setKeyLeft(false);
  setKeyRight(false);
  setDirX(1);
  setDirY(0);
  setDirZ(0);
  setDeads(0);
  setKills(0);
  setPosY(getHeight() * RATIO);
  setCube(1);
  setFlag(NULL);

  this->respawn();
}

void Player::fire(){
  Projectile * p = new Projectile(this);

  this->projectiles.push_back(p);
}

void Player::setFloorDistance(){
  switch(this->getCube()){
  case 1:
    this->setPosY(this->getRadius());
    break;

  case 2:
    this->setPosZ(this->getRadius());
    break;

  case 3:
    this->setPosX(Server::getServer()->getGameEngine().getField().maxX - this->getRadius());
    break;

  case 4:
    this->setPosZ(Server::getServer()->getGameEngine().getField().maxX - this->getRadius());
    break;

  case 5:
    this->setPosY(Server::getServer()->getGameEngine().getField().maxX - this->getRadius());
    break;

  case 6:
    this->setPosX(this->getRadius());
    break;

  }
}

QString Player::getName(){
  return this->name;
}

void Player::setName(QString name){
  if(this->name != name){
    this->name = name;
    this->modifiedProperties.insert("name", name);
  }
}

bool Player::getKeyForwards(){
  return this->keyForwards;
}

void Player::setKeyForwards(bool keyForwards){
  if(this->keyForwards != keyForwards){
    this->keyForwards = keyForwards;
    this->modifiedProperties.insert("keyForwards", keyForwards ? "true" : "false");
  }
}

bool Player::getKeyBackwards(){
  return this->keyBackwards;
}

void Player::setKeyBackwards(bool keyBackwards){
  if(this->keyBackwards != keyBackwards){
    this->keyBackwards = keyBackwards;
    this->modifiedProperties.insert("keyBackwards", keyBackwards ? "true" : "false");
  }
}

bool Player::getKeyLeft(){
  return this->keyLeft;
}

void Player::setKeyLeft(bool keyLeft){
  if(this->keyLeft != keyLeft){
    this->keyLeft = keyLeft;
    this->modifiedProperties.insert("keyLeft", keyLeft ? "true" : "false");
  }
}

bool Player::getKeyRight(){
  return this->keyRight;
}

void Player::setKeyRight(bool keyRight){
  if(this->keyRight != keyRight){
    this->keyRight = keyRight;
    this->modifiedProperties.insert("keyRight", keyRight ? "true" : "false");
  }
}

int Player::getSpeed(){
  return this->speed;
}

void Player::setSpeed(int speed){
  if(this->speed != speed){
    this->speed = speed;
    this->modifiedProperties.insert("speed", QString::number(speed));
  }
}

int Player::getLife(){
  return this->life;
}

void Player::setLife(int life){
  if(this->life != life){
    this->life = life;
    this->modifiedProperties.insert("life", QString::number(life));
  }
}

int Player::getDeads(){
  return this->deads;
}

void Player::setDeads(int deads){
  if(this->deads != deads){
    this->deads = deads;
    this->modifiedProperties.insert("deads", QString::number(deads));
  }
}

int Player::getKills(){
  return this->kills;
}

void Player::setKills(int kills){
  if(this->kills != kills){
    this->kills = kills;
    this->modifiedProperties.insert("kills", QString::number(kills));
  }
}

void Player::addKill(){
  if(Server::getServer()->getGameDirector().getGameType() == 2 && this->team != NULL){ //En cas de TDM
    this->team->addTeamPoints(1);
  }

  this->kills++;
  this->modifiedProperties.insert("kills", QString::number(kills));
}

Team * Player::getTeam(){
  return this->team;
}

int Player::getTeamNum(){
  if(this->team == NULL){
    return 0;
  } else {
    return this->team->getNum();
  }
}

void Player::setTeam(Team * team){
  if(this->team != team){
    this->team = team;
    this->modifiedProperties.insert("team", QString::number(this->getTeamNum()));
  }
}

int Player::getFlagId(){
  return this->flag == NULL ? 0 : this->flag->id;
}

Flag * Player::getFlag(){
  return this->flag;
}

void Player::setFlag(Flag * flag){
  if(this->flag != flag){
    this->flag = flag;
    this->modifiedProperties.insert("flag", QString::number(getFlagId()));
  }
}

int Player::getColorRED(){
  if(this->team != NULL){
    return this->team->getColorRED();
  } else {
    return Sphere::getColorRED();
  }
}

int Player::getColorGREEN(){
  if(this->team != NULL){
    return this->team->getColorGREEN();
  } else {
    return Sphere::getColorGREEN();
  }
}

int Player::getColorBLUE(){
  if(this->team != NULL){
    return this->team->getColorBLUE();
  } else {
    return Sphere::getColorBLUE();
  }
}
