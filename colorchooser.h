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
#include <QSpinBox>

namespace Ui {
class ColorChooser;
}

class ColorCircle;

class CustomBackground : public QWidget
{
    Q_OBJECT
public:
    CustomBackground(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags()){
        QWidget::QWidget(parent);
        setWindowFlags(Qt::FramelessWindowHint  | Qt::X11BypassWindowManagerHint);
        //setAttribute(Qt::WA_TranslucentBackground);
        desktop = new QDesktopWidget;
        setGeometry(desktop->width()/2 ,desktop->height()/2 ,3,3);
        show();
    }

    void drawPixmap(QPixmap pm){
       setGeometry(0,0,desktop->width(),desktop->height());
       pixmap = &pm;
       image = pixmap->toImage();
       isBig = true;
//       qDebug() << pixmap << "  "  ;
       repaint();
   }

    void shrink(){
        setGeometry(desktop->width()/2 ,desktop->height()/2 ,3,3);
        isBig=false;

        emit finished(isBig);

   }

private:
    QDesktopWidget *desktop;
    QPixmap *pixmap;
    QImage image;
    bool isBig=false;
    QColor color;

protected:
    void paintEvent(QPaintEvent *event){
        if(isBig){
        QPainter painter(this);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
        painter.drawPixmap(0,0,desktop->width(),desktop->height(),*pixmap);
      //  qDebug() << *pixmap << "  "  ;;
        setMouseTracking(true);

        }
    }

    void mousePressEvent(QMouseEvent *event){
        shrink();
    }

    void mouseMoveEvent(QMouseEvent *event)
    {
        if(isBig){

            color = color.fromRgb(image.pixel(this->mapFromGlobal(QCursor::pos())));
//            qDebug() << color;
            emit positionChanged(color);
        }

    }

signals:
    void finished(bool b);
    void positionChanged(QColor color);

};



class CustomSlider;
class CustomBackground;
class ColorChooser : public QWidget
{
    Q_OBJECT

public:
    explicit ColorChooser(QWidget *parent = 0);
    ~ColorChooser();

private slots:
    void changeBackgroundColorOfDisplayWidgetHsv();
    void changeBackgroundColorOfDisplayWidgetRgb();
    void configureDisplay();
    void pickerMode(bool ye);


private:
    Ui::ColorChooser *ui;

    void setConnections();
    void setColorBackground();
    void setStyleForApplication();
    void ErrorAdjustSliderValues();
    void exitPickerMode();
    void enterPickerMode();


protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *);
    void paintEvent(QPaintEvent *event);

    private slots:
    void setSliders(QColor color);
    void setRgbSliders(QColor color);
    void setHsvSliders(QColor color);
    void setColorFromHex();
    void setSliderLabels();
    void setValueInColor();


private:
    QDesktopWidget *desktop;
    QPixmap pixmap;
    CustomBackground *cbg;
    QRgb rgb;
    QColor color;
    ColorCircle* circlebg;
    int x, y, gWidth, gHeight;

    QGroupBox* groupBox;
    CustomSlider *alphaSlider, *redSlider, *greenSlider, *blueSlider, *hueSlider, *saturationSlider, *valueSlider, *adjustSlider;
    QStackedWidget *stackHolder;
    QPushButton *cancel, *select, *picker, *rgbBtn, *hsv, *hex;
    QWidget *colorDisplay, *rgbHolder, *hsvHolder, *hexHolder;
    QLineEdit *hexEdit;
    QLabel *hLabel, *sLabel, *vLabel, *rLabel, *gLabel, *bLabel;
    QLabel *hValueLabel, *sValueLabel, *vValueLabel, *rValueLabel, *gValueLabel, *bValueLabel;
    QSpinBox *rSpin, *gSpin, *bSpin, *hSpin, *sSpin, *vSpin;
    QDoubleSpinBox *alphaSpin;
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
class CustomSlider : public QSlider
{
    Q_OBJECT
   public:

    QString minLabel;
    QString maxLabel;

    void setMinAndMaxLabels(QString min, QString max){
       setMinLabel(min);
       setMaxLabel(max);
    }

    void setMaxLabel(QString string){
        maxLabel = string;
        repaint();
    }

    void setMinLabel(QString string){
        minLabel = string;
    }

    CustomSlider(QWidget *parent = Q_NULLPTR){
        QSlider::QSlider(parent);
        QSlider::setStyle(new CustomStyle(this->style()));


    }
    CustomSlider(Qt::Orientation orientation, QWidget *parent = Q_NULLPTR){
        CustomSlider::CustomSlider(parent);
        QSlider::setOrientation(orientation);

    }

    void adjustValue(){
        if(value() <= 2)
           setValue(0);
        if(value()>= 253 && maximum()==255){
            setValue(255);
         }
        setMaxLabel(QString::number(value()));
           emit QSlider::sliderPressed();
    }

protected:
    void paintEvent(QPaintEvent *event){
        QSlider::paintEvent(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);

        QRect rect = geometry();

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

            int x = width()/2 -1;
            //qreal y = -((rect.height()/255.0f) * value()) + maximum();
            qreal y = sliderPosition();

            QColor color(250,250,250);
            if(value() < 195)
                color.setRgb(50,50,50);
            QPen pen(color);
            pen.setWidth(2);
            painter.setPen(pen);
            painter.drawEllipse(x,y,5,5);

        }

    }

private:
QColor color;
bool isColorSlider;

};


#endif // COLORCHOOSER_H
