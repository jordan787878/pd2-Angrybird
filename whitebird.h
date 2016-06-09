#ifndef WHITEBIRD_H
#define WHITEBIRD_H
#define WhiteBird_Density 900.0f
#define WhiteBird_Friction 0.2f
#define WHiteBird_Elastic 0.8f

#include <QGraphicsScene>
#include "gameitem.h"

class WhiteBird : public GameItem
{
public:
   WhiteBird(b2World * g_world);
   virtual void shoot();
   virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
   virtual void Destroy();

   bool SecondShoot = false;
   bool ThirdShoot = false;
};


#endif // WHITEBIRD_H
