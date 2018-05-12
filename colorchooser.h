#ifndef COLORCHOOSER_H
#define COLORCHOOSER_H

#include <QLineEdit>
#include <QProxyStyle>
#include <QWidget>
#include <qdesktopwidget.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qstackedwidget.h>
#include <qdebug.h>
#include <qpainter.h>

namespace Ui {
class ColorChooser;
}

class ColorCircle;
class CustomSlider : public QSlider
{
    Q_OBJECT
   public:

    QString minLabel;
    QString maxLabel;

    void setMinAndMaxLabels(QString min, QString max){
        maxLabel = max;
        minLabel = min;
    }

    void setMaxLabel(QString string){
        maxLabel = string;
    }

    void setMinLabel(QString string){
        minLabel = string;
    }

    CustomSlider(QWidget *parent = Q_NULLPTR){
        QSlider::QSlider(parent);
    }
    CustomSlider(Qt::Orientation orientation, QWidget *parent = Q_NULLPTR){
        QSlider::QSlider(parent);
        QSlider::setOrientation(orientation);
    }

protected:
    void paintEvent(QPaintEvent *event){
        QSlider::paintEvent(event);

        QPainter painter(this);
        QRect rect = geometry();
      //  this->setFont();

        if(orientation() == Qt::Horizontal){
            int x = 5;
            int z = (rect.width()-maxLabel.length()*4) -13;
            qreal y = rect.height() * .75;
            QColor color(250,250,250);
            QPen pen(color);
            painter.setPen(pen);
            painter.drawText(QPoint(x,y),minLabel);
            painter.drawText(QPoint(z,y),maxLabel);


        }else{

        }

    }

};


class ColorChooser : public QWidget
{
    Q_OBJECT

public:
    explicit ColorChooser(QWidget *parent = 0);
    ~ColorChooser();

private slots:
    void changeBackgroundColorOfDisplayWidget();
    void showColorChooser();
    void configureDisplay();

private:
    Ui::ColorChooser *ui;
     QColor colorHSL, colorRGB, colorHEX;

    void setConnections();
    void setColorBackground();
    void setStyleForApplication();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void enterEvent(QEvent *);
    void paintEvent(QPaintEvent *event);

    private slots:
    void setSliders(QColor color);
    void setValueInColor(int v);

private:
    QDesktopWidget *desktop;
    QPixmap pixmap;
    QRgb rgb;
    QColor color;
    ColorCircle* circlebg;
    int r,g,b,a,h,s,v,l, x, y;

    QGroupBox* groupBox;
    CustomSlider *redSlider, *greenSlider, *blueSlider, *hueSlider, *saturationSlider, *valueSlider, *adjustSlider;
    QStackedWidget *stackHolder;
    QPushButton *cancel, *select, *picker, *rgbBtn, *hsv, *hex;
    QWidget *colorDisplay, *rgbHolder, *hsvHolder, *hexHolder, *buttonHolder, *mainHolder;
    QLineEdit *hexEdit;
    QLabel *hLabel, *sLabel, *vLabel, *rLabel, *gLabel, *bLabel;
    QLabel *hValueLabel, *sValueLabel, *vValueLabel, *rValueLabel, *gValueLabel, *bValueLabel;
   // CustomSlider *sliderOne;
};


class CustomStyle : public QProxyStyle
{

public:

    using QProxyStyle::QProxyStyle;

    int styleHint(QStyle::StyleHint hint,

                  const QStyleOption* option = 0,

                  const QWidget* widget = 0,

                  QStyleHintReturn* returnData = 0) const

    {

        if (hint == QStyle::SH_Slider_AbsoluteSetButtons) {

            return (Qt::LeftButton | Qt::MidButton | Qt::RightButton);

        }

        return QProxyStyle::styleHint(hint, option, widget, returnData);

    }

};


#endif // COLORCHOOSER_H
