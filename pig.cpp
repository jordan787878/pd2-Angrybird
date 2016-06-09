#include "pig.h"
#include "game.h"
#include <QDebug>


extern Game * game;


Pig::Pig(b2World *g_world, QPointF position, int32 Item_Width, int32 Item_Height)
{

    //set enemyitem world to game world
    EnemyItem_world = g_world;
    EnemyItem_groundBody = game->groundBody;

    //set width and height
    Width = Item_Width;
    Height = Item_Height;


    //create dynamic body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set( position.x()+Width/2, game->size().height()-position.y()-Height/2 );
    EnemyItem_body = EnemyItem_world->CreateBody(&bodyDef);

    b2CircleShape bodyshape;
    bodyshape.m_radius = 25.0f;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &bodyshape;

    fixtureDef.density = Pig_Density;
    fixtureDef.friction = 0.6f;
    fixtureDef.restitution = 0.15f;
    EnemyItem_body->CreateFixture(&fixtureDef);
    EnemyItem_body->SetAngularDamping(1.5);




    //draw the body
    setPixmap(QPixmap(":/image/pig.png"));

    setPos(position.x(),game->size().height()-EnemyItem_body->GetPosition().y);
    game->scene->addItem(this);


    timer_move = new QTimer(this);
    connect(timer_move,SIGNAL(timeout()),this,SLOT(move()));
    timer_move->start(12.5);

    game->setRenderHint(QPainter::Antialiasing);
    setTransformOriginPoint(Width/2,Height/2);
    setTransformationMode(Qt::SmoothTransformation);


    //add userdata
    bodyUserData* myStruct = new bodyUserData;
    myStruct->ID_NAME = QString("Pig");
    myStruct->MaxImpulse = 500;
    EnemyItem_body->SetUserData(myStruct);


}

void Pig::move()
{
    this->setRotation(-(EnemyItem_body->GetAngle()*180/3.141592));
    this->setPos(EnemyItem_body->GetPosition().x-Width/2,
                 game->size().height()-EnemyItem_body->GetPosition().y-Height/2);


    if( this->pos().y() >= 800 ||
        this->pos().x() >= 2400||
        this->pos().x() <= 0 )
    {
        timer_move->disconnect(this);
        delete timer_move;
        //test
        EnemyItem_body->SetLinearVelocity(b2Vec2_zero);

        EnemyItem_world->DestroyBody(EnemyItem_body);

        game->PigList.removeOne(this);
        delete this;
    }


    // if MaxImpulse surpass the limit, block break!
    bodyUserData* PigData = (bodyUserData*)this->EnemyItem_body->GetUserData();
    if(PigData->MaxImpulse <= 0  && removeaid == 1)
    {
        --removeaid;
        game->destroyPig(this->EnemyItem_body);
    }




}









