#include "explosioneffect.h"
#include "game.h"
#include <math.h>

extern Game *game;

ExplosionEffect::ExplosionEffect(QPixmap pic, int explodetime, float x, float y)
{
    xp = x;
    yp = y;
    n = 10;

    //b2world
    explo_world = game->world;
    explo_ground = game->groundBody;

    b2BodyDef chipsDef;
    chipsDef.type = b2_dynamicBody;
    chipsDef.position.Set(x+16/2,700-y-16/2);


    //draw it
    chips = new QGraphicsPixmapItem[n];
    for(int i = 0;i<n;i++) {
      chips[i].setTransformOriginPoint(16/2,16/2);
      chips[i].setPixmap(pic);
      chips[i].setPos(x,y);
      game->scene->addItem(&chips[i]);

      chipsbody[i] = explo_world->CreateBody(&chipsDef);
      chipsbody[i]->SetAngularDamping(0.6f);
      chipsbody[i]->SetLinearDamping(0.5f);


      float vx = 50 * cos(360/n*i*3.14159/180);
      float vy = 50 * sin(360/n*i*3.14159/180);
      chipsbody[i]->SetLinearVelocity(b2Vec2(vx,vy));

    }



    timer_end = new QTimer(this);
    connect(timer_end,SIGNAL(timeout()),this,SLOT(endexplo()));
    timer_end->start(explodetime);

    timer_explo = new QTimer(this);
    connect(timer_explo,SIGNAL(timeout()),this,SLOT(displayexplo()));
    timer_explo->start(12.5);


}

void ExplosionEffect::endexplo()
{
   timer_explo->disconnect(this);
   timer_end->disconnect(this);
   for(int i = 0; i<n; i++) {
       explo_world->DestroyBody(chipsbody[i]);
   }
   delete []chips;
   delete this;
}

void ExplosionEffect::displayexplo()
{
  for(int i = 0;i<n;i++)
  {
      chips[i].setPos(chipsbody[i]->GetPosition().x-16/2,
                        game->size().height()-chipsbody[i]->GetPosition().y-16/2);
      chips[i].setRotation(chipsbody[i]->GetAngle()*180/3.141592);
  }
}
