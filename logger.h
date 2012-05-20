#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QDebug>

class Logger
{
public:
  enum logType {
    INIT,        //Messages d'initialisation du serveur
    REQUEST_R,   //Requettes reçues
    REQUEST_S,   //Requettes envoyées
    GAME_INFO_1,   //Informations sur le jeu pas importantes
    GAME_INFO_2,   //Informations sur le jeu importantes
    DEBUG_1,     //Differents niveaux de debug
    DEBUG_2,
    DEBUG_3,
    WARNING,
    ERROR
  };

  static void log(QString log, logType typeLog);

private:
  static QSet<int> logLvl;

  Logger();

  static void init();
};

#endif // LOGGER_H
