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

    float getPosX();
    void setPosX(float posX);
    float getPosY();
    void setPosY(float posY);
    float getPosZ();
    void setPosZ(float posZ);

    float getOldPosX();
    void setOldPosX(float oldPosX);
    float getOldPosY();
    void setOldPosY(float oldPosY);
    float getOldPosZ();
    void setOldPosZ(float oldPosZ);

    float getDirX();
    void setDirX(float dirX);
    float getDirY();
    void setDirY(float dirY);
    float getDirZ();
    void setDirZ(float dirZ);

    float getWidth();
    void setWidth(float width);
    float getHeight();
    void setHeight(float height);
    float getLength();
    void setLength(float length);

    int getColorRED();
    void setColorRED(int colorRED);
    int getColorGREEN();
    void setColorGREEN(int colorGREEN);
    int getColorBLUE();
    void setColorBLUE(int colorBLUE);

    QMap<QString, QString> & getModifiedProperties();

signals:

public slots:

protected:
    QMap<QString, QString> modifiedProperties;

private:
    static int idCounter;

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

    int colorRED;
    int colorGREEN;
    int colorBLUE;

};

#endif // GAMEOBJECT_H
