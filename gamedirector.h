#ifndef GAMEDIRECTOR_H
#define GAMEDIRECTOR_H

#include <QObject>

#include "gameengine.h"
#include "team.h"

#define GAME_TIME 10
#define PAUSE_TIME 10

class GameEngine;

class GameDirector : public QObject
{
  Q_OBJECT
public:
  explicit GameDirector(QObject *parent = 0);

  void startFun();

  GameEngine & getGameEngine();

  float getRemainingGameTime();
  float getRemainingPauseTime();

  int getGameType();
  QString getGameTypeName();

  QList<Team*> getTeams();

  QList<Flag*> getFlags();

signals:

public slots:

private slots:
  void newGame();
  void stopGame();

private:

  GameEngine * g;

  QTimer * gameTimer;
  QTimer * pauseTimer;

  QTime startedTime;

  QList<Team*> teams;

  QList<Flag *> flags;

  int gameType;

  //########Games section############
  void reset();
  void setTeams();

  //-------------DM------------------
  void setDM();

  //-------------TDM------------------
  void setTDM();

  //-------------CTF------------------
  void setCTF();

};

#endif // GAMEDIRECTOR_H
