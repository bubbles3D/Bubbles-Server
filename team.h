#ifndef TEAM_H
#define TEAM_H

#include <QObject>

#include "player.h"

class Player;

class Team : public QObject
{
    Q_OBJECT
public:
  explicit Team(QString name, int num, int colorRED, int colorGREEN, int colorBLUE, int faceRespawn = 0, QObject *parent = 0);

  int getPoints();
  void addTeamPoints(int teamPoints);
  void removeTeamPoints(int teamPoints);

  void addPlayer(Player* p);
  void removePlayer(Player* p);

  QString getName();
  int getNum();
  int getColorRED();
  int getColorGREEN();
  int getColorBLUE();

  QSet<QString> & getModifiedProperties();

signals:

public slots:

private:
  int teamPoints;

  QString name;

  int num;

  int colorRED;
  int colorGREEN;
  int colorBLUE;

  int faceRespawn;

  QList<Player*> players;

  QSet<QString> modifiedProperties;


};

#endif // TEAM_H
