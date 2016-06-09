#ifndef SMOKEEFFECT_H
#define SMOKEEFFECT_H
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QObject>
#include <QTimer>

class SmokeEffect : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    SmokeEffect(QPixmap EffectPix, float x, float y,
                float Max,float ScAid,float ScStep,float OpaAid,float OpaStep);
    QTimer * timer_display;

    float MaxScale;
    float ScaleAid;
    float OpacityAid;
    float ScaleStep;
    float OpacityStep;

public slots:
    void move();

};

#endif // SMOKEEFFECT_H
