#ifndef BUTTOM_H
#define BUTTOM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QObject>

class Button :public QObject,public QGraphicsRectItem {
    Q_OBJECT
public:
    Button(QString name, QGraphicsItem * parent=0);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

signals:
    void clicked();

private:
    QGraphicsTextItem * text;
};


#endif // BUTTOM_H
