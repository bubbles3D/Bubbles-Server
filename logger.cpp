#include "logger.h"

int Logger::logLvl = LOG_LEVEL;

Logger::Logger()
{
}

void Logger::log(QString log, logType typeLog){
    if((Logger::logLvl & typeLog) == 1){
	qDebug() << log;
    }
}
