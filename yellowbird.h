#ifndef YELLOWBIRD_H
#define YELLOWBIRD_H

#define YellowBird_Density 1600.0f
#define YellowBird_Friction 0.2f
#define YellowBird_Elastic 0.2f

#include "gameitem.h"

class YellowBird : public GameItem
{
public:
   YellowBird(b2World * g_world);
   virtual void shoot();
   virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
   virtual void Destroy();

   bool SecondShoot = false;


};

#endif // YELLOWBIRD_H
