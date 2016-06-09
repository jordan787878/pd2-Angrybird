#ifndef ENEMYITEM_H
#define ENEMYITEM_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsItem>
#include <QObject>
#include <QTimer>
#include "Box2D/Box2D.h"
#include "userdata.h"
#include <QMediaPlayer>


class EnemyItem : public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    EnemyItem();
    //world data
    b2World * EnemyItem_world;
    b2Body * EnemyItem_groundBody;

    //b2 body
    b2Body * EnemyItem_body;
    int32 Width;
    int32 Height;

    //userdata
    bodyUserData* myStruct;

    //move timer
    QTimer * timer_move;


public slots:
    virtual void move();



};

#endif // ENEMYITEM_H
