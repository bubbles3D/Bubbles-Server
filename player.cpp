#include "player.h"

#include "server.h"

Player::Player(QObject *parent) : Sphere(parent) {
  keyForwards = false;
  keyBackwards = false;
  keyRight = false;
  keyLeft = false;
  speed = SPEED;
  dirX = 1;
  dirY = 0;
  dirZ = 0;
  life = START_LIFE;
  deads = 0;
  kills = 0;
  width = PLAYER_DIAMETER_RATIO;
  height = PLAYER_DIAMETER_RATIO;
  length = PLAYER_DIAMETER_RATIO;
  posY = height * RATIO;
  cube = 1;

  this->respawn();
}

bool Player::takeHit(int power){
  bool death = false;

  this->life -= power;



  if(life <= 0){
	this->life = START_LIFE;

	this->deads++;

	respawn();

	death = true;
  }

  double diameterDiff = MAX_DIAMETER - MIN_DIAMETER;

  this->width = ((diameterDiff * this->life / MAX_LIFE) + MIN_DIAMETER) * PLAYER_DIAMETER_RATIO;
  this->height = this->width;
  this->length = this->width;

  return death;
}

void Player::hitPlayer(Player & p, int power){
  if(this->life < MAX_LIFE){
	this->life++;

	double diameterDiff = MAX_DIAMETER - MIN_DIAMETER;

	this->width = ((diameterDiff * this->life / MAX_LIFE) + MIN_DIAMETER) * PLAYER_DIAMETER_RATIO;
	this->height = this->width;
	this->length = this->width;
  }

  if(p.takeHit(power)){
	this->kills++;
  }
}

void Player::respawn(){
  Server::getServer()->getGameEngine().getField().setRespawnPos(*this);
}

void Player::fire(){
  Projectile * p = new Projectile(this);

  this->projectiles.push_back(p);
}
