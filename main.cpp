#include <QtCore/QCoreApplication>

#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server::initServer(&a, 9001);

    return a.exec();
}
