#include "colorchooser.h"
#include "ui_colorchooser.h"
#include <qdebug.h>
#include <qpainter.h>
#include <qcursor.h>
#include "colorcircle.h"
#include <qrgb.h>
#include <qlayout.h>
#include <QVBoxLayout>


ColorChooser::ColorChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorChooser)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

    desktop = new QDesktopWidget;
    configureDisplay();
    setColorBackground();

    x = groupBox->geometry().x();
    y = groupBox->geometry().y();

       setConnections();
       setStyleForApplication();

}

ColorChooser::~ColorChooser()
{
    delete ui;
}

void ColorChooser::changeBackgroundColorOfDisplayWidgetHsv()
{
    color.setHsv(hueSlider->value(),saturationSlider->value(), valueSlider->value());
    circlebg->drawSmallCircle(color);
    setValueInColor();
    setRgbSliders(color);


}
void ColorChooser::changeBackgroundColorOfDisplayWidgetRgb()
{

    color.setRgb(redSlider->value(),greenSlider->value(), blueSlider->value());
    circlebg->drawSmallCircle(color);
    setValueInColor();
    setHsvSliders(color);
}

void ColorChooser::showColorChooser()
{
    this->setGeometry(0,0,desktop->width(),desktop->height());
    //ui->groupBox->setGeometry(desktop->width()/2 - ui->groupBox->width()/2, desktop->height()/2 - ui->groupBox->height()/2, ui->groupBox->width(), ui->groupBox->height());
    hide();
    this->pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    show();
    setMouseTracking(true);

}

void ColorChooser::configureDisplay()
{
    setGeometry(desktop->width()/2 - 150,desktop->height()/2 - 210,300,420);
    QVBoxLayout *vLayout = new QVBoxLayout();
    QVBoxLayout *rgbLayout= new QVBoxLayout();
    QVBoxLayout *hsvLayout = new QVBoxLayout();
    QHBoxLayout *colorLayout = new QHBoxLayout();
    QHBoxLayout *hexLayout = new QHBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QHBoxLayout *finalButtonLayout = new QHBoxLayout();

    setLayout(vLayout);

    colorDisplay = new QWidget();
    rgbHolder = new QWidget();
    rgbHolder->setLayout(rgbLayout);

    hsvHolder= new QWidget();
    hsvHolder->setLayout(hsvLayout);

    hexHolder= new QWidget();
    hexEdit = new QLineEdit();
    hexHolder->setLayout(hexLayout);
    hexLayout->addWidget(hexEdit);

  //  buttonHolder= new QWidget();
  //  buttonHolder->setLayout(buttonLayout);

  //  mainHolder = new QWidget();
  //  mainHolder->setLayout(hLayout);

    cancel = new QPushButton("cancel");
    select = new QPushButton("Select");
    picker = new QPushButton("pick");
    rgbBtn = new QPushButton("RGB");
    hex = new QPushButton("HEX");
    hsv = new QPushButton("HSV");
    buttonLayout->addWidget(rgbBtn);
    buttonLayout->addWidget(hsv);
    buttonLayout->addWidget(hex);
    finalButtonLayout->addWidget(cancel);
    finalButtonLayout->addWidget(select);

    groupBox = new QGroupBox();
    stackHolder = new QStackedWidget();

    redSlider = new CustomSlider(Qt::Horizontal);
    greenSlider = new CustomSlider(Qt::Horizontal);
    blueSlider = new CustomSlider(Qt::Horizontal);
    hueSlider = new CustomSlider(Qt::Horizontal);
    saturationSlider = new CustomSlider(Qt::Horizontal);
    valueSlider = new CustomSlider(Qt::Horizontal);
    adjustSlider = new CustomSlider(Qt::Vertical);
    connect(valueSlider,SIGNAL(valueChanged(int)),adjustSlider,SLOT(setValue(int)));
    connect(adjustSlider,SIGNAL(valueChanged(int)),valueSlider,SLOT(setValue(int)));


    redSlider->setMinLabel(QString("R:"));
    greenSlider->setMinLabel(QString("G:"));
    blueSlider->setMinLabel(QString("B:"));
    hueSlider->setMinLabel(QString("H:"));
    saturationSlider->setMinLabel(QString("S:"));
    valueSlider->setMinLabel(QString("V:"));
    redSlider->setMaximum(255);
    greenSlider->setMaximum(256);
    blueSlider->setMaximum(256);
    saturationSlider->setMaximum(256);
    valueSlider->setMaximum(256);
    adjustSlider->setMaximum(256);
    hueSlider->setMaximum(360);


    colorLayout->addWidget(colorDisplay);
    colorLayout->addWidget(adjustSlider);
    rgbLayout->addWidget(redSlider);
    rgbLayout->addWidget(greenSlider);
    rgbLayout->addWidget(blueSlider);
    hsvLayout->addWidget(hueSlider);
    hsvLayout->addWidget(saturationSlider);
    hsvLayout->addWidget(valueSlider);


    stackHolder->addWidget(rgbHolder);
    stackHolder->addWidget(hsvHolder);
    stackHolder->addWidget(hexHolder);


   // vLayout->addWidget(mainHolder);
   // vLayout->addWidget(buttonHolder);
    vLayout->addLayout(colorLayout);
    vLayout->addLayout(buttonLayout);
    vLayout->addWidget(stackHolder);
    vLayout->addLayout(finalButtonLayout);

}

void ColorChooser::setConnections()
{
    connect(rgbBtn, &QPushButton::pressed, [this](){
        stackHolder->setCurrentIndex(0);
    });
    connect(hsv, &QPushButton::pressed, [this](){
        stackHolder->setCurrentIndex(1);
    });
    connect(hex, &QPushButton::pressed, [this](){
        stackHolder->setCurrentIndex(2);
    });

    connect(picker, SIGNAL(clicked(bool)),this, SLOT(showColorChooser()));
    connect(hueSlider, SIGNAL(sliderPressed()), this, SLOT(changeBackgroundColorOfDisplayWidgetHsv()));
    connect(saturationSlider, SIGNAL(sliderPressed()), this, SLOT(changeBackgroundColorOfDisplayWidgetHsv()));
    connect(valueSlider, SIGNAL(sliderPressed()), this, SLOT(changeBackgroundColorOfDisplayWidgetHsv()));
    connect(adjustSlider, SIGNAL(sliderPressed()), this, SLOT(changeBackgroundColorOfDisplayWidgetHsv()));
    connect(redSlider, SIGNAL(sliderPressed()), this, SLOT(changeBackgroundColorOfDisplayWidgetRgb()));
    connect(greenSlider, SIGNAL(sliderPressed()), this, SLOT(changeBackgroundColorOfDisplayWidgetRgb()));
    connect(blueSlider, SIGNAL(sliderPressed()), this, SLOT(changeBackgroundColorOfDisplayWidgetRgb()));
    connect(hueSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeBackgroundColorOfDisplayWidgetHsv()));
    connect(saturationSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeBackgroundColorOfDisplayWidgetHsv()));
    connect(valueSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeBackgroundColorOfDisplayWidgetHsv()));
    connect(adjustSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeBackgroundColorOfDisplayWidgetHsv()));
    connect(redSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeBackgroundColorOfDisplayWidgetRgb()));
    connect(greenSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeBackgroundColorOfDisplayWidgetRgb()));
    connect(blueSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeBackgroundColorOfDisplayWidgetRgb()));
   connect(cancel, &QPushButton::pressed, [this]() {
        destroy(this);
        exit(0);


    });
    connect(select, &QPushButton::pressed, [this]() {
        destroy(this);
        exit(0);
    });

    connect(circlebg, SIGNAL(positionChanged(QColor)), this,SLOT(setSliders(QColor)));

}

void ColorChooser::setColorBackground()
{
    colorDisplay->setGeometry(0,0,132,132);
    circlebg = new ColorCircle(colorDisplay);

}

void ColorChooser::setStyleForApplication()
{
    setStyle(new CustomStyle(this->style()));
    setStyleSheet( " QSlider::handle { height: 0px; width:1px; margin: -2px 0px; background: rgba(250,100,100,0.9);}"
                   " QSlider::groove { border: 1px solid black; margin: 2px 0px;}"
                   " QSlider::add-page {background: rgba(90,90,90,1); border-radius: 3px;}"
                   " QSlider::sub-page {background: rgba(0,0,0,1); border-radius: 3px;}");
}

void ColorChooser::ErrorAdjustSliderValues()
{
   QVector<CustomSlider*> list;
    list.append(redSlider);
    list.append(greenSlider);
    list.append(blueSlider);
  //  list.append(hueSlider);
    list.append(saturationSlider);
    list.append(adjustSlider);

    for(int i=0; i< list.length(); i++){
        auto s = list.at(i);
        if(s->value() <= 2)
            s->setValue(0);
        if(s->value()>= 253){
            s->setValue(255);
            qDebug() << s << " " << s->value();
        }
    }

}

void ColorChooser::mouseMoveEvent(QMouseEvent *event)
{
    if(picker->isChecked()){
        rgb = pixmap.toImage().pixel(this->mapFromGlobal(QCursor::pos()));
        color = color.fromRgb(rgb);
        setSliders(color);
    }
}

void ColorChooser::mousePressEvent(QMouseEvent *event)
{
    picker->setChecked(false);
    setGeometry(desktop->width()/2 - groupBox->width()/2, desktop->height()/2 - groupBox->height()/2, groupBox->width(), groupBox->height());
    groupBox->setGeometry(4,4,groupBox->width(), groupBox->height());
    repaint();
}

void ColorChooser::enterEvent(QEvent *)
{

}

void ColorChooser::paintEvent(QPaintEvent *event)
{
    if(picker->isChecked()){
    QPainter painter(this) ;
    painter.drawPixmap(0,0,desktop->width(),desktop->height(),pixmap);
    }
}

void ColorChooser::setSliders(QColor color)
{
    setRgbSliders(color);
    setHsvSliders(color);

}

void ColorChooser::setRgbSliders(QColor color)
{

    redSlider->setValue(color.red());
    greenSlider->setValue(color.green());
    blueSlider->setValue(color.blue());
    this->color = color;
  //   ErrorAdjustSliderValues();
    setSliderLabels();
}

void ColorChooser::setHsvSliders(QColor color)
{

    hueSlider->setValue(color.hsvHue());
    saturationSlider->setValue(color.hsvSaturation());
    valueSlider->setValue(color.value());
    this->color = color;
 //    ErrorAdjustSliderValues();
    setSliderLabels();

}

void ColorChooser::setSliderLabels()
{
    ErrorAdjustSliderValues();

    hueSlider->setMaxLabel(QString::number(hueSlider->value()));
    saturationSlider->setMaxLabel(QString::number(saturationSlider->value()));
    valueSlider->setMaxLabel(QString::number(valueSlider->value()));
    redSlider->setMaxLabel(QString::number(color.red()));
    greenSlider->setMaxLabel(QString::number(color.green()));
    blueSlider->setMaxLabel(QString::number(color.blue()));

}


void ColorChooser::setValueInColor()
{

    circlebg->setValueInColor(color);
}


