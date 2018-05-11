#include "circlebackground.h"

#include <qpainter.h>
#include <qdesktopwidget.h>
#include <QtMath>
#include <QColor>
#include <QVector2D>
#include <qdebug.h>

void CircleBackground::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
   // painter.drawEllipse(0,0, width(), height());

    QPoint centerPoint(width()/2,height()/2);
    int radius = width()/2;
    int h,s;

    QColor color(255,0,0,255);
    color = color.toHsl();

    float radSqrd = radius * radius;

    for(int i=0; i < width(); i++){
        for(int j=0; j < width(); j++){
            /*
            QVector2D point(i,j);
            QVector2D diff = point - centerPoint;
            if (diff.lengthSquared() < radSqrd) {
                s = (diff.lengthSquared() / radSqrd) * 255;
                qreal theta = qAtan2(diff.y(), diff.x());
                theta = 180 + qRadiansToDegrees(theta);

                //qDebug()<<s;
                //color.setHsl(theta, s,200);
                color.setHsv(theta, s,255);


                painter.setPen(color);
                painter.drawPoint(QPoint(i,j));
            }
            */

            QPoint point(i,j);
            int d = qPow(point.rx()-centerPoint.rx(), 2) + qPow(point.ry() - centerPoint.ry(), 2);
            if(d <= qPow(radius,2)) {

                s = (qSqrt(d)/radius)*255;
                qreal theta = qAtan2(point.ry()-centerPoint.ry(), point.rx()-centerPoint.rx());

                theta = (180 +90 + (int)qRadiansToDegrees(theta))%360;
                //qDebug() << s << ' '<<theta;
                color.setHsv(theta,s,255,255);
                painter.setPen(color);
                painter.drawPoint(point);

            }


        }
    }

}

CircleBackground::CircleBackground(QWidget *parent)
    : QWidget(parent)
{
   // QDesktopWidget *desktop = new QDesktopWidget();
    setGeometry(0,0,parent->width(),parent->height());

    repaint();
}

CircleBackground::~CircleBackground()
{

}
