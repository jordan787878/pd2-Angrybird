#ifndef BLACKBLOCK_H
#define BLACKBLOCK_H
#define BlackBlock_Density 400.0f

#include "enemyitem.h"


class BlackBlock : public EnemyItem
{
public:
   BlackBlock(b2World * g_world, QPointF position,
              int32 Item_Width, int32 Item_Height, float32 angle);

   //member data
   QMediaPlayer * Wood_Break_Sound;
   int BreakState = 0;

public slots:
    virtual void move();

};

#endif // BLACKBLOCK_H
