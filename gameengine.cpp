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

  //qDebug() << "Players updated! Span:" + QString::number(span);

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

	p->oldPosX = p->posX;
	p->oldPosY = p->posY;
	p->oldPosZ = p->posZ;

	switch(p->cube){
	case 1:
      coeffX = p->dirX / (abs(p->dirX) + abs(p->dirZ));
      coeffZ = p->dirZ / (abs(p->dirX) + abs(p->dirZ));
      break;
	case 2:
      coeffZ = p->dirY / (abs(p->dirY) + abs(p->dirX));
      coeffX = - p->dirX / (abs(p->dirY) + abs(p->dirX));
      break;
	case 3:
      coeffZ = p->dirZ / (abs(p->dirZ) + abs(p->dirY));
      coeffX = p->dirY / (abs(p->dirZ) + abs(p->dirY));
      break;
	case 4:
      coeffZ = p->dirY / (abs(p->dirY) + abs(p->dirX));
      coeffX = p->dirX / (abs(p->dirY) + abs(p->dirX));
      break;
	case 5:
      coeffX = p->dirX / (abs(p->dirX) + abs(p->dirZ));
      coeffZ = - p->dirZ / (abs(p->dirX) + abs(p->dirZ));
      break;
	case 6:
      coeffZ = - p->dirZ / (abs(p->dirZ) + abs(p->dirY));
      coeffX = p->dirY / (abs(p->dirZ) + abs(p->dirY));
      break;
	}

    float relativeSpeed = msSinceLastUpdate / 1000.0 * p->speed;

	//Avancer / Reculer
	if(!(p->keyForwards && p->keyBackwards)){
      if(p->keyForwards){
		if(!(p->keyLeft || p->keyRight)){
          XMove += relativeSpeed * coeffX;
          ZMove += relativeSpeed * coeffZ;
		} else {
          XMove += relativeSpeed * coeffX / 2;
          ZMove += relativeSpeed * coeffZ / 2;
		}
		if(!this->modif)
          this->modif = true;
      } else if(p->keyBackwards){
		if(!(p->keyLeft || p->keyRight)){
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
	if(!(p->keyLeft && p->keyRight)){
      if(p->keyLeft){
		if(!(p->keyBackwards || p->keyForwards)){
          XMove += relativeSpeed * coeffZ;
          ZMove -= relativeSpeed * coeffX;
		} else {
          XMove += relativeSpeed * coeffZ / 2;
          ZMove -= relativeSpeed * coeffX / 2;
		}
		if(!this->modif)
          this->modif = true;
      } else if(p->keyRight){
		if(!(p->keyBackwards || p->keyForwards)){
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

	float playerRadius = p->width * RATIO;
	float realMaxField = field.maxX - playerRadius;

	float pos1;
	float pos2;

	float tmp;

	switch(p->cube){
	case 1:
      pos1 = p->posX + XMove;
      pos2 = p->posZ + ZMove;

      if(pos1 > realMaxField){
		p->posX = realMaxField;
		p->posY = playerRadius + pos1 - realMaxField;
		p->cube = 3;
		tmp = p->dirX;
		p->dirX = - p->dirY;
		p->dirY = tmp;
      } else if(pos1 < playerRadius){
		p->posX = playerRadius;
		p->posY = playerRadius + playerRadius - pos1;
		p->cube = 6;
		tmp = p->dirX;
		p->dirX = p->dirY;
		p->dirY = - tmp;
      } else {
		p->posX += XMove;
      }

      if(pos2 > realMaxField){
		p->posZ = realMaxField;
		p->posY = playerRadius + pos2 - realMaxField;
		p->cube = 4;
		tmp = p->dirZ;
        p->dirZ = - p->dirY;
        p->dirY = tmp;
      } else if(pos2 < playerRadius){
		p->posZ = playerRadius;
		p->posY = playerRadius + playerRadius - pos2;
		p->cube = 2;
		tmp = p->dirZ;
		p->dirZ = p->dirY;
		p->dirY = - tmp;
      } else {
		p->posZ += ZMove;
      }

      break;

	case 2:
      pos1 = p->posX - XMove;
      pos2 = p->posY + ZMove;

      if(pos1 > realMaxField){
		p->posX = realMaxField;
		p->posZ = playerRadius + pos1 - realMaxField;
		p->cube = 3;
		tmp = p->dirX;
		p->dirX = - p->dirZ;
		p->dirZ = tmp;
      } else if(pos1 < playerRadius){
		p->posX = playerRadius;
		p->posZ = playerRadius + playerRadius - pos1;
		p->cube = 6;
        qDebug() << "2to6 z: " << p->posX << ":" << p->posZ;
		tmp = p->dirX;
        p->dirX = p->dirZ;
        p->dirZ = - tmp;
      } else {
        p->posX -= XMove;
      }

      if(pos2 > realMaxField){
		p->posY = realMaxField;
		p->posZ = playerRadius + pos2 - realMaxField;
		p->cube = 5;
        tmp = p->dirY;
        p->dirY = - p->dirZ;
        p->dirZ = tmp;
      } else if(pos2 < playerRadius){
		p->posY = playerRadius;
		p->posZ = playerRadius + playerRadius - pos2;
		p->cube = 1;
		tmp = p->dirZ;
		p->dirZ = - p->dirY;
		p->dirY = tmp;
      } else {
		p->posY += ZMove;
      }

      break;

	case 3:
      pos1 = p->posY + XMove;
      pos2 = p->posZ + ZMove;

      if(pos1 > realMaxField){
		p->posY = realMaxField;
		p->posX = realMaxField - (pos1 - realMaxField);
		p->cube = 5;
		tmp = p->dirX;
		p->dirX = - p->dirY;
		p->dirY = tmp;
      } else if(pos1 < playerRadius){
		p->posX = realMaxField - playerRadius - pos1;
		p->posY = playerRadius;
		p->cube = 1;
		tmp = p->dirX;
		p->dirX = p->dirY;
		p->dirY = - tmp;
      } else {
		p->posY += XMove;
      }

      if(pos2 > realMaxField){
		p->posZ = realMaxField;
		p->posX = realMaxField - pos2 + realMaxField;
		p->cube = 4;
		tmp = p->dirX;
		p->dirX = - p->dirZ;
		p->dirZ = tmp;
      } else if(pos2 < playerRadius){
		p->posZ = playerRadius;
		p->posX = realMaxField - playerRadius + pos2;
		p->cube = 2;
		tmp = p->dirX;
		p->dirX = p->dirZ;
		p->dirZ = - tmp;
      } else {
		p->posZ += ZMove;
      }

      break;

	case 4:
      pos1 = p->posX + XMove;
      pos2 = p->posY + ZMove;

      if(pos1 > realMaxField){
		p->posX = realMaxField;
		p->posZ = realMaxField - pos1 + realMaxField;
		p->cube = 3;
		tmp = p->dirX;
		p->dirX = p->dirZ;
		p->dirZ = - tmp;
      } else if(pos1 < playerRadius){
		p->posX = playerRadius;
		p->posZ = realMaxField - playerRadius + pos1;
		p->cube = 6;
		tmp = p->dirX;
		p->dirX = - p->dirZ;
		p->dirZ = tmp;
      } else {
		p->posX += XMove;
      }

      if(pos2 > realMaxField){
		p->posY = realMaxField;
		p->posZ = realMaxField - pos2 + realMaxField;
		p->cube = 5;
		tmp = p->dirZ;
		p->dirZ = - p->dirY;
		p->dirY = tmp;
      } else if(pos2 < playerRadius){
		p->posY = playerRadius;
		p->posZ = realMaxField - playerRadius + pos2;
		p->cube = 1;
		tmp = p->dirZ;
		p->dirZ = p->dirY;
		p->dirY = - tmp;
      } else {
		p->posY += ZMove;
      }

      break;


	case 5:
      pos1 = p->posX + XMove;
      pos2 = p->posZ - ZMove;

      if(pos1 > realMaxField){
		p->posX = realMaxField;
		p->posY = realMaxField - (pos1 - realMaxField);
		p->cube = 3;
		tmp = p->dirX;
		p->dirX = p->dirY;
		p->dirY = - tmp;
      } else if(pos1 < playerRadius){
		p->posX = playerRadius;
		p->posY = realMaxField - (playerRadius - pos1);
		p->cube = 6;
		tmp = p->dirX;
		p->dirX = - p->dirY;
		p->dirY = tmp;
      } else {
		p->posX += XMove;
      }

      if(pos2 > realMaxField){
		p->posZ = realMaxField;
		p->posY = realMaxField - pos2 + realMaxField;
		p->cube = 4;
		tmp = p->dirZ;
		p->dirZ = p->dirY;
		p->dirY = - tmp;
      } else if(pos2 < playerRadius){
		p->posZ = playerRadius;
		p->posY = realMaxField - playerRadius + pos2;
		p->cube = 2;
		tmp = p->dirZ;
		p->dirZ = p->dirY;
		p->dirY = - tmp;
      } else {
		p->posZ -= ZMove;
      }

      break;

	case 6:
      pos1 = p->posY + XMove;
      pos2 = p->posZ - ZMove;

      if(pos1 > realMaxField){
		p->posY = realMaxField;
		p->posX = playerRadius + pos1 - realMaxField;
		p->cube = 5;
		tmp = p->dirX;
		p->dirX = p->dirY;
		p->dirY = - tmp;
      } else if(pos1 < playerRadius){
		p->posX = playerRadius - pos1 + playerRadius;
		p->posY = playerRadius;
		p->cube = 1;
		tmp = p->dirX;
		p->dirX = - p->dirY;
		p->dirY = tmp;
      } else {
		p->posY += XMove;
      }

      if(pos2 > realMaxField){
		p->posZ = realMaxField;
		p->posX = playerRadius + pos2 - realMaxField;
		p->cube = 4;
		tmp = p->dirX;
		p->dirX = p->dirZ;
		p->dirZ = - tmp;
      } else if(pos2 < playerRadius){
		p->posZ = playerRadius;
		p->posX = playerRadius + playerRadius - pos2;
        qDebug() << "6to2 x: " << p->posX << ":" << p->posZ;;
		p->cube = 2;
        tmp = p->dirZ;
        p->dirZ = p->dirX;
        p->dirX = - tmp;
      } else {
        p->posZ -= ZMove;
      }

      break;
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
}

void GameEngine::updateProjectiles(int msSinceLastUpdate){

  float coeffX;
  float coeffY;
  float coeffZ;

  float sumCoeff;

  float relativeSpeed;

  foreach(Player *pl, server->getPlayers()){
    foreach(Projectile *p, pl->projectiles){
      p->oldPosX = p->posX;
      p->oldPosY = p->posY;
      p->oldPosZ = p->posZ;

      sumCoeff = abs(p->dirX) + abs(p->dirY) + abs(p->dirZ);
      coeffX = p->dirX / sumCoeff;
      coeffY = p->dirY / sumCoeff;
      coeffZ = p->dirZ / sumCoeff;

      relativeSpeed = msSinceLastUpdate / 1000.0 * p->speed;

      p->posX += relativeSpeed * coeffX;
      p->posY += relativeSpeed * coeffY;
      p->posZ += relativeSpeed * coeffZ;

      if(p->posX < 0 || p->posZ < 0 || p->posX > field.maxX ||p->posZ > field.maxZ ){
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
  float d = sqrt(pow(s1->posX - s2->posX, 2) + pow(s1->posY - s2->posY, 2) + pow(s1->posZ - s2->posZ, 2));

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

  if(s->posX > c->posX + (c->width)){
	pX = c->posX + (c->width);
  } else if(s->posX < c->posX - (c->width)){
	pX = c->posX - (c->width);
  } else {
	pX = s->posX;
  }

  if(s->posY > c->posY + (c->height)){
	pY = c->posY + (c->height);
  } else if(s->posY < c->posY - (c->height)){
	pY = c->posY - (c->height);
  } else {
	pY = s->posY;
  }

  if(s->posZ > c->posZ + (c->length)){
	pZ = c->posZ + (c->length);
  } else if(s->posZ < c->posZ - (c->length)){
	pZ = c->posZ - (c->length);
  } else {
	pZ = s->posZ;
  }

  float d = sqrt(pow(s->posX - pX, 2) + pow(s->posY - pY, 2) + pow(s->posZ - pZ, 2));

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
  QList<GameObject*> objects;

  //On construit une liste avec tous les objets du jeu
  foreach(Player * p, server->getPlayers()){
	objects << p;

	foreach(Projectile * pr, p->projectiles){
      objects << pr;
	}
  }

  QList<GameObject*> remainingObjects;
  remainingObjects.append(objects);

  foreach(Obstacle *o, this->field.obstacles){
	objects << o;
  }

  foreach(GameObject* o1, objects){
	foreach(GameObject* o2, remainingObjects){
      if(o1 != o2 && colide(o1, o2)){
		//qDebug() << "COLIDEEEEEEEEEEE ";
      }
	}
  }

  //remainingObjects.removeOne(objects.at(i));

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
	pl->posX = pl->oldPosX;
	pl->posY = pl->oldPosY;
	pl->posZ = pl->oldPosZ;

	pr->owner->hitPlayer(*pl, pr->power);

	pr->explode();

	return true;
  } else {
	return false;
  }
}

bool GameEngine::colidePlayerPlayer(Player *p1, Player *p2){
  p1->posX = p1->oldPosX;
  p1->posY = p1->oldPosY;
  p1->posZ = p1->oldPosZ;

  p2->posX = p2->oldPosX;
  p2->posY = p2->oldPosY;
  p2->posZ = p2->oldPosZ;

  return true;
}

bool GameEngine::colideProjectileProjectile(Projectile *p1, Projectile *p2){
  if(p1->owner != p2->owner){
	p1->explode();
	p2->explode();

	return true;
  } else {
	return false;
  }
}

bool GameEngine::colideProjectileObstacle(Projectile *p, Obstacle *o){
  o->takeHit(p->power);

  p->explode();

  return true;
}

bool GameEngine::colidePlayerObstacle(Player *p1, Obstacle *){
  p1->posX = p1->oldPosX;
  p1->posY = p1->oldPosY;
  p1->posZ = p1->oldPosZ;

  return true;
}










