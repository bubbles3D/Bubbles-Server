#include "projectile.h"

Projectile::Projectile(Player * playerOwner) : Sphere(playerOwner){
    this->owner = playerOwner;
    speed = DEFAULT_SPEED;
    this->power = DEFAULT_POWER;

    this->width = PROJECTILE_DIAMETER_RATIO;
    this->height = PROJECTILE_DIAMETER_RATIO;
    this->length = PROJECTILE_DIAMETER_RATIO;

    this->posX = playerOwner->posX;
    this->posY = playerOwner->posY;
    this->posZ = playerOwner->posZ;
    this->dirX = playerOwner->dirX;
    this->dirY = playerOwner->dirY;
    this->dirZ = playerOwner->dirZ;
}

void Projectile::explode(){
    this->owner->projectiles.removeOne(this);

    this->deleteLater();
}
