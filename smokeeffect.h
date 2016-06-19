#ifndef SMOKEEFFECT_H
#define SMOKEEFFECT_H
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QObject>
#include <QTimer>

class SmokeEffect : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    SmokeEffect(bool IsBigSmoke,QPixmap EffectPix,int displaytime,float x, float y,
                float Max,float ScAid,float ScStep,float OpaAid,float OpaStep);

    QTimer * timer_scale;
    QTimer * timer_move;
    QTimer * timer_end;


    float MaxScale;
    float ScaleAid;
    float OpacityAid;
    float ScaleStep;
    float OpacityStep;
    bool isBig;

public slots:
    void endshow();
    void scalesmoke();
    void movesmoke();

};

#endif // SMOKEEFFECT_H
