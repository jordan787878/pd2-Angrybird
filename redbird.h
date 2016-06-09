#ifndef REDBIRD_H
#define REDBIRD_H
#define RedBird_Density 1200.0f
#define RedBird_Friction 0.5f
#define RedBird_Elastic 0.3f

#include "gameitem.h"

class RedBird : public GameItem
{
public:
   RedBird(b2World * g_world);
   virtual void shoot();
   virtual void Destroy();

};

#endif // REDBIRD_H
