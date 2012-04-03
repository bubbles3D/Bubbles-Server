#include "gameobject.h"

int GameObject::idCounter = 0;

GameObject::GameObject(QObject *parent) : QObject(parent){
  this->id = idCounter;
  idCounter++;
}

QMap<QString, QString> & GameObject::getModifiedProperties(){
  return this->modifiedProperties;
}


float GameObject::getPosX(){
  return this->posX;
}

void GameObject::setPosX(float posX){
  if(this->posX != posX){
    this->posX = posX;
    this->modifiedProperties.insert("posX", QString::number(posX));
  }
}

float GameObject::getPosY(){
  return this->posY;
}

void GameObject::setPosY(float posY){
  if(this->posY != posY){
    this->posY = posY;
    this->modifiedProperties.insert("posY", QString::number(posY));
  }
}

float GameObject::getPosZ(){
  return this->posZ;
}

void GameObject::setPosZ(float posZ){
  if(this->posZ != posZ){
    this->posZ = posZ;
    this->modifiedProperties.insert("posZ", QString::number(posZ));
  }
}

float GameObject::getOldPosX(){
  return this->oldPosX;
}

void GameObject::setOldPosX(float oldPosX){
  if(this->oldPosX != oldPosX){
    this->oldPosX = oldPosX;
    this->modifiedProperties.insert("oldPosX", QString::number(oldPosX));
  }
}

float GameObject::getOldPosY(){
  return this->oldPosY;
}

void GameObject::setOldPosY(float oldPosY){
  if(this->oldPosY != oldPosY){
    this->oldPosY = oldPosY;
    this->modifiedProperties.insert("oldPosY", QString::number(oldPosY));
  }
}

float GameObject::getOldPosZ(){
  return this->oldPosZ;
}

void GameObject::setOldPosZ(float oldPosZ){
  if(this->oldPosZ != oldPosZ){
    this->oldPosZ = oldPosZ;
    this->modifiedProperties.insert("oldPosZ", QString::number(oldPosZ));
  }
}

float GameObject::getDirX(){
  return this->dirX;
}

void GameObject::setDirX(float dirX){
  if(this->dirX != dirX){
    this->dirX = dirX;
    this->modifiedProperties.insert("dirX", QString::number(dirX));
  }
}

float GameObject::getDirY(){
  return this->dirY;
}

void GameObject::setDirY(float dirY){
  if(this->dirY != dirY){
    this->dirY = dirY;
    this->modifiedProperties.insert("dirY", QString::number(dirY));
  }
}

float GameObject::getDirZ(){
  return this->dirZ;
}

void GameObject::setDirZ(float dirZ){
  if(this->dirZ != dirZ){
    this->dirZ = dirZ;
    this->modifiedProperties.insert("dirZ", QString::number(dirZ));
  }
}

float GameObject::getWidth(){
  return this->width;
}

void GameObject::setWidth(float width){
  if(this->width != width){
    this->width = width;
    this->modifiedProperties.insert("width", QString::number(width));
  }
}

float GameObject::getHeight(){
  return this->height;
}

void GameObject::setHeight(float height){
  if(this->height != height){
    this->height = height;
    this->modifiedProperties.insert("height", QString::number(height));
  }
}

float GameObject::getLength(){
  return this->length;
}

void GameObject::setLength(float length){
  if(this->length != length){
    this->length = length;
    this->modifiedProperties.insert("length", QString::number(length));
  }
}

int GameObject::getColorRED(){
  return this->colorRED;
}

void GameObject::setColorRED(int colorRED){
  if(this->colorRED != colorRED){
    this->colorRED = colorRED;
    this->modifiedProperties.insert("colorRED", QString::number(colorRED));
  }
}

int GameObject::getColorGREEN(){
  return this->colorGREEN;
}

void GameObject::setColorGREEN(int colorGREEN){
  if(this->colorGREEN != colorGREEN){
    this->colorGREEN = colorGREEN;
    this->modifiedProperties.insert("colorGREEN", QString::number(colorGREEN));
  }
}

int GameObject::getColorBLUE(){
  return this->colorBLUE;
}

void GameObject::setColorBLUE(int colorBLUE){
  if(this->colorBLUE != colorBLUE){
    this->colorBLUE = colorBLUE;
    this->modifiedProperties.insert("colorBLUE", QString::number(colorBLUE));
  }
}
