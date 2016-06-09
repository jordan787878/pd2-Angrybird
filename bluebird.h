#ifndef BLUEBIRD_H
#define BLUEBIRD_H

#define BlueBird_Density 850.0f
#define BlueBird_Friction 0.7f
#define BlueBird_Elastic 0.1f

#include "gameitem.h"

class BlueBird : public GameItem
{
public:
   BlueBird(b2World * g_world);
   virtual void Destroy();
   virtual void shoot();
   virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

   bool SecondShoot = false;
   BlueBird * blue1 = nullptr;
   BlueBird * blue2 = nullptr;

public slots:
   void smallmove();

};

#endif // BLUEBIRD_H
