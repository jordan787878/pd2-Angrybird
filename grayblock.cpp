#include "grayblock.h"
#include "game.h"
#include <QDebug>
#include "visualeffect.h"

extern Game * game;

GrayBlock::GrayBlock(b2World *g_world, QPointF position, int32 Item_Width, int32 Item_Height, float32 angle)
{
    //set enemyitem world to game world
    EnemyItem_world = g_world;
    EnemyItem_groundBody = game->groundBody;

    //set sound
    Rock_Break_Sound = new QMediaPlayer(this);
    Rock_Break_Sound->setMedia(QUrl("qrc:/music/rock_damage.mp3"));


    //set width and height
    Width = Item_Width;
    Height = Item_Height;


    //create dynamic body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set( position.x()+Width/2, game->size().height()-position.y()-Height/2 );
    EnemyItem_body = EnemyItem_world->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(Width/2,Height/2);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = GrayBlock_Density;
    fixtureDef.friction = 0.9f;
    fixtureDef.restitution = 0.02f;
    EnemyItem_body->CreateFixture(&fixtureDef);
    EnemyItem_body->SetAngularDamping(4);
    setTransformOriginPoint(Width/2,Height/2);
    EnemyItem_body->SetTransform(b2Vec2(bodyDef.position.x,bodyDef.position.y),angle*3.141592/180);


    //draw the body
    setPixmap(QPixmap(":/image/Rock_Block.png"));
    setPos(position.x(),game->size().height()-EnemyItem_body->GetPosition().y);
    game->scene->addItem(this);


    timer_move = new QTimer(this);
    connect(timer_move,SIGNAL(timeout()),this,SLOT(move()));
    timer_move->start(12.5);

    game->setRenderHint(QPainter::Antialiasing);
    setTransformationMode(Qt::SmoothTransformation);

    //add userdata
    myStruct = new bodyUserData;
    myStruct->ID_NAME = QString("GrayBlock");
    myStruct->MaxImpulse = 400;
    EnemyItem_body->SetUserData(myStruct);

}

void GrayBlock::move()
{
    this->setRotation(-(EnemyItem_body->GetAngle()*180/3.141592));
    this->setPos(EnemyItem_body->GetPosition().x-Width/2,
                 game->size().height()-EnemyItem_body->GetPosition().y-Height/2);

    if(this->EnemyItem_body->GetLinearVelocity().LengthSquared() > 0)
    {
        game->Rock_rolling_sound->setVolume(10*this->EnemyItem_body->GetLinearVelocity().LengthSquared());
        game->Rock_rolling_sound->play();
    }

    if( this->pos().y() >= 800 ||
        this->pos().x() >= 2400||
        this->pos().x() <= 0 )
    {
        timer_move->disconnect(this);
        delete timer_move;

        EnemyItem_world->DestroyBody(EnemyItem_body);

        game->Enemyitemlist.removeOne(this);
        delete this;
    }


    // if MaxImpulse surpass the limit, block break!
    bodyUserData* udStruct = (bodyUserData*)this->EnemyItem_body->GetUserData();

    if(udStruct->MaxImpulse <= 0 )
    {
        //explode effect
        VisualEffect(QString("ChipEffect_BBlock"),QPixmap(":/effect/blackblock_chip.png"),
                     this->pos().x()+this->boundingRect().width()/2,
                     this->pos().y()+this->boundingRect().height()/2);

        game->Rock_destroy_sound->play();
        timer_move->disconnect(this);
        delete timer_move;
        delete udStruct;
        delete myStruct;

        EnemyItem_world->DestroyBody(EnemyItem_body);

        game->Enemyitemlist.removeOne(this);
        delete this;
        return;
    }

    if(udStruct->MaxImpulse < 160)
    {
        if(BreakState == 0) {
        Rock_Break_Sound->play();
        setPixmap(QPixmap(":/image/Rock_Block_break1.png"));
        ++BreakState;
        }
    }

    if(udStruct->MaxImpulse < 100)
    {
        if(BreakState == 1){
        Rock_Break_Sound->play();
        setPixmap(QPixmap(":/image/Rock_Block_break2.png"));
        ++BreakState;
        }
    }

    if(udStruct->MaxImpulse < 50)
    {
        if(BreakState == 1) {
        Rock_Break_Sound->play();
        setPixmap(QPixmap(":/image/Rock_Block_break3.png"));
        ++BreakState;
        }
    }
}
