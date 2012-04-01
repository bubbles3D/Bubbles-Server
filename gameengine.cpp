#include "gameengine.h"

GameEngine::GameEngine(Server* server) : QObject(server), modif(false), server(server), fps(FPS) {
  msIdealUpdate = 1000/this->fps;
}

void GameEngine::init(){
  this->timer = new QTimer(this);
  timer->setSingleShot(true);
  connect(this->timer, SIGNAL(timeout()), this, SLOT(updateObjects()));

  time.start();

  updateObjects();
}

void GameEngine::updateObjects(){
  //QTime t;
  int msSinceLastUpdate = time.restart();

  updatePlayers(msSinceLastUpdate);
  updateProjectiles(msSinceLastUpdate);
  manageColisions();

  if(this->modif) {
	//qDebug() << this->modif;
	server->sendUpdateToPlayers();

	this->modif = false;
  }

  int span = msIdealUpdate - time.restart();

  if(span <= 0){
	span = 1;
  }

  //qDebug() << "Objects updated! Span:" + QString::number(span);

  this->timer->start(span);
}

/*
void GameEngine::updatePlayers(){
    float coeffX;
    float coeffZ;

    //Avancer / Reculer
    foreach(Player *p, server->getPlayers()){

 p->oldPosX = p->posX;
 p->oldPosY = p->posY;
 p->oldPosZ = p->posZ;

 coeffX = p->dirX / (abs(p->dirX) + abs(p->dirZ));
 coeffZ = p->dirZ / (abs(p->dirX) + abs(p->dirZ));

 //Avancer / Reculer
 if(!(p->keyForwards && p->keyBackwards)){
     if(p->keyForwards){
  if(!(p->keyLeft || p->keyRight)){
      p->posX += p->speed * coeffX;
      p->posZ += p->speed * coeffZ;
  } else {
      p->posX += p->speed * coeffX / 2;
      p->posZ += p->speed * coeffZ / 2;
  }
  if(!this->modif)
      this->modif = true;
     } else if(p->keyBackwards){
  if(!(p->keyLeft || p->keyRight)){
      p->posX -= p->speed * coeffX;
      p->posZ -= p->speed * coeffZ;
  } else {
      p->posX -= p->speed * coeffX / 2;
      p->posZ -= p->speed * coeffZ / 2;
  }
  if(!this->modif)
      this->modif = true;
     }
 }

 //Droite / Gauche
 if(!(p->keyLeft && p->keyRight)){
     if(p->keyLeft){
  if(!(p->keyBackwards || p->keyForwards)){
      p->posX += p->speed * coeffZ;
      p->posZ -= p->speed * coeffX;
  } else {
      p->posX += p->speed * coeffZ / 2;
      p->posZ -= p->speed * coeffX / 2;
  }
  if(!this->modif)
      this->modif = true;
     } else if(p->keyRight){
  if(!(p->keyBackwards || p->keyForwards)){
      p->posX -= p->speed * coeffZ;
      p->posZ += p->speed * coeffX;
  } else {
      p->posX -= p->speed * coeffZ / 2;
      p->posZ += p->speed * coeffX / 2;
  }
  if(!this->modif)
      this->modif = true;
     }
 }

 //Pour ne pas depasser les limites de la carte.
 if(p->posX < 0){
     p->posX = 0;
 }
 if(p->posZ < 0){
     p->posZ = 0;
 }

 if(p->posX > field.maxX){
     p->posX = field.maxX;
 }
 if(p->posZ > field.maxZ){
     p->posZ = field.maxZ;
 }
    }
}*/

void GameEngine::updatePlayers(int msSinceLastUpdate){
  float coeffX = 0;
  float coeffZ = 0;

  int XMove = 0;
  int ZMove = 0;

  //Avancer / Reculer
  foreach(Player *p, server->getPlayers()){
	XMove = 0;
	ZMove = 0;

    p->setOldPosX(p->getPosX());
    p->setOldPosY(p->getPosY());
    p->setOldPosZ(p->getPosZ());

    switch(p->getCube()){
	case 1:
      coeffX = p->getDirX() / (abs(p->getDirX()) + abs(p->getDirZ()));
      coeffZ = p->getDirZ() / (abs(p->getDirX()) + abs(p->getDirZ()));
      break;
	case 2:
      coeffZ = p->getDirY() / (abs(p->getDirY()) + abs(p->getDirX()));
      coeffX = - p->getDirX() / (abs(p->getDirY()) + abs(p->getDirX()));
      break;
	case 3:
      coeffZ = p->getDirZ() / (abs(p->getDirZ()) + abs(p->getDirY()));
      coeffX = p->getDirY() / (abs(p->getDirZ()) + abs(p->getDirY()));
      break;
	case 4:
      coeffZ = p->getDirY() / (abs(p->getDirY()) + abs(p->getDirX()));
      coeffX = p->getDirX() / (abs(p->getDirY()) + abs(p->getDirX()));
      break;
	case 5:
      coeffX = p->getDirX() / (abs(p->getDirX()) + abs(p->getDirZ()));
      coeffZ = - p->getDirZ() / (abs(p->getDirX()) + abs(p->getDirZ()));
      break;
	case 6:
      coeffZ = - p->getDirZ() / (abs(p->getDirZ()) + abs(p->getDirY()));
      coeffX = p->getDirY() / (abs(p->getDirZ()) + abs(p->getDirY()));
      break;
	}

    float relativeSpeed = msSinceLastUpdate / 1000.0 * p->getSpeed();

	//Avancer / Reculer
    if(!(p->getKeyForwards() && p->getKeyBackwards())){
      if(p->getKeyForwards()){
        if(!(p->getKeyLeft() || p->getKeyRight())){
          XMove += relativeSpeed * coeffX;
          ZMove += relativeSpeed * coeffZ;
		} else {
          XMove += relativeSpeed * coeffX / 2;
          ZMove += relativeSpeed * coeffZ / 2;
		}
		if(!this->modif)
          this->modif = true;
      } else if(p->getKeyBackwards()){
        if(!(p->getKeyLeft() || p->getKeyRight())){
          XMove -= relativeSpeed * coeffX;
          ZMove -= relativeSpeed * coeffZ;
		} else {
          XMove -= relativeSpeed * coeffX / 2;
          ZMove -= relativeSpeed * coeffZ / 2;
		}
		if(!this->modif)
          this->modif = true;
      }
	}

	//Droite / Gauche
    if(!(p->getKeyLeft() && p->getKeyRight())){
      if(p->getKeyLeft()){
        if(!(p->getKeyBackwards() || p->getKeyForwards())){
          XMove += relativeSpeed * coeffZ;
          ZMove -= relativeSpeed * coeffX;
		} else {
          XMove += relativeSpeed * coeffZ / 2;
          ZMove -= relativeSpeed * coeffX / 2;
		}
		if(!this->modif)
          this->modif = true;
      } else if(p->getKeyRight()){
        if(!(p->getKeyBackwards() || p->getKeyForwards())){
          XMove -= relativeSpeed * coeffZ;
          ZMove += relativeSpeed * coeffX;
		} else {
          XMove -= relativeSpeed * coeffZ / 2;
          ZMove += relativeSpeed * coeffX / 2;
		}
		if(!this->modif)
          this->modif = true;
      }
	}

    float playerRadius = p->getWidth() * RATIO;
	float realMaxField = field.maxX - playerRadius;

	float pos1;
	float pos2;

	float tmp;

    switch(p->getCube()){
	case 1:
      pos1 = p->getPosX() + XMove;
      pos2 = p->getPosZ() + ZMove;

      if(pos1 > realMaxField){
        p->setPosX(realMaxField);
        p->setPosY(playerRadius + pos1 - realMaxField);
        p->setCube(3);
        tmp = p->getDirX();
        p->setDirX(- p->getDirY());
        p->setDirY(tmp);
      } else if(pos1 < playerRadius){
        p->setPosX(playerRadius);
        p->setPosY(playerRadius + playerRadius - pos1);
        p->setCube(6);
        tmp = p->getDirX();
        p->setDirX(p->getDirY());
        p->setDirY(- tmp);
      } else {
        p->setPosX(p->getPosX() + XMove);
      }

      if(pos2 > realMaxField){
        p->setPosZ(realMaxField);
        p->setPosY(playerRadius + pos2 - realMaxField);
        p->setCube(4);
        tmp = p->getDirZ();
        p->setDirZ(- p->getDirY());
        p->setDirY(tmp);
      } else if(pos2 < playerRadius){
        p->setPosZ(playerRadius);
        p->setPosY(playerRadius + playerRadius - pos2);
        p->setCube(2);
        tmp = p->getDirZ();
        p->setDirZ(p->getDirY());
        p->setDirY(- tmp);
      } else {
        p->setPosZ(p->getPosZ() + ZMove);
      }

      break;

	case 2:
      pos1 = p->getPosX() - XMove;
      pos2 = p->getPosY() + ZMove;

      if(pos1 > realMaxField){
        p->setPosX(realMaxField);
        p->setPosZ(playerRadius + pos1 - realMaxField);
        p->setCube(3);
        tmp = p->getDirX();
        p->setDirX(- p->getDirZ());
        p->setDirZ(tmp);
      } else if(pos1 < playerRadius){
        p->setPosX(playerRadius);
        p->setPosZ(playerRadius + playerRadius - pos1);
        p->setCube(6);
        tmp = p->getDirX();
        p->setDirX(p->getDirZ());
        p->setDirZ(- tmp);
      } else {
        p->setPosX(p->getPosX() - XMove);
      }

      if(pos2 > realMaxField){
        p->setPosY(realMaxField);
        p->setPosZ(playerRadius + pos2 - realMaxField);
        p->setCube(5);
        tmp = p->getDirY();
        p->setDirY(- p->getDirZ());
        p->setDirZ(tmp);
      } else if(pos2 < playerRadius){
        p->setPosY(playerRadius);
        p->setPosZ(playerRadius + playerRadius - pos2);
        p->setCube(1);
        tmp = p->getDirZ();
        p->setDirZ(- p->getDirY());
        p->setDirY(tmp);
      } else {
        p->setPosY(p->getPosY() + ZMove);
      }

      break;

	case 3:
      pos1 = p->getPosY() + XMove;
      pos2 = p->getPosZ() + ZMove;

      if(pos1 > realMaxField){
        p->setPosY(realMaxField);
        p->setPosX(realMaxField - (pos1 - realMaxField));
        p->setCube(5);
        tmp = p->getDirX();
        p->setDirX(- p->getDirY());
        p->setDirY(tmp);
      } else if(pos1 < playerRadius){
        p->setPosX(realMaxField - playerRadius - pos1);
        p->setPosY(playerRadius);
        p->setCube(1);
        tmp = p->getDirX();
        p->setDirX(p->getDirY());
        p->setDirY(- tmp);
      } else {
        p->setPosY(p->getPosY() + XMove);
      }

      if(pos2 > realMaxField){
        p->setPosZ(realMaxField);
        p->setPosX(realMaxField - pos2 + realMaxField);
        p->setCube(4);
        tmp = p->getDirX();
        p->setDirX(- p->getDirZ());
        p->setDirZ(tmp);
      } else if(pos2 < playerRadius){
        p->setPosZ(playerRadius);
        p->setPosX(realMaxField - playerRadius + pos2);
        p->setCube(2);
        tmp = p->getDirX();
        p->setDirX(p->getDirZ());
        p->setDirZ(- tmp);
      } else {
        p->setPosZ(p->getPosZ() + ZMove);
      }

      break;

	case 4:
      pos1 = p->getPosX() + XMove;
      pos2 = p->getPosY() + ZMove;

      if(pos1 > realMaxField){
        p->setPosX(realMaxField);
        p->setPosZ(realMaxField - pos1 + realMaxField);
        p->setCube(3);
        tmp = p->getDirX();
        p->setDirX(p->getDirZ());
        p->setDirZ(- tmp);
      } else if(pos1 < playerRadius){
        p->setPosX(playerRadius);
        p->setPosZ(realMaxField - playerRadius + pos1);
        p->setCube(6);
        tmp = p->getDirX();
        p->setDirX(- p->getDirZ());
        p->setDirZ(tmp);
      } else {
        p->setPosX(p->getPosX() + XMove);
      }

      if(pos2 > realMaxField){
        p->setPosY(realMaxField);
        p->setPosZ(realMaxField - pos2 + realMaxField);
        p->setCube(5);
        tmp = p->getDirZ();
        p->setDirZ(- p->getDirY());
        p->setDirY(tmp);
      } else if(pos2 < playerRadius){
        p->setPosY(playerRadius);
        p->setPosZ(realMaxField - playerRadius + pos2);
        p->setCube(1);
        tmp = p->getDirZ();
        p->setDirZ(p->getDirY());
        p->setDirY(- tmp);
      } else {
        p->setPosY(p->getPosY() + ZMove);
      }

      break;


	case 5:
      pos1 = p->getPosX() + XMove;
      pos2 = p->getPosZ() - ZMove;

      if(pos1 > realMaxField){
        p->setPosX(realMaxField);
        p->setPosY(realMaxField - (pos1 - realMaxField));
        p->setCube(3);
        tmp = p->getDirX();
        p->setDirX(p->getDirY());
        p->setDirY(- tmp);
      } else if(pos1 < playerRadius){
        p->setPosX(playerRadius);
        p->setPosY(realMaxField - (playerRadius - pos1));
        p->setCube(6);
        tmp = p->getDirX();
        p->setDirX(- p->getDirY());
        p->setDirY(tmp);
      } else {
        p->setPosX(p->getPosX() + XMove);
      }

      if(pos2 > realMaxField){
        p->setPosZ(realMaxField);
        p->setPosY(realMaxField - pos2 + realMaxField);
        p->setCube(4);
        tmp = p->getDirZ();
        p->setDirZ(p->getDirY());
        p->setDirY(- tmp);
      } else if(pos2 < playerRadius){
        p->setPosZ(playerRadius);
        p->setPosY(realMaxField - playerRadius + pos2);
        p->setCube(2);
        tmp = p->getDirZ();
        p->setDirZ(p->getDirY());
        p->setDirY(- tmp);
      } else {
        p->setPosZ(p->getPosZ() - ZMove);
      }

      break;

	case 6:
      pos1 = p->getPosY() + XMove;
      pos2 = p->getPosZ() - ZMove;

      if(pos1 > realMaxField){
        p->setPosY(realMaxField);
        p->setPosX(playerRadius + pos1 - realMaxField);
        p->setCube(5);
        tmp = p->getDirX();
        p->setDirX(p->getDirY());
        p->setDirY(- tmp);
      } else if(pos1 < playerRadius){
        p->setPosX(playerRadius - pos1 + playerRadius);
        p->setPosY(playerRadius);
        p->setCube(1);
        tmp = p->getDirX();
        p->setDirX(- p->getDirY());
        p->setDirY(tmp);
      } else {
        p->setPosY(p->getPosY() + XMove);
      }

      if(pos2 > realMaxField){
        p->setPosZ(realMaxField);
        p->setPosX(playerRadius + pos2 - realMaxField);
        p->setCube(4);
        tmp = p->getDirX();
        p->setDirX(p->getDirZ());
        p->setDirZ(- tmp);
      } else if(pos2 < playerRadius){
        p->setPosZ(playerRadius);
        p->setPosX(playerRadius + playerRadius - pos2);
        p->setCube(2);
        tmp = p->getDirZ();
        p->setDirZ(p->getDirX());
        p->setDirX(- tmp);
      } else {
        p->setPosZ(p->getPosZ() - ZMove);
      }

      break;
	}

	//Pour ne pas depasser les limites de la carte.
    if(p->getPosX() < 0){
      p->setPosX(0);
    }
    if(p->getPosY() < 0){
      p->setPosY(0);
    }
    if(p->getPosZ() < 0){
      p->setPosZ(0);
    }

    if(p->getPosX() > field.maxX){
      p->setPosX(field.maxX);
    }
    if(p->getPosY() > field.maxY){
      p->setPosY(field.maxY);
    }
    if(p->getPosZ() > field.maxZ){
      p->setPosZ(field.maxZ);
    }
  }
}

void GameEngine::updateProjectiles(int msSinceLastUpdate){

  float coeffX;
  float coeffY;
  float coeffZ;

  float sumCoeff;

  float relativeSpeed;

  foreach(Player *pl, server->getPlayers()){
    foreach(Projectile *p, pl->projectiles){
      p->setOldPosX(p->getPosX());
      p->setOldPosY(p->getPosY());
      p->setOldPosZ(p->getPosZ());

      sumCoeff = abs(p->getDirX()) + abs(p->getDirY()) + abs(p->getDirZ());
      coeffX = p->getDirX() / sumCoeff;
      coeffY = p->getDirY() / sumCoeff;
      coeffZ = p->getDirZ() / sumCoeff;

      relativeSpeed = msSinceLastUpdate / 1000.0 * p->speed;

      p->setPosX(p->getPosX() + relativeSpeed * coeffX);
      p->setPosY(p->getPosY() + relativeSpeed * coeffY);
      p->setPosZ(p->getPosZ() + relativeSpeed * coeffZ);

      if(p->getPosX() < 0 || p->getPosY() < 0 || p->getPosZ() < 0 || p->getPosX() > field.maxX || p->getPosY() > field.maxY ||p->getPosZ() > field.maxZ ){
		pl->projectiles.removeOne(p);
		p->deleteLater();
      }

      if(!this->modif)
		this->modif = true;
	}
  }
}

Field & GameEngine::getField(){
  return this->field;
}

bool GameEngine::colide(GameObject *o1, GameObject *o2){

  if(dynamic_cast<Sphere*>(o1) != NULL && dynamic_cast<Sphere*>(o2) != NULL){
	return colide((Sphere*) o1, (Sphere*) o2);
  } else if(dynamic_cast<Sphere*>(o1) != NULL && dynamic_cast<Cuboid*>(o2) != NULL){
	return colide((Sphere*) o1, (Cuboid*) o2);
  } else if(dynamic_cast<Cuboid*>(o1) != NULL && dynamic_cast<Sphere*>(o2) != NULL) {
	return colide((Sphere*) o2, (Cuboid*) o1);
  }

  return false;
}

bool GameEngine::colide(Sphere *s1, Sphere *s2){
  float d = sqrt(pow(s1->getPosX() - s2->getPosX(), 2) + pow(s1->getPosY() - s2->getPosY(), 2) + pow(s1->getPosZ() - s2->getPosZ(), 2));

  bool b = false;

  if(d < s1->getRealDiameter() + s2->getRealDiameter()){
	//Player - Projectile
	if(dynamic_cast<Projectile*>(s1) != NULL && dynamic_cast<Player*>(s2) != NULL){
      b = colidePlayerProjectile(dynamic_cast<Player*>(s2), dynamic_cast<Projectile*>(s1));
	} else if(dynamic_cast<Player*>(s1) != NULL && dynamic_cast<Projectile*>(s2) != NULL){
      b = colidePlayerProjectile(dynamic_cast<Player*>(s1), dynamic_cast<Projectile*>(s2));

      //Player - Player
	} else if(dynamic_cast<Player*>(s1) != NULL && dynamic_cast<Player*>(s2) != NULL){
      b = colidePlayerPlayer(dynamic_cast<Player*>(s1), dynamic_cast<Player*>(s2));

      //Projectile - Projectile
	} else if (dynamic_cast<Projectile*>(s1) != NULL && dynamic_cast<Projectile*>(s2) != NULL){
      b = colideProjectileProjectile(dynamic_cast<Projectile*>(s1), dynamic_cast<Projectile*>(s2));
	}
  }

  return b;
}

bool GameEngine::colide(Sphere *s, Cuboid *c){
  float pX;
  float pY;
  float pZ;

  if(s->getPosX() > c->getPosX() + (c->getWidth())){
    pX = c->getPosX() + (c->getWidth());
  } else if(s->getPosX() < c->getPosX() - (c->getWidth())){
    pX = c->getPosX() - (c->getWidth());
  } else {
    pX = s->getPosX();
  }

  if(s->getPosY() > c->getPosY() + (c->getHeight())){
    pY = c->getPosY() + (c->getHeight());
  } else if(s->getPosY() < c->getPosY() - (c->getHeight())){
    pY = c->getPosY() - (c->getHeight());
  } else {
    pY = s->getPosY();
  }

  if(s->getPosZ() > c->getPosZ() + (c->getLength())){
    pZ = c->getPosZ() + (c->getLength());
  } else if(s->getPosZ() < c->getPosZ() - (c->getLength())){
    pZ = c->getPosZ() - (c->getLength());
  } else {
    pZ = s->getPosZ();
  }

  float d = sqrt(pow(s->getPosX() - pX, 2) + pow(s->getPosY() - pY, 2) + pow(s->getPosZ() - pZ, 2));

  bool b = false;

  if(d < s->getRealDiameter()){
	//Player - Projectile
	if(dynamic_cast<Projectile*>(s) != NULL && dynamic_cast<Obstacle*>(c) != NULL){
      b = colideProjectileObstacle(dynamic_cast<Projectile*>(s), dynamic_cast<Obstacle*>(c));

      //Player - Player
	} else if(dynamic_cast<Player*>(s) != NULL && dynamic_cast<Obstacle*>(c) != NULL){
      b = colidePlayerObstacle(dynamic_cast<Player*>(s), dynamic_cast<Obstacle*>(c));
	}
  }

  return b;
}

void GameEngine::manageColisions(){
  QList<GameObject*> staticObjects;
  QList<GameObject*> dynamicObjects;

  //Dynamic
  foreach(Player * p, server->getPlayers()){
    dynamicObjects << p;

	foreach(Projectile * pr, p->projectiles){
      dynamicObjects << pr;
	}
  }

  //Static
  foreach(Obstacle *o, this->field.obstacles){
    staticObjects << o;
  }

  //Static - Dynamic
  foreach(GameObject* o1, staticObjects){
    foreach(GameObject* o2, dynamicObjects){
      if(o1 != o2 && colide(o1, o2)){
        //qDebug() << "COLIDEEEEEEEEEEE ";
      }
    }
  }

  int i;
  int j;
  //Dynamic - Dynamic
  for(i = 0; i < dynamicObjects.length(); ++i){
    for(j = i + 1; j < dynamicObjects.length(); ++j){
      if(colide(dynamicObjects[i], dynamicObjects[j])){
        //qDebug() << "COLIDEEEEEEEEEEE ";
      }
    }
  }

}

bool GameEngine::colideObject(GameObject* ob){
  QList<GameObject*> objects;

  //On construit une liste avec tous les objets du jeu
  foreach(Player * p, server->getPlayers()){
	objects << p;

	foreach(Projectile * pr, p->projectiles){
      objects << pr;
	}
  }

  foreach(Obstacle *o, this->field.obstacles){
	objects << o;
  }

  foreach(GameObject* o1, objects){
	if(o1 != ob && colide(o1, ob)){
      qDebug() << "COLIDEEEEEEEEEEE ";
      return true;
	}
  }

  return false;
}

bool GameEngine::colidePlayerProjectile(Player *pl, Projectile *pr){
  if(pr->owner != pl){
    pl->setPosX(pl->getOldPosX());
    pl->setPosY(pl->getOldPosY());
    pl->setPosZ(pl->getOldPosZ());

	pr->owner->hitPlayer(*pl, pr->power);

	pr->explode();

	return true;
  } else {
	return false;
  }
}

bool GameEngine::colidePlayerPlayer(Player *p1, Player *p2){
  p1->setPosX(p1->getOldPosX());
  p1->setPosY(p1->getOldPosY());
  p1->setPosZ(p1->getOldPosZ());

  p2->setPosX(p2->getOldPosX());
  p2->setPosY(p2->getOldPosY());
  p2->setPosZ(p2->getOldPosZ());

  return true;
}

bool GameEngine::colideProjectileProjectile(Projectile *p1, Projectile *p2){
  //if(p1->owner != p2->owner){
	p1->explode();
	p2->explode();

	return true;
  //} else {
    //return false;
  //}
}

bool GameEngine::colideProjectileObstacle(Projectile *p, Obstacle *o){
  o->takeHit(p->power);

  p->explode();

  return true;
}

bool GameEngine::colidePlayerObstacle(Player *p1, Obstacle *){
  p1->setPosX(p1->getOldPosX());
  p1->setPosY(p1->getOldPosY());
  p1->setPosZ(p1->getOldPosZ());

  return true;
}










