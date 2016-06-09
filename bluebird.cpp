#include "bluebird.h"
#include "game.h"


extern Game * game;

BlueBird::BlueBird(b2World * g_world)
{
   //draw the body
   setPixmap(QPixmap(":/image/bluebird.png"));
   setPos(200,game->size().height()-250);
   game->scene->addItem(this);

   //set gameitem world to game world
   Item_world = g_world;
   Item_groundBody = game->groundBody;

   //createbody
   b2BodyDef bodyDef;
   bodyDef.type = b2_dynamicBody;
   bodyDef.position.Set(200+25,225);
   body = Item_world->CreateBody(&bodyDef);


   b2CircleShape bodyshape;
   bodyshape.m_radius = 25.0f;
   b2FixtureDef fixtureDef;
   fixtureDef.shape = &bodyshape;

   fixtureDef.density = BlueBird_Density;
   fixtureDef.friction = BlueBird_Friction;
   fixtureDef.restitution =BlueBird_Elastic;

   body->CreateFixture(&fixtureDef);
   body->SetAngularDamping(0.5);

   //Initially set to no active
   body->SetActive(false);

   //add userdata
   bodyUserData * myStruct = new bodyUserData();
   myStruct->ID_NAME = QString("Bird");
   body->SetUserData(myStruct);

}

void BlueBird::Destroy()
{
   timer_shoot->disconnect(this);
   delete timer_shoot;

   if(this->blue1)
   {
      blue1->Item_world->DestroyBody(blue1->body);

      //yell!!!
      game->Bird_Red_Yell->play();

      //explode effect
      VisualEffect(QString("ChipEffect_Red"),QPixmap(":/effect/blue_chips.png"),
                   blue1->pos().x(),blue1->pos().y());

      delete blue1;




   }

   if(this->blue2)
   {
       blue2->Item_world->DestroyBody(blue2->body);

       //yell!!!
       game->Bird_Red_Yell->play();

       //explode effect
       VisualEffect(QString("ChipEffect_Red"),QPixmap(":/effect/blue_chips.png"),
                    blue2->pos().x(),blue2->pos().y());

       delete blue2;

   }

   Item_world->DestroyBody(body);
   game->Birditemlist.removeOne(this);

   //yell!!!
   game->Bird_Red_Yell->play();

   //explode effect
   VisualEffect(QString("ChipEffect_Red"),QPixmap(":/effect/blue_chips.png"),
                this->pos().x(),this->pos().y());

   delete this;
}

void BlueBird::shoot()
{
    //test code
    connect(game->timer_next,SIGNAL(timeout()),game,SLOT(NextTurn()));
    game->timer_next->start(25);


    body->SetActive(true);
    body->SetTransform(b2Vec2(game->originalptr.x(),game->size().height()-game->originalptr.y()),0);

    b2Vec2 FireVelocity(( game->pivotptr.x()-game->originalptr.x() ),
                        ( game->originalptr.y()-game->pivotptr.y() ) );
    FireVelocity *= 1.5;
    body->SetLinearVelocity(FireVelocity);


    timer_shoot = new QTimer(this);
    connect(timer_shoot,SIGNAL(timeout()),this,SLOT(move()));
    timer_shoot->start(12.5);

    game->setRenderHint(QPainter::Antialiasing);
    setTransformOriginPoint(25,25);
    setTransformationMode(Qt::SmoothTransformation);
    setisShoot(true);

    this->grabMouse();

    //fall sound
    QTimer::singleShot(300,game->Bird_Falling_Sound,SLOT(play()));
}

void BlueBird::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(getisShoot() == false && game->ItemToAttack == nullptr)
    {
       qDebug() << "first move";
       game->movebird(this);
       return;
    }

    if(getisShoot() == true && SecondShoot == false)
    {
       //add sound
       yell_sound->play();

       blue1 = new BlueBird(game->world);
       blue2 = new BlueBird(game->world);
       blue1->setisShoot(true);
       blue1->SecondShoot = true;
       blue2->setisShoot(true);
       blue2->SecondShoot = true;

       game->setRenderHint(QPainter::Antialiasing);
       blue1->setTransformOriginPoint(25,25);
       blue1->setTransformationMode(Qt::SmoothTransformation);
       blue2->setTransformOriginPoint(25,25);
       blue2->setTransformationMode(Qt::SmoothTransformation);

       blue1->body->SetTransform(b2Vec2(this->body->GetPosition().x-100,this->body->GetPosition().y-100),0);
       blue1->setPos(blue1->body->GetPosition().x-25,
                     game->size().height()-blue1->body->GetPosition().y-25);

       blue2->body->SetTransform(b2Vec2(this->body->GetPosition().x+100,this->body->GetPosition().y+100),0);
       blue2->setPos(blue2->body->GetPosition().x-25,
                     game->size().height()-blue2->body->GetPosition().y-25);


       blue1->body->SetLinearVelocity(this->body->GetLinearVelocity());
       blue2->body->SetLinearVelocity(this->body->GetLinearVelocity());


       qDebug() << "second move";
       SecondShoot = true;
       this->body->SetActive(true);
       blue1->body->SetActive(true);
       blue2->body->SetActive(true);

       connect(timer_shoot,SIGNAL(timeout()),this,SLOT(smallmove()));

    }

}

void BlueBird::smallmove()
{

    blue1->setRotation(-(blue1->body->GetAngle()*180/3.141592));
    blue1->setPos(blue1->body->GetPosition().x-25,
                  game->size().height()-blue1->body->GetPosition().y-25);

    blue2->setRotation(-(blue2->body->GetAngle()*180/3.141592));
    blue2->setPos(blue2->body->GetPosition().x-25,
                  game->size().height()-blue2->body->GetPosition().y-25);

}





