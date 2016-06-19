#ifndef CHIPEFFECT_H
#define CHIPEFFECT_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>

class ChipEffect : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    ChipEffect(QPixmap EffectPic, float x, float y, int ChipsNum, int range);

    //how many chips
    int n;

    QGraphicsPixmapItem * chips;

    //memeber data
    QPointF ExplodeCenter;
    QTimer * timer_explode;

    float ExplodeRange;
    float ExplodeStep;
    float OpacityAid;


public slots:
    void move();


};


#endif // CHIPEFFECT_H
