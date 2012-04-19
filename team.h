#ifndef TEAM_H
#define TEAM_H

#include <QObject>

#include "player.h"

class Team : public QObject
{
    Q_OBJECT
public:
    explicit Team(QObject *parent = 0);

  int getPoints();
  int getCTFPoints();
  void addTeamPoints(int teamPoints);
  void removeTeamPoints(int teamPoints);

  void addPlayer(Player* p);
  void removePlayer(Player* p);

signals:

public slots:

private:
  int teamPoints;
  int cftPoints;

  QList<Player*> players;


};

#endif // TEAM_H
