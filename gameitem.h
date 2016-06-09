#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsItem>
#include <QObject>
#include <QTimer>
#include "Box2D/Box2D.h"
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMediaPlayer>


class GameItem : public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    GameItem();
    virtual void Destroy();
    virtual void shoot();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);


    //world data
    b2World * Item_world;
    b2Body * Item_groundBody;

    //b2 body
    b2Body * body;
    QTimer * timer_shoot;

    //memeber data
    QMediaPlayer * yell_sound;

    bool getisShoot();
    void setisShoot(bool isornot);


public slots:
    void move();
    virtual void smallmove();

private:
    bool isShoot = false;

};

#endif // GAMEITEM_H
