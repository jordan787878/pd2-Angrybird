#ifndef PIG_H
#define PIG_H
#define Pig_Density 2000.0f

#include "enemyitem.h"

class Pig : public EnemyItem
{
public:
   Pig(b2World * g_world,QPointF position,
              int32 Item_Width,int32 Item_Height);

   virtual void move();

   int removeaid = 1;


};

#endif // PIG_H
