#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QDebug>

#define LOG_LEVEL INIT | REQUEST | DEBUG_1

class Logger
{
public:
    enum logType {
	REQUEST = 1,
	DEBUG_1 = 2,
	DEBUG_2 = 4,
	DEBUG_3 = 8,
	INIT = 16
    };

    static void log(QString log, logType typeLog);



private:
    Logger();

    static int logLvl;
};

#endif // LOGGER_H
