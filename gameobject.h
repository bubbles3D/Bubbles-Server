#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QDebug>

//#define RATIO 4.98725
#define RATIO 2.25

class GameObject : public QObject
{
    Q_OBJECT
public:
    explicit GameObject(QObject *parent = 0);

    int id;

    float posX;
    float posY;
    float posZ;

    float oldPosX;
    float oldPosY;
    float oldPosZ;

    float dirX;
    float dirY;
    float dirZ;

    float width;
    float height;
    float length;

signals:

public slots:

protected:

private:
    static int idCounter;

};

#endif // GAMEOBJECT_H
