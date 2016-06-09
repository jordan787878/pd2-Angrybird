#include "gameitem.h"
#include <QDebug>
#include "game.h"
#include <QTimer>


extern Game * game;

GameItem::GameItem()
{
    game->centerOn(this);

    yell_sound = new QMediaPlayer(this);
}

void GameItem::Destroy()
{
}

void GameItem::shoot()
{
}

void GameItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
     if(event->button() == Qt::LeftButton)
     {

        if(this->getisShoot() == false && game->isPicked == false) // && game->ItemToAttack == nullptr
        {
            qDebug() << "press";
            game->movebird(this);
        }

     }

}



bool GameItem::getisShoot()
{
    return isShoot;
}

void GameItem::setisShoot(bool isornot)
{
    isShoot = isornot;
}

void GameItem::move()
{
    //set view center
    game->centerOn(this);
    game->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setRotation(-(body->GetAngle()*180/3.141592));
    this->setPos(body->GetPosition().x-25,
                 game->size().height()-body->GetPosition().y-25);

    //check if Bird Stop
    b2Vec2 Velocity(body->GetLinearVelocity());
    if( Velocity.LengthSquared() <= 0.5 ||
        this->pos().y() >= game->sceneRect().size().height()||
        this->pos().x() >= game->sceneRect().size().width()||
        this->pos().x() <= 0 )
    {
        this->Destroy();
    }

}

void GameItem::smallmove()
{

}


