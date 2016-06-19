#include "smokeeffect.h"
#include "game.h"


extern Game * game;

SmokeEffect::SmokeEffect(bool IsBigSmoke, QPixmap EffectPix, int displaytime, float x, float y,
                         float Max, float ScAid, float ScStep, float OpaAid, float OpaStep)
{
    //set Display
    isBig = IsBigSmoke;
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


    timer_scale = new QTimer(this);
    connect(timer_scale,SIGNAL(timeout()),this,SLOT(scalesmoke()));
    timer_scale->start(12.5);

    timer_move = new QTimer(this);
    connect(timer_move,SIGNAL(timeout()),this,SLOT(movesmoke()));
    timer_move->start(12.5);


    timer_end = new QTimer(this);
    connect(timer_end,SIGNAL(timeout()),this,SLOT(endshow()));
    timer_end->start(displaytime);


}

void SmokeEffect::endshow()
{
    timer_move->disconnect(this);
    timer_end->disconnect(this);
    delete this;
}

void SmokeEffect::scalesmoke()
{
    this->setScale(ScaleAid);
    ScaleAid += ScaleStep;

    //end scale if reach max
    if(ScaleAid > MaxScale)
    {
        timer_scale->disconnect(this);
    }

}

void SmokeEffect::movesmoke()
{
    if(isBig == false)
    {
        this->setPos(this->pos().x(),this->pos().y()-0.75);
        this->setOpacity(OpacityAid);
        OpacityAid -= OpacityStep;
    }

    if(isBig == true)
    {
        this->setPos(this->pos().x(),this->pos().y()-1);
        this->setOpacity(OpacityAid);
        OpacityAid -= OpacityStep;
    }
}

