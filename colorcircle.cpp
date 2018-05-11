#include "colorcircle.h"

#include <QPainter>
#include <qvector2d.h>
#include <qmath.h>
#include <QDebug>
#include <QMouseEvent>

ColorCircle::ColorCircle(QWidget *parent) : QWidget(parent)
{
    setGeometry(4,4,parent->width()-4,parent->height()-4);
    radius = width()/2-4;
    centerPoint.setX(radius+4);
    centerPoint.setY(radius+4);
    v=255;
    drawCircleColorBackground();
    repaint();

}

void ColorCircle::drawSmallCircle(QColor color)
{
   qreal theta =qDegreesToRadians((qreal)color.hsvHue()+90);
   auto sat = color.hsvSaturation()/255.0f * radius;
   qreal x = sat* qCos(theta) + centerPoint.x();
   qreal y = sat* qSin(theta) + centerPoint.y();
    QPoint vector(x,y);
    pos = QPoint(x,y);
    repaint();
}

void ColorCircle::setValueInColor(int v)
{
    this->v = v;
    drawCircleColorBackground();
    repaint();
}

void ColorCircle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter.drawImage(0,0,*image);

QColor color1(240,240,240,255);

    QPen pen(color1,2);
    painter.setPen(pen);
    painter.drawEllipse(4,4,width()-8, height()-8);

    color1.setRgb(0,0,0,250);
    pen.setColor(color1);
    painter.setPen(pen);

    painter.drawEllipse(QPoint(pos.x(), pos.y()), 2,2);

}

void ColorCircle::mouseMoveEvent(QMouseEvent *event)
{
    //QPoint mousePoint = mapFromGlobal(QCursor::pos());



    if(isPressed){
        isDragging = true;
    }

//    if(inCircle(mousePoint)){
//        pos = mousePoint;
//        repaint();
//        emit positionChanged( getCurrentColorFromPosition() );

//    }

 //   if(outsideCirle(mousePoint)){
    QPoint mousePoint = event->pos();
        QVector2D mousePos(mousePoint.x(), mousePoint.y());
        auto centerPos = QVector2D(centerPoint.x(), centerPoint.y());
        auto diff = mousePos - centerPos;
        if (diff.length() > radius) {
            diff = diff.normalized() * radius;
        }

        auto position = centerPos + diff;
        pos.setX(position.x());
        pos.setY(position.y());
        qDebug() << "outside";
        repaint();
        emit positionChanged( getCurrentColorFromPosition() );
 //   }
}

void ColorCircle::mousePressEvent(QMouseEvent *event)
{

//    isPressed = true;

//    QPoint pp = mapFromGlobal(QCursor::pos());
//    if(inCircle(pp) && isPressed){
//        pos = pp;
//        repaint();
//        emit positionChanged( getCurrentColorFromPosition() );
//    }

    QPoint mousePoint = event->pos();
        QVector2D mousePos(mousePoint.x(), mousePoint.y());
        auto centerPos = QVector2D(centerPoint.x(), centerPoint.y());
        auto diff = mousePos - centerPos;
        if (diff.length() > radius) {
            diff = diff.normalized() * radius;
        }

        auto position = centerPos + diff;
        pos.setX(position.x());
        pos.setY(position.y());
        repaint();
        emit positionChanged( getCurrentColorFromPosition() );

}

void ColorCircle::mouseReleaseEvent(QMouseEvent *event)
{
    isPressed = false;
    isDragging = false;
}



void ColorCircle::drawCircleColorBackground()
{

    image = new QImage(width(),height(), QImage::Format_ARGB32_Premultiplied);
    color.setRgb(50,0,0);
    color.setAlpha(0);
    for(int i=0; i < width(); i++){
        for(int j=0; j < width(); j++){

            QPoint point(i,j);
               int d = qPow(point.rx()-centerPoint.rx(), 2) + qPow(point.ry() - centerPoint.ry(), 2);
               if(d <= qPow(radius,2)) {

                   s = (qSqrt(d)/radius)*255.0f;
                   qreal theta = qAtan2(point.ry()-centerPoint.ry(), point.rx()-centerPoint.rx());

                   theta = (180 +90 + (int)qRadiansToDegrees(theta))%360;
                   //qDebug() << s << ' '<<theta;
                   color.setHsv(theta,s,v,255);
                   image->setPixelColor(i,j,color);
               }else{
                   color.setRgb(240,240,240,0);
                   image->setPixelColor(i,j,color);
               }
        }
    }
}

bool ColorCircle::inCircle(QPoint point)
{


       int d = qPow(point.rx()-centerPoint.rx(), 2) + qPow(point.ry() - centerPoint.ry(), 2);
       if(d <= qPow(radius,2)) {

            return true;
       }

            return false;
}

bool ColorCircle::outsideCirle(QPoint point)
{

       int d = qPow(point.rx()-centerPoint.rx(), 2) + qPow(point.ry() - centerPoint.ry(), 2);
       if(d > qPow(radius,2)) {
            return true;
       }
       return false;
}

QColor ColorCircle::getCurrentColorFromPosition()
{
    int d = qPow(pos.rx()-centerPoint.rx(), 2) + qPow(pos.ry() - centerPoint.ry(), 2);


        s = (qSqrt(d)/radius)*255.0f;
        qreal theta = qAtan2(pos.ry()-centerPoint.ry(), pos.rx()-centerPoint.rx());
        theta = (180 +90 + (int)qRadiansToDegrees(theta))%360;
        if(s > 255)
            s=255;
        color.setHsv(theta,s,v,255);
        qDebug() << theta << " " << s << " " << v;
        return color;


}
