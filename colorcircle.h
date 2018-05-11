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
    void setValueInColor(int v);

signals:
    void positionChanged(QColor color);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QImage *image;
    int radius, s, v;
    QPoint centerPoint;
    QColor color;
    QPoint pos;
    bool isPressed, isDragging;
    void drawCircleColorBackground();
    bool inCircle(QPoint point);
    bool outsideCirle(QPoint point);
    QColor getCurrentColorFromPosition();

public slots:
};

#endif // COLORCIRCLE_H
