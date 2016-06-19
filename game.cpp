#include "game.h"
#include "addbird.h"
#include <stdlib.h>
#include "time.h"
#include <QDebug>

#include "blackblock.h"
#include "grayblock.h"
#include "pig.h"
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


    //connect timer_next
    timer_next = new QTimer(this);

    //connect quitGame()
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));

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
   scene->clear();
   scene->setSceneRect(0,0,2400,700);


   BGM->play();
   connect(BGM,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(replayBGM()));

   BirdNumber = 0;
   GameOverScore = 0;

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
        GameOverClean();
        GameOver("CLEAR~~~");
        return;
    }

    //game over
    if(BirdNumber > 7-1 && PigList.size() != 0)
    {
        GameOverClean();
        GameOver("GameOver");
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

   Pig * pig4 = new Pig(world,QPointF(675+600,500),50,50);
   PigList.append(pig4);

   Pig * pig6 = new Pig(world,QPointF(675+600,100),50,50);
   PigList.append(pig6);

   Pig * pig3 = new Pig(world,QPointF(1500,500),50,50);
   PigList.append(pig3);



   GrayBlock * block9 = new GrayBlock(world,QPointF(1950,350),150,50,0);
   Enemyitemlist.append(block9);

   BlackBlock * block10 = new BlackBlock(world,QPointF(2000,500),50,150,0);
   Enemyitemlist.append(block10);

   Pig * pig2 = new Pig(world,QPointF(2000,300),50,50);
   PigList.append(pig2);

   Pig * pig5 = new Pig(world,QPointF(1100,550),50,50);
   PigList.append(pig5);

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

void Game::QUITSLOT()
{
    std::cout << "Quit Game Signal receive !" << std::endl ;
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
    //set score to display
    GameOverScore = ScoreOfGame->SumScore;

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
    delete ScoreOfGame;

}

void Game::GameOver(QString overstate)
{
    emit quitGame();

    this->centerOn(600,350);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //drawPanel(0,0,1200,700,Qt::black,0.8);
    //drawPanel(320,120,570,360,Qt::white,0.25);

    QGraphicsTextItem * state_Over = new QGraphicsTextItem();
    QFont statefont("comic sans",50);
    state_Over->setPlainText(overstate);
    state_Over->setFont(statefont);
    if(overstate == "CLEAR~~~") { state_Over->setDefaultTextColor(Qt::yellow); }
    state_Over->setPos(450,120);
    scene->addItem(state_Over);

    QGraphicsTextItem * score_Over = new QGraphicsTextItem();
    QFont scorefont("comic sans",50);
    score_Over->setPlainText(QString("Score:")+QString::number(GameOverScore));
    score_Over->setFont(scorefont);
    score_Over->setDefaultTextColor(Qt::yellow);
    int xp = 1200/2 - score_Over->boundingRect().width()/2;
    score_Over->setPos(xp,200);
    scene->addItem(score_Over);

    Button * playagainbtn_Over = new Button("Play Again");
    playagainbtn_Over->setPos(510,300);
    connect(playagainbtn_Over,SIGNAL(clicked()),this,SLOT(restart()));
    scene->addItem(playagainbtn_Over);

    Button * quitbtn_Over = new Button("Exit");
    quitbtn_Over->setPos(510,375);
    connect(quitbtn_Over,SIGNAL(clicked()),this,SLOT(over()));
    scene->addItem(quitbtn_Over);

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

        //smoke effect
        VisualEffect(QString("SmokeEffect_big"),QPixmap(":/effect/smoke.png"),
                     TempBody->GetPosition().x,this->size().height()-TempBody->GetPosition().y);

        //50000 effect  still need to recode
        VisualEffect(QString("50000effect"),QPixmap(":/effect/50000.png"),
                     TempBody->GetPosition().x,this->size().height()-TempBody->GetPosition().y);

        //add score 50000
        ScoreOfGame->setSumScore(500000);



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



