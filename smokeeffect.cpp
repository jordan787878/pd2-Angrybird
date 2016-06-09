#include "smokeeffect.h"
#include "game.h"

extern Game * game;

SmokeEffect::SmokeEffect(QPixmap EffectPix, float x, float y,
                         float Max,float ScAid,float ScStep,float OpaAid,float OpaStep)
{
    //set Display
    MaxScale = Max;
    ScaleAid = ScAid;
    ScaleStep = ScStep;
    OpacityAid = OpaAid;
    OpacityStep = OpaStep;



    this->setPixmap(EffectPix);
    this->setTransformOriginPoint(this->boundingRect().width()/2,
                                  this->boundingRect().height()/2);
    this->setScale(ScaleAid);
    setPos(x-this->boundingRect().width()/2,
           y-this->boundingRect().height()/2);

    game->scene->addItem(this);


    timer_display = new QTimer(this);
    connect(timer_display,SIGNAL(timeout()),this,SLOT(move()));
    timer_display->start(12.5);


}

void SmokeEffect::move()
{
    this->setScale(ScaleAid);
    this->setOpacity(OpacityAid);

    if(OpacityStep == 0)
    {
        this->setPos(this->x(),this->y()-2);
    }

    ScaleAid += ScaleStep;
    OpacityAid -= OpacityStep;

    if(ScaleAid > MaxScale)
    {
        timer_display->disconnect(this);
        delete this;
    }

}

