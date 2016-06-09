#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QObject>
#include "Box2D/Box2D.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QList>
#include <QMouseEvent>
#include <QMediaPlayer>

#include "gameitem.h"
#include "enemyitem.h"
#include "pig.h"
#include "mycontactlist.h"
#include "userdata.h"
#include "score.h"
#include "visualeffect.h"


class Game : public QGraphicsView{
    Q_OBJECT
public:
    Game();
    QGraphicsScene * scene;

    void displayMenu();
    void CreateWorld();


    void ShowWorld();
    void CreateBirdItem();
    void CreateEnemyItem();
    void activateworld();

    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

    void drawPanel(int x, int y, int width, int height, QColor color, double opacity);
    void GameOverClean();
    void GameOver();


    void movebird(GameItem * ItemToMove);
    void destroyPig(b2Body * BodyToDestroy);


    //game world
    b2Vec2 gravity;
    b2World * world;
    b2Body * groundBody;
    QGraphicsPixmapItem * grounditem;
    QGraphicsPixmapItem * pivot;
    QGraphicsPixmapItem * SlingShot;
    QTimer * timer_world;
    MyContactList * ContactList;



    //game data
    QList <GameItem *> Birditemlist;
    QList <EnemyItem *> Enemyitemlist;
    QList <Pig *> PigList;
    Score * ScoreOfGame;


    //sound  & effect
    QMediaPlayer * BGM;
    QMediaPlayer * Aim_sound;
    QMediaPlayer * Wood_colli_sound;
    QMediaPlayer * Wood_destroy_sound;
    QMediaPlayer * Wood_rolling_sound;
    QMediaPlayer * Rock_colli_sound;
    QMediaPlayer * Rock_destroy_sound;
    QMediaPlayer * Rock_rolling_sound;
    QMediaPlayer * pig_delete_SOUND;
    QMediaPlayer * pig_yell_sound;
    QMediaPlayer * Fire_BIRD_SOUND;
    QMediaPlayer * Bird_Falling_Sound;
    QMediaPlayer * Bird_Bouncing_Sound;
    QMediaPlayer * Bird_Red_Yell;


    //pivot point & Aim
    QGraphicsLineItem * LineItem1;
    QGraphicsLineItem * LineItem2;


    //game running aid
    GameItem * ItemToAttack = nullptr;
    QPointF originalptr;
    QPointF pivotptr;
    b2Body * TempBody;

    int ReMoveAid;
    bool isPicked = false;

    QTimer * timer_next;
    int BirdNumber;


public slots:
    void start();
    void over();
    void restart();

    void tick();
    void ReMove();
    void replayBGM();

    bool NextTurn();
    void CreateBirdLater();

};

#endif // GAME_H
