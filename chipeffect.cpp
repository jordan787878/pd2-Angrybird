#include "chipeffect.h"
#include "game.h"

extern Game * game;

ChipEffect::ChipEffect(QPixmap EffectPic, float x, float y, int ChipsNum,int range)
{
    ExplodeCenter = QPointF(x,y);
    ExplodeRange = range;
    ExplodeStep = 1;
    OpacityAid = 80;
    n = ChipsNum;

    //setpix map
    chips = new QGraphicsPixmapItem[n];

    for(int i = 0;i<n; i++)
    {
        chips[i].setPixmap(EffectPic);
        chips[i].setPos(x,y);
        game->scene->addItem(&chips[i]);
    }

    timer_explode = new QTimer(this);
    connect(timer_explode,SIGNAL(timeout()),this,SLOT(move()));
    timer_explode->start(12.5);

}

void ChipEffect::move()
{
    for(int i = 0; i<n; i++)
    {
        chips[i].setPos(chips[i].x() + ExplodeStep*cos(360/n*i*3.14159/180),
                         chips[i].y() + ExplodeStep*sin(360/n*i*3.14159/180));
        chips[i].setOpacity(OpacityAid/80);
    }

    OpacityAid--;


    QLineF ExplodeDistance(chips[0].pos(),ExplodeCenter);

    if(ExplodeDistance.length() >= ExplodeRange)
    {
       timer_explode->disconnect(this);
       delete timer_explode;
       delete []chips;
       delete this;
       return;
    }


}
