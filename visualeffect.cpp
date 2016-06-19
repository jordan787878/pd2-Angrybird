#include "chipeffect.h"
#include "smokeeffect.h"
#include "game.h"

extern Game * game;


void VisualEffect(QString EffectType, QPixmap Pic, float x, float y)
{


    if(EffectType == QString("ChipEffect_Red"))
    {
        ChipEffect * chipeffect = new ChipEffect(Pic,x,y,10,50);
        return;
    }

    if(EffectType == QString("ChipEffect_White"))
    {
        ChipEffect * chipeffect = new ChipEffect(Pic,x,y,9,50);
        return;
    }

    if(EffectType == QString("ChipEffect_Yellow"))
    {
        ChipEffect * chipeffect = new ChipEffect(Pic,x,y,14,50);
        return;
    }

    if(EffectType == QString("ChipEffect_Blue"))
    {
        ChipEffect * chipeffect = new ChipEffect(Pic,x,y,10,50);
        return;
    }

    if(EffectType == QString("ChipEffect_BBlock"))
    {
        ChipEffect * chipeffect = new ChipEffect(Pic,x,y,9,80);
        return;
    }


    if(EffectType == QString("SmokeEffect_small"))
    {
        SmokeEffect * smokeeffect_small = new SmokeEffect(false,Pic,500,x,y,0.6,0.3,0.0075,1,0.025);
        return;

//        VisualEffect(QString("SmokeEffect_small"),QPixmap(":/effect/smoke.png"),
//                     this->pos().x()+this->boundingRect().width()/2,
//                     this->pos().y()+this->boundingRect().height()/2);
    }

    if(EffectType == QString("SmokeEffect_big"))
    {
        SmokeEffect * smokeeffect_big = new SmokeEffect(true,Pic,500,x,y,1,0.5,0.01,1,0.025);
        return;
    }

    if(EffectType == QString("50000effect"))
    {
        SmokeEffect * Score50000effect = new SmokeEffect(true,Pic,600,x,y,1.5,0.5,0.05,0.75,-0.01);
        return;
    }



}
