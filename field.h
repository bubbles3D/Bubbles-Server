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

#define OBJECTS_DENSITY 30 //Nb d'objets dans une surface 1000*1000*1000
#define OBJECTS_MAX_WIDTH 250
#define OBJECTS_MIN_WIDTH 20
#define OBJECTS_MAX_HEIGHT 250
#define OBJECTS_MIN_HEIGHT 20
#define OBJECTS_MAX_DEPTH 250
#define OBJECTS_MIN_DEPTH 20

class Player;

class Field : public QObject
{
    Q_OBJECT
public:
    explicit Field(QObject *parent = 0);

    int maxX;
    int maxY;
    int maxZ;

    void setRespawnPos(Player & p);

    void regenerateMap();

    QList<Obstacle*> obstacles;

signals:

public slots:

private:


};

#endif // FIELD_H
