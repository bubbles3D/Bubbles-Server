#include "logger.h"

#define NO_COLOR  "\033[0m" //disable any colors
// regular colors
#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
// emphasized (bolded) colors
#define EBLACK "\033[1;30m"
#define ERED "\033[1;31m"
#define EGREEN "\033[1;32m"
#define EYELLOW "\033[1;33m"
#define EBLUE "\033[1;34m"
#define EMAGENTA "\033[1;35m"
#define ECYAN "\033[1;36m"
#define EWHITE "\033[1;37m"
// underlined colors
#define UBLACK "\033[4;30m"
#define URED "\033[4;31m"
#define UGREEN "\033[4;32m"
#define UYELLOW "\033[4;33m"
#define UBLUE "\033[4;34m"
#define UMAGENTA "\033[4;35m"
#define UCYAN "\033[4;36m"
#define UWHITE "\033[4;37m"
// background colors
#define BBLACK "\033[40m"
#define BRED "\033[41m"
#define BGREEN "\033[42m"
#define BYELLOW "\033[43m"
#define BBLUE "\033[44m"
#define BMAGENTA "\033[45m"
#define BCYAN "\033[46m"
#define BWHITE "\033[47m"


QSet<int> Logger::logLvl;

Logger::Logger()
{
}

void Logger::log(QString log, logType typeLog){
  if(Logger::logLvl.isEmpty()){
    Logger::init();
  }

  if(Logger::logLvl.contains(typeLog)){
    switch(typeLog){
    case WARNING :
      qDebug() << EYELLOW << "WARNING" << NO_COLOR << log;
      break;
    case ERROR :
      qDebug() << ERED << "ERROR" << NO_COLOR << log;
      break;
    case INIT :
      qDebug() << ECYAN << "INIT" << NO_COLOR << log;
      break;
    case REQUEST_R :
      qDebug() << EMAGENTA << "REQUEST_R" << NO_COLOR << log;
      break;
    case REQUEST_S :
      qDebug() << EMAGENTA << "REQUEST_S" << NO_COLOR << log;
      break;
    case DEBUG_1 :
      qDebug() << EBLUE << "DEBUG_1" << NO_COLOR << log;
      break;
    case DEBUG_2 :
      qDebug() << EBLUE << "DEBUG_2" << NO_COLOR << log;
      break;
    case DEBUG_3 :
      qDebug() << EBLUE << "DEBUG_3" << NO_COLOR << log;
      break;
    case GAME_INFO_1 :
      qDebug() << EGREEN << "GAME_INFO_1" << NO_COLOR << log;
      break;
    case GAME_INFO_2 :
      qDebug() << EGREEN << "GAME_INFO_2" << NO_COLOR << log;
      break;
    default:
      qDebug() << log;
    }


  }
}

void Logger::init(){
  Logger::logLvl << INIT << REQUEST_S << GAME_INFO_1 << GAME_INFO_2 << WARNING << ERROR << DEBUG_1;
}
