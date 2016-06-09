#include "game.h"
#include "addbird.h"
#include <stdlib.h>
#include "time.h"
#include <QDebug>

#include "blackblock.h"
#include "grayblock.h"
#include "pig.h"
#include "button.h"
#include <QTime>

#include <QBrush>
#include<QPixmap>

Game::Game()
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,1200,700);

    setScene(scene);
    setFixedSize(1200,700);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //set background
    setBackgroundBrush(QBrush(QPixmap(":/image/back.png")));

    //create B2World
    CreateWorld();

    //show Menu
    displayMenu();
}


void Game::CreateWorld()
{

    //create b2world
    gravity.Set(0.0f,-9.8f);
    world = new b2World(gravity);

    //add contact list
    ContactList = new MyContactList();
    world->SetContactListener(ContactList);

    //set ground (1000x50)
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f,25.0f);
    groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groudBox;
    groudBox.SetAsBox(2400.0f,25.0f);
    groundBody->CreateFixture(&groudBox,1.0f);    //&groudBox,0.0f

     ////add ground userdata
     bodyUserData * myStruct = new bodyUserData();
     myStruct->ID_NAME = QString("Ground");
     groundBody->SetUserData(myStruct);



    //add BGM
    BGM = new QMediaPlayer(this);
    BGM->setMedia(QUrl("qrc:/music/BGM.mp3"));


    //add aim sound
    Aim_sound = new QMediaPlayer(this);
    Aim_sound->setMedia(QUrl("qrc:/music/Aim.mp3"));


    //add wood sound
    Wood_colli_sound = new QMediaPlayer(this);
    Wood_colli_sound->setMedia(QUrl("qrc:/music/wood_colli.mp3"));
    Wood_destroy_sound = new QMediaPlayer(this);
    Wood_destroy_sound->setMedia(QUrl("qrc:/music/wood_destroy.mp3"));
    Wood_rolling_sound = new QMediaPlayer(this);
    Wood_rolling_sound->setMedia(QUrl("qrc:/sound/wood_rolling.mp3"));

    //add rock sound
    Rock_colli_sound = new QMediaPlayer(this);
    Rock_colli_sound->setMedia(QUrl("qrc:/music/rock_colli.mp3"));
    Rock_destroy_sound = new QMediaPlayer(this);
    Rock_destroy_sound->setMedia(QUrl("qrc:/music/rock_destroy.mp3"));
    Rock_rolling_sound = new QMediaPlayer(this);
    Rock_rolling_sound->setMedia(QUrl("qrc:/sound/rock_rolling.mp3"));


    //add pig sound
    pig_delete_SOUND = new QMediaPlayer(this);
    pig_delete_SOUND->setMedia(QUrl("qrc:/music/pig_delete.mp3"));
    pig_yell_sound = new QMediaPlayer(this);
    pig_yell_sound->setMedia(QUrl("qrc:/sound/pig_yell.mp3"));

    //add bird sound
    Fire_BIRD_SOUND = new QMediaPlayer(this);
    Fire_BIRD_SOUND->setMedia(QUrl("qrc:/music/fire.mp3"));
    Bird_Falling_Sound = new QMediaPlayer(this);
    Bird_Falling_Sound->setMedia(QUrl("qrc:/music/falling.mp3"));
    Bird_Bouncing_Sound = new QMediaPlayer(this);
    Bird_Bouncing_Sound->setMedia(QUrl("qrc:/sound/bird_bounce.mp3"));

    Bird_Red_Yell = new QMediaPlayer(this);
    Bird_Red_Yell->setMedia(QUrl("qrc:/sound/redbird_yell.mp3"));


    //test
    timer_next = new QTimer(this);

}


void Game::displayMenu()
{

    QGraphicsTextItem * title = new QGraphicsTextItem(QString("Angry Bird"));
    QFont titlefont("comic sans",50);
    title->setFont(titlefont);
    int txPos = this->width()/2 - title->boundingRect().width()/2;
    title->setPos(txPos,200);
    scene->addItem(title);

    Button * playbtn = new Button("Play");
    int pbxPox = this->width()/2 - playbtn->boundingRect().width()/2;
    playbtn->setPos(pbxPox,320);
    connect(playbtn,SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(playbtn);


    Button * exitbtn = new Button("Exit");
    exitbtn->setPos(pbxPox,400);
    connect(exitbtn,SIGNAL(clicked()),this,SLOT(over()));
    scene->addItem(exitbtn);

}


void Game::start()
{
   //delete Last Time Score
   if(ScoreOfGame)
   {
       delete ScoreOfGame;
   }

   scene->clear();
   scene->setSceneRect(0,0,2400,700);



   BGM->play();
   connect(BGM,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(replayBGM()));

   BirdNumber = 0;

   ShowWorld();
   CreateBirdItem();
   CreateEnemyItem();
   activateworld();

}

void Game::ShowWorld()
{
    //add ground pix
    grounditem = new QGraphicsPixmapItem();
    grounditem->setPixmap(QPixmap(":/image/ground.png"));
    grounditem->setPos(0,this->sceneRect().size().height()-50);
    scene->addItem(grounditem);



    //add pivot to measure the strength, midpoint(125,425)
    pivot = new QGraphicsPixmapItem();
    pivot->setPixmap(QPixmap(":/image/pivot.png"));
    pivot->setPos(215,this->sceneRect().size().height()-235);
    scene->addItem(pivot);
    pivot->setVisible(false);
    pivotptr = QPointF(225,this->sceneRect().size().height()-225);

    //SlingShot
    SlingShot = new QGraphicsPixmapItem();
    SlingShot->setPixmap(QPixmap(":/image/aim.png"));
    SlingShot->setPos(215-40,450);
    scene->addItem(SlingShot);


    //add aiming line
    LineItem1 = new QGraphicsLineItem();
    LineItem2 = new QGraphicsLineItem();
    scene->addItem(LineItem1);
    scene->addItem(LineItem2);

    //add score
    ScoreOfGame = new Score(false,900,40,0);

}


void Game::CreateBirdItem()
{
    //game clear
    if(BirdNumber > 0 && PigList.size() == 0)
    {
        qDebug() << "game clear";
        GameOverClean();
        GameOver();
        return;
    }

    //game over
    if(BirdNumber > 15-1 && PigList.size() != 0)
    {
        qDebug() << "gameover";
        GameOverClean();
        GameOver();
        return;
    }

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    srand(time(NULL));
    GameItem * gameitem = AddBird();
    Birditemlist.append(gameitem);
    ++BirdNumber;
}

void Game::CreateEnemyItem()
{
   BlackBlock * block1 = new BlackBlock(world,QPointF(675+600,350),50,150,90);
   Enemyitemlist.append(block1);


   GrayBlock * block2 = new GrayBlock(world,QPointF(550+600,500),150,50,90);
   Enemyitemlist.append(block2);


   GrayBlock * block4 = new GrayBlock(world,QPointF(700+600,500),150,50,90);
   Enemyitemlist.append(block4);

   GrayBlock * block5 = new GrayBlock(world,QPointF(625+600,200),150,50,0);
   Enemyitemlist.append(block5);


   BlackBlock * block7 = new BlackBlock(world,QPointF(615+600,250),50,150,0);
   Enemyitemlist.append(block7);

   BlackBlock * block8 = new BlackBlock(world,QPointF(735+600,250),50,150,0);
   Enemyitemlist.append(block8);

   Pig * pig1 = new Pig(world,QPointF(675+600,300),50,50);
   PigList.append(pig1);



   GrayBlock * block9 = new GrayBlock(world,QPointF(1950,350),150,50,0);
   Enemyitemlist.append(block9);


   BlackBlock * block10 = new BlackBlock(world,QPointF(2000,500),50,150,0);
   Enemyitemlist.append(block10);

   Pig * pig2 = new Pig(world,QPointF(2000,300),50,50);
   PigList.append(pig2);


}


void Game::activateworld()
{
    timer_world = new QTimer(this);
    connect(timer_world,SIGNAL(timeout()),this,SLOT(tick()));
    timer_world->start(3);
}

bool Game::NextTurn()
{

   float BodyVelocity = 0;

   for(size_t i = 0,n=Birditemlist.size(); i<n; i++)
   {
       if(Birditemlist[i]->body)
       {
           BodyVelocity = Birditemlist[i]->body->GetLinearVelocity().Length();
           if(BodyVelocity > 0.05)
           {
               return false;
           }
       }
   }

   for(size_t i = 0,n = Enemyitemlist.size(); i<n; i++)
   {
       if(Enemyitemlist[i]->EnemyItem_body)
       {
           BodyVelocity = Enemyitemlist[i]->EnemyItem_body->GetLinearVelocity().Length();
           if(BodyVelocity > 0.05)
           {
               return false;
           }
       }
   }

   for(size_t i = 0,n = PigList.size(); i<n; i++)
   {
       if(PigList[i]->EnemyItem_body)
       {
           BodyVelocity = Enemyitemlist[i]->EnemyItem_body->GetLinearVelocity().Length();
           if(BodyVelocity > 0.05)
           {
               return false;
           }
       }
   }

   timer_next->disconnect();
   QTimer::singleShot(800,this,SLOT(CreateBirdLater()));
   return true;
}

void Game::CreateBirdLater()
{
    CreateBirdItem();
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
  if(isPicked)
  {
     this->Aim_sound->play();
     float xp = ItemToAttack->boundingRect().width()/2;
     float yp = ItemToAttack->boundingRect().height()/2;
     ItemToAttack->setPos(event->pos().x()-xp,event->pos().y()-yp);
     originalptr = event->pos();

     QLineF line1(originalptr.x(),originalptr.y(),pivotptr.x()+20,pivotptr.y());
     QLineF line2(originalptr.x(),originalptr.y(),pivotptr.x()-40,pivotptr.y());
     QPen pen;
     pen.setWidth(10);
     pen.setColor(Qt::black);
     LineItem1->setPen(pen);
     LineItem1->setLine(line1);
     LineItem1->setVisible(true);
     LineItem2->setPen(pen);
     LineItem2->setLine(line2);
     LineItem2->setVisible(true);



  }

  QGraphicsView::mouseMoveEvent(event);
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if(ItemToAttack)
    {
       //sound
       Fire_BIRD_SOUND->play();

       isPicked = false;
       QPen pen;
       pen.setStyle(Qt::DotLine);
       pen.setWidth(2.5);
       LineItem1->setPen(pen);
       LineItem2->setPen(pen);
       ItemToAttack->shoot();

       ItemToAttack = nullptr;
    }

   QGraphicsView::mouseReleaseEvent(event);

}

void Game::drawPanel(int x, int y, int width, int height, QColor color, double opacity)
{
    //draw panel with specific loction width x height
    QGraphicsRectItem * panel = new QGraphicsRectItem(x,y,width,height);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    panel->setBrush(brush);
    panel->setOpacity(opacity);
    scene->addItem(panel);
}



void Game::GameOverClean()
{
    //stop BGM
    BGM->disconnect(this);
    BGM->stop();

    for(size_t i=0,n=Birditemlist.size(); i<n; i++)
    {
        delete Birditemlist[i];
    }

    for(size_t i =0,n = Enemyitemlist.size(); i<n ;i++)
    {
        world->DestroyBody(Enemyitemlist[i]->EnemyItem_body);
        delete Enemyitemlist[i];
    }

    for(size_t i =0,n = PigList.size(); i<n; i++)
    {
        world->DestroyBody(PigList[i]->EnemyItem_body);
        delete PigList[i];
    }


    //world data clean
    Birditemlist.clear();
    Enemyitemlist.clear();
    PigList.clear();

    //world clean
    timer_world->disconnect(this);
    delete timer_world;

    delete LineItem1;
    delete LineItem2;
    delete grounditem;
    delete pivot;
    delete SlingShot;

}

void Game::GameOver()
{
    this->centerOn(600,350);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //drawPanel(0,0,1200,700,Qt::black,0.8);
    //drawPanel(320,120,570,360,Qt::white,0.25);


    //show score
    ScoreOfGame->setPos(510,150);

    Button * playagainbtn = new Button("Play Again");
    playagainbtn->setPos(510,275);
    connect(playagainbtn,SIGNAL(clicked()),this,SLOT(restart()));
    scene->addItem(playagainbtn);

    Button * quitbtn = new Button("Exit");
    quitbtn->setPos(510,350);
    connect(quitbtn,SIGNAL(clicked()),this,SLOT(over()));
    scene->addItem(quitbtn);



}




void Game::movebird(GameItem *ItemToMove)
{
    isPicked = true;
    ItemToAttack = ItemToMove;
}

void Game::destroyPig(b2Body *BodyToDestroy)
{
    ReMoveAid = 1;
    TempBody = BodyToDestroy;
    QTimer::singleShot(0,this,SLOT(ReMove()));
    qDebug() << "pre remove";
}





void Game::tick()
{
    world->Step(1.0/60.0,6,2);
}

void Game::ReMove()
{
    ReMoveAid--;

    if(ReMoveAid == 0)
    {
        pig_yell_sound->play();
        qDebug() << "remove";

        //smoke effect
        VisualEffect(QString("SmokeEffect_big"),QPixmap(":/effect/smoke.png"),
                     TempBody->GetPosition().x,this->size().height()-TempBody->GetPosition().y);

        //5000 effect  still need to recode
        VisualEffect(QString("50000effect"),QPixmap(":/effect/50000.png"),
                     TempBody->GetPosition().x,this->size().height()-TempBody->GetPosition().y);



        TempBody->SetTransform(b2Vec2(3000,0),0);
    }

}

void Game::replayBGM()
{
    BGM->play();
}


void Game::over()
{
    delete this;
}

void Game::restart()
{
    this->start();
}



