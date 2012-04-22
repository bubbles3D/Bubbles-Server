#ifndef FLAG_H
#define FLAG_H

#include "cuboid.h"
#include "player.h"
#include "team.h"

#define FLAG_WIDTH 50
#define FLAG_HEIGHT 50
#define FLAG_LENGTH 50

class Team;

class Flag : public Cuboid
{
  Q_OBJECT

public:
    Flag(float initialPosX, float initialPosY, float initialPosZ, int initialCube, Team * team, QObject *parent = 0);

    bool hasBeenMoved();

    void respawn();

    Player * getOwner();
    void setOwner(Player * p);

    Team * getTeam();

private:
    bool moved;

    float initialPosX;
    float initialPosY;
    float initialPosZ;
    int initialCube;

    Player * owner;

    Team * team;
};

#endif // FLAG_H
