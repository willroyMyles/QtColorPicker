#ifndef CIRCLEBACKGROUND_H
#define CIRCLEBACKGROUND_H

#include <QWidget>

class CircleBackground : public QWidget
{
    Q_OBJECT

protected:
        void paintEvent(QPaintEvent *event) override;

public:
    CircleBackground(QWidget *parent);
    ~CircleBackground();
};

#endif // CIRCLEBACKGROUND_H
