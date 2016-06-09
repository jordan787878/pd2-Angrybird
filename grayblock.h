#ifndef GRAYBLOCK_H
#define GRAYBLOCK_H
#define GrayBlock_Density 1500.0f

#include "enemyitem.h"

class GrayBlock : public EnemyItem
{
public:
   GrayBlock(b2World * g_world,QPointF position,
              int32 Item_Width,int32 Item_Height,
             float32 angle);

   //member data
   QMediaPlayer * Rock_Break_Sound;
   int BreakState = 0;


public slots:
    virtual void move();


};

#endif // GRAYBLOCK_H
