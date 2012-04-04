#include "projectile.h"
#include "server.h"

Projectile::Projectile(Player * playerOwner) : Sphere(playerOwner){
    this->owner = playerOwner;
    speed = DEFAULT_SPEED;
    this->power = DEFAULT_POWER;

    this->setWidth(PROJECTILE_DIAMETER_RATIO);
    this->setHeight(PROJECTILE_DIAMETER_RATIO);
    this->setLength(PROJECTILE_DIAMETER_RATIO);

    this->setPosX(playerOwner->getPosX());
    this->setPosY(playerOwner->getPosY());
    this->setPosZ(playerOwner->getPosZ());
    this->setDirX(playerOwner->getDirX());
    this->setDirY(playerOwner->getDirY());
    this->setDirZ(playerOwner->getDirZ());

    this->setColorRED(playerOwner->getColorRED());
    this->setColorGREEN(playerOwner->getColorGREEN());
    this->setColorBLUE(playerOwner->getColorBLUE());
}

void Projectile::explode(){
    this->owner->projectiles.removeOne(this);

    Server::getServer()->addObjectToClear(this->id);

    this->deleteLater();
}
