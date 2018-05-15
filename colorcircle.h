#ifndef COLORCIRCLE_H
#define COLORCIRCLE_H

#include <QWidget>

class ColorCircle : public QWidget
{
    Q_OBJECT
public:
    explicit ColorCircle(QWidget *parent = nullptr);
    void drawSmallCircle(QColor color);
    void updateColor(int h, int s);
    void setValueInColor(QColor color);

signals:
    void positionChanged(QColor color);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QImage *image;
    int radius, v;
    qreal s;
    QPoint centerPoint;
    QColor color;
    QPoint pos;
    void drawCircleColorBackground();

    QColor getCurrentColorFromPosition();

public slots:
};

#endif // COLORCIRCLE_H
