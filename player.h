#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QtNetwork>

#include "sphere.h"
#include "projectile.h"
#include "team.h"
#include "flag.h"

#define MIN_DIAMETER 0.3
#define MAX_DIAMETER 2.0
#define MAX_LIFE 200
#define START_LIFE 100
#define SPEED 800
#define PLAYER_DIAMETER_RATIO 100

class Server;
class Projectile;
class Team;
class Flag;

class Player : public Sphere
{
  Q_OBJECT
public:
  explicit Player(QObject *parent = 0);
  virtual ~Player();

  QAbstractSocket * socket;
  QIPv6Address ip;
  int port;

  QString getName();
  void setName(QString name);

  bool getKeyForwards();
  void setKeyForwards(bool keyForwards);
  bool getKeyBackwards();
  void setKeyBackwards(bool keyBackwards);
  bool getKeyLeft();
  void setKeyLeft(bool keyLeft);
  bool getKeyRight();
  void setKeyRight(bool keyRight);

  int getSpeed();
  void setSpeed(int speed);

  int getLife();
  void setLife(int life);

  int getDeaths();
  void setDeaths(int deaths);
  int getKills();
  void setKills(int kills);
  void addKill();

  Team * getTeam();
  int getTeamNum();
  void setTeam(Team * team);

  int getFlagId();
  Flag * getFlag();
  void setFlag(Flag * flag);

  int getColorRED();
  int getColorGREEN();
  int getColorBLUE();

  QString request;
  bool pendingRequest;

  bool takeHit(int power = 1);
  void hitPlayer(Player & p, int power = 1);

  void respawn();
  void throwFlag();
  void reset();

  void fire();

  QList<Projectile *> projectiles;

signals:

public slots:

private:

  void setFloorDistance();

  QString name;

  bool keyForwards;
  bool keyBackwards;
  bool keyLeft;
  bool keyRight;

  int speed;

  int life;

  int deaths;
  int kills;

  Team * team;

  Flag * flag;

};

#endif // PLAYER_H
