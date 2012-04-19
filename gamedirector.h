#ifndef GAMEDIRECTOR_H
#define GAMEDIRECTOR_H

#include <QObject>

#include "gameengine.h"

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

};

#endif // GAMEDIRECTOR_H
