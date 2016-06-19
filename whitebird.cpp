#include "whitebird.h"
#include "game.h"
#include <QDebug>

extern Game * game;

WhiteBird::WhiteBird(b2World *g_world)
{  
    //draw the body
    setPixmap(QPixmap(":/image/white_bird.gif"));
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
    body->SetUserData( this );


    b2CircleShape bodyshape;
    bodyshape.m_radius = 25.0f;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &bodyshape;

    fixtureDef.density = WhiteBird_Density;
    fixtureDef.friction = WhiteBird_Friction;
    fixtureDef.restitution = WHiteBird_Elastic;
    body->CreateFixture(&fixtureDef);
    body->SetAngularDamping(1.0);

    //Initially set to no active
    body->SetActive(false);

    //add userdata
    bodyUserData * myStruct = new bodyUserData();
    myStruct->ID_NAME = QString("Bird");
    body->SetUserData(myStruct);

}

void WhiteBird::shoot()
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

    //set focus on this item
    this->grabMouse();


    //all sound
    QTimer::singleShot(300,game->Bird_Falling_Sound,SLOT(play()));

}

void WhiteBird::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(getisShoot() == false && game->ItemToAttack == nullptr)
    {
       game->movebird(this);
    }

    if(getisShoot() == true  && SecondShoot == false)
    {
        //add sound
        yell_sound->play();

        this->body->SetLinearVelocity(b2Vec2(55.0f,55.0f));
        SecondShoot = true;
        return;
    }

    if(SecondShoot == true && ThirdShoot == false)
    {
        //add sound
        yell_sound->play();

        this->body->SetLinearVelocity(b2Vec2(55.0f,55.0f));
        ThirdShoot = true;
        return;
    }


}

void WhiteBird::Destroy()
{
    timer_shoot->disconnect(this);
    delete timer_shoot;
    Item_world->DestroyBody(body);
    game->Birditemlist.removeOne(this);

    //yell!!!
    game->Bird_Red_Yell->play();

    //explode effect
    VisualEffect(QString("ChipEffect_White"),QPixmap(":/effect/white_chips.png"),
                 this->pos().x(),this->pos().y());

    delete this;

}




