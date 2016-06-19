#ifndef SCORE_H
#define SCORE_H
#include <QGraphicsTextItem>
#include <QObject>
#include <QTimer>


class Score: public QGraphicsTextItem{
    Q_OBJECT
public:
    Score(bool SumOrNot, float x, float y, int scorenumber, int displaytime = 0);
    int hitscore = 0;
    int SumScore;
    float xp;
    float yp;
    QTimer * timer_move;
    QTimer * timer_end_display;
    float ScaleAid;
    float ScaleStep;

    void setSumScore(int increscore);


public slots:
    void move();
    void endshowhit();
};

#endif // SCORE_H
