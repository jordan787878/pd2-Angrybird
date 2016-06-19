#include "score.h"
#include <QFont>
#include <QStyle>
#include "game.h"

extern Game * game;

Score::Score(bool SumOrNot, float x, float y, int scorenumber, int displaytime)
{

 //if move
 if(SumOrNot == true) {

    xp = x;
    yp = y;

    ScaleAid = 0.25;
    ScaleStep = 0.06;

    if(scorenumber > hitscore && scorenumber >= 50) {
    hitscore = scorenumber;

    //add to sumscore
    game->ScoreOfGame->setSumScore(hitscore);

    //set font style
    setDefaultTextColor(Qt::black);
    setPlainText(QString::number(hitscore));
    QFont font;
    font.setBold(true);
    font.setPixelSize(50);
    setFont(font);


    this->setScale(ScaleAid);
    setTransformOriginPoint(this->boundingRect().width()/2,
                            this->boundingRect().height()/2);
    this->setPos(xp,yp);
    game->scene->addItem(this);

    }


    timer_move = new QTimer(this);
    connect(timer_move,SIGNAL(timeout()),this,SLOT(move()));
    timer_move->start(12.5);


    timer_end_display = new QTimer(this);
    connect(timer_end_display,SIGNAL(timeout()),this,SLOT(endshowhit()));
    timer_end_display->start(displaytime);

  }

  //if not move
  if(SumOrNot == false)  {

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
    if(SumScore >= 50000) { setDefaultTextColor(Qt::yellow); }
    setPlainText(QString("Score: ")+QString::number(SumScore));

}

void Score::move()
{
    this->setScale(ScaleAid);
    ScaleAid += ScaleStep;

    if(ScaleAid >= 1)
    {
        timer_move->disconnect(this);
    }

}

void Score::endshowhit()
{
    delete this;
}
