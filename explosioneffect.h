#ifndef EXPLOSIONEFFECT_H
#define EXPLOSIONEFFECT_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QPixmap>
#include "Box2D/Box2D.h"

class ExplosionEffect : public QObject {
    Q_OBJECT
public:
    ExplosionEffect(QPixmap pic, int explodetime, float x, float y);


    //memeber data
    QGraphicsPixmapItem * chips;
    b2Body * explo_ground;
    b2World * explo_world;

    b2Body * chipsbody[10];



    QTimer * timer_end;
    QTimer * timer_explo;
    int n;
    float xp;
    float yp;

public slots:
    void endexplo();
    void displayexplo();



};

#endif // EXPLOSIONEFFECT_H
