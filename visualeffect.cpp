#include "chipeffect.h"
#include "smokeeffect.h"

void VisualEffect(QString EffectType, QPixmap Pic, float x, float y)
{
    if(EffectType == QString("ChipEffect_Red"))
    {
        ChipEffect * chipeffect = new ChipEffect(Pic,x,y);
        return;
    }

    if(EffectType == QString("ChipEffect_White"))
    {
        ChipEffect * chipeffect = new ChipEffect(Pic,x,y);
        return;
    }

    if(EffectType == QString("ChipEffect_Yellow"))
    {
        ChipEffect * chipeffect = new ChipEffect(Pic,x,y);
        return;
    }

    if(EffectType == QString("ChipEffect_Blue"))
    {
        ChipEffect * chipeffect = new ChipEffect(Pic,x,y);
        return;
    }


    if(EffectType == QString("SmokeEffect_small"))
    {
        SmokeEffect * smokeeffect_small = new SmokeEffect(Pic,x,y,0.4,0.2,0.0075,1,0.005);
        return;
    }

    if(EffectType == QString("SmokeEffect_big"))
    {
        SmokeEffect * smokeeffect_big = new SmokeEffect(Pic,x,y,1,0.5,0.012,1,0.025);
        return;
    }

    if(EffectType == QString("50000effect"))
    {
        SmokeEffect * Score50000effect = new SmokeEffect(Pic,x,y,1.5,1.45,0.001,1,0);
        return;
    }



}
