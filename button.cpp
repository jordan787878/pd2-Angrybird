#include "button.h"
#include <QBrush>
#include <QDebug>
#include "game.h"

extern Game * game;


Button::Button(QString name, QGraphicsItem *parent) :QGraphicsRectItem(parent)
{
  setRect(0,0,200,50);
  QBrush brush;
  brush.setStyle(Qt::SolidPattern);
  brush.setColor(Qt::yellow);
  setBrush(brush);

  text = new QGraphicsTextItem(name,this);
  int xPos = rect().width()/2- text->boundingRect().width()/2;
  int yPos = rect().height()/2-text->boundingRect().height()/2;
  text->setPos(xPos,yPos);

  //allow responding to mouse hover event
  setAcceptHoverEvents(true);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
   emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
   //change color to cyan
   QBrush brush;
   brush.setStyle(Qt::SolidPattern);
   brush.setColor(Qt::red);
   setBrush(brush);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::yellow);
    setBrush(brush);
}
