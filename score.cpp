#include "score.h"
#include <QFont>
#include <QStyle>
#include "game.h"

extern Game * game;

Score::Score(bool moveornot, float x, float y, int scorenumber)
{

 //if move
 if(moveornot == true) {

    setTransformOriginPoint(this->boundingRect().width()/2,
                            this->boundingRect().height()/2);
    xp = x-40;
    yp = y-40;

    if(scorenumber > hitscore && scorenumber >= 50) {
    hitscore = scorenumber;

    //add to sumscore
    game->ScoreOfGame->setSumScore(hitscore);

    setDefaultTextColor(Qt::black);
    setPlainText(QString::number(hitscore));
    QFont font;
    font.setBold(true);
    font.setPixelSize(50);
    setFont(font);
    this->setPos(xp,yp);
    game->scene->addItem(this);

    }


    timer_move = new QTimer(this);
    connect(timer_move,SIGNAL(timeout()),this,SLOT(move()));
    timer_move->start(12.5);


    timer_end_display = new QTimer(this);
    connect(timer_end_display,SIGNAL(timeout()),this,SLOT(endshowhit()));
    timer_end_display->start(750);

  }

  //if not move
  if(moveornot == false)  {

      xp = x;
      yp = y;
      SumScore = scorenumber;

      setDefaultTextColor(Qt::black);
      setPlainText(QString("Score: ")+QString::number(SumScore));
      QFont font;
      font.setBold(true);
      font.setPixelSize(50);
      setFont(font);
      this->setPos(xp,yp);
      game->scene->addItem(this);

  }

}

void Score::setSumScore(int increscore)
{
    SumScore += increscore;
    setPlainText(QString("Score: ")+QString::number(SumScore));

}

void Score::move()
{
    this->setPos(this->x()-1,this->y()-1);
}

void Score::endshowhit()
{
    delete this;
}
