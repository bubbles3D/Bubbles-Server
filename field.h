#ifndef FIELD_H
#define FIELD_H

#include <QObject>
#include <QPair>
#include <time.h>

#include "player.h"
#include "obstacle.h"

#define MAX_FIELD_X 3000
#define MAX_FIELD_Y 3000
#define MAX_FIELD_Z 3000

#define OBJECTS_DENSITY 0 //Nb d'objets dans une aire de 1000*1000*1000
#define OBJECTS_MAX_WIDTH 450
#define OBJECTS_MIN_WIDTH 40
#define OBJECTS_MAX_HEIGHT 450
#define OBJECTS_MIN_HEIGHT 40
#define OBJECTS_MAX_DEPTH 450
#define OBJECTS_MIN_DEPTH 40

class Player;

class Field : public QObject
{
    Q_OBJECT
public:
    explicit Field(QObject *parent = 0);

    int maxX;
    int maxY;
    int maxZ;

    void setRespawnPos(Player & p, int face = 0);

    void regenerateMap();

    QList<Obstacle*> obstacles;

signals:

public slots:

private:


};

#endif // FIELD_H
