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
    setWindowFlags(Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    desktop = new QDesktopWidget;

    setGeometry(desktop->width()/2 - 150,desktop->height()/2 - 200,300,400);
    x = geometry().x();
    y = geometry().y();
    gWidth = geometry().width();
    gHeight = geometry().height();

    configureDisplay();
    setColorBackground();
    setConnections();
    setStyleForApplication();

}

ColorChooser::~ColorChooser()
{
    delete ui;
}

void ColorChooser::changeBackgroundColorOfDisplayWidgetHsv()
{

    color.setHsv(hueSlider->value(),saturationSlider->value(), adjustSlider->value());
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

void ColorChooser::configureDisplay()
{
    groupBox = new QGroupBox();
    auto mainLayout = new QGridLayout();
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout);

    QVBoxLayout *vLayout = new QVBoxLayout();
    QVBoxLayout *rgbLayout= new QVBoxLayout();
    QVBoxLayout *hsvLayout = new QVBoxLayout();
    QHBoxLayout *colorLayout = new QHBoxLayout();
    QHBoxLayout *hexLayout = new QHBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QHBoxLayout *finalButtonLayout = new QHBoxLayout();
    groupBox->setLayout(vLayout);

    rgbLayout->setSpacing(0);
    hsvLayout->setSpacing(0);

   // setLayout(vLayout);


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
    picker->setCheckable(true);
//    picker->setAutoExclusive(true);
    rgbBtn = new QPushButton("RGB");
    rgbBtn->setCheckable(true);
    rgbBtn->setChecked(true);
    rgbBtn->setAutoExclusive(true);
    hex = new QPushButton("HEX");
    hex->setCheckable(true);
    hex->setAutoExclusive(true);
    hsv = new QPushButton("HSV");
    hsv->setCheckable(true);
    hsv->setAutoExclusive(true);
    buttonLayout->addWidget(rgbBtn);
    buttonLayout->addWidget(hsv);
    buttonLayout->addWidget(hex);
    buttonLayout->addWidget(picker);
    finalButtonLayout->addWidget(cancel);
    finalButtonLayout->addWidget(select);


    stackHolder = new QStackedWidget();

    redSlider = new CustomSlider(Qt::Horizontal);
    greenSlider = new CustomSlider(Qt::Horizontal);
    blueSlider = new CustomSlider(Qt::Horizontal);
    hueSlider = new CustomSlider(Qt::Horizontal);
    saturationSlider = new CustomSlider(Qt::Horizontal);
    valueSlider = new CustomSlider(Qt::Horizontal);
    adjustSlider = new CustomSlider(Qt::Vertical);
    adjustSlider->setInvertedAppearance(true);
    connect(valueSlider,SIGNAL(valueChanged(int)),adjustSlider,SLOT(setValue(int)));
    connect(adjustSlider,SIGNAL(valueChanged(int)),valueSlider,SLOT(setValue(int)));


    redSlider->setMinLabel(QString("R:"));
    greenSlider->setMinLabel(QString("G:"));
    blueSlider->setMinLabel(QString("B:"));
    hueSlider->setMinLabel(QString("H:"));
    saturationSlider->setMinLabel(QString("S:"));
    valueSlider->setMinLabel(QString("V:"));
    redSlider->setMaximum(255);
    greenSlider->setMaximum(255);
    blueSlider->setMaximum(255);
    saturationSlider->setMaximum(255);
    valueSlider->setMaximum(255);
    adjustSlider->setMaximum(255);
    hueSlider->setMaximum(360);

    colorLayout->addSpacing(30);
    colorLayout->addWidget(colorDisplay);
    colorLayout->addWidget(adjustSlider);
    colorLayout->addSpacing(50);

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
    vLayout->addSpacing(15);
    vLayout->addLayout(buttonLayout);
    vLayout->addWidget(stackHolder);
    vLayout->addLayout(finalButtonLayout);
    groupBox->setGeometry(0,0,gWidth,gHeight);
}

void ColorChooser::setConnections()
{
    connect(rgbBtn, &QPushButton::pressed, [this](){
        if(picker->isChecked())
            exitPickerMode();
        stackHolder->setCurrentIndex(0);
    });
    connect(hsv, &QPushButton::pressed, [this](){
        if(picker->isChecked())
            exitPickerMode();
        stackHolder->setCurrentIndex(1);
    });
    connect(hex, &QPushButton::pressed, [this](){
        if(picker->isChecked())
            exitPickerMode();
        stackHolder->setCurrentIndex(2);
    });

    connect(picker, SIGNAL(pressed()),this, SLOT(enterPickerMode()));
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
                   " QSlider::sub-page {background: rgba(0,0,0,1); border-radius: 3px;}"
                   "QSlider::groove:vertical{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 black, stop: 1 white); border-radius: 5px;}"
                   " QSlider::add-page:vertical {background: rgba(0,0,0,0); border-radius: 3px;}"
                   " QSlider::sub-page:vertical {background: rgba(90,90,90,0); border-radius: 3px;}"
                   "QWidget{ background: rgba(50,50,50,1);border: 1px solid rgba(0,0,0,0); padding:0px; }"
                   "QLineEdit {border: 1px solid rgba(0,0,0,.4); color: rgba(255,255,255,.8); }"
                   "QPushButton{ background: rgba(60,60,60,1); color: rgba(255,255,255,.9); padding: 2px; border: 1px solid rgba(0,0,0,.4); border-radius:2px; }"
                   "QPushButton::checked {background: rgba(45,45,45,.9); }");

}

void ColorChooser::ErrorAdjustSliderValues()
{

}

void ColorChooser::exitPickerMode()
{
    picker->setChecked(false);
    setGeometry(desktop->width()/2 - gWidth/2,desktop->height()/2 - gHeight/2,gWidth,gHeight);
    repaint();
}

QString ColorChooser::toHex()
{

    QString s;
    s.append('#');
    s.append(toHex(color.red()));
    s.append(toHex(color.green()));
    s.append(toHex(color.blue()));

    qDebug() << toHex(color.red());
    qDebug() << toHex(color.green());
    qDebug() << toHex(color.blue());
    return s;
}

QString ColorChooser::toHex(int a)
{

    int rem;
    QString s;
    if(a==0){
        s.append("00");
        return s;
    }
    while(a>0){
        rem = a%16;
    if(rem>=10){
        switch(rem){
        case 10:
            s.append('A');
            break;
        case 11:
            s.append('B');
            break;
        case 12:
            s.append('C');
            break;
        case 13:
            s.append('D');
            break;
        case 14:
            s.append('E');
            break;
        case 15:
            s.append('F');
            break;
        }
    }
    a = a/16;
    if(a==0){
        s.append("0");
        return s;
    }
    }

    return s;
}

void ColorChooser::enterPickerMode()
{
    if(!picker->isChecked()){
        picker->setChecked(true);
    qDebug() <<"help";
    setGeometry(0,0,desktop->width(),desktop->height());
    groupBox->setGeometry(x,y,gWidth,gHeight);
    hide();
    pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    show();
    setMouseTracking(true);
    }else{
        exitPickerMode();
    }
}

void ColorChooser::mouseMoveEvent(QMouseEvent *event)
{
    if(picker->isChecked()){
        rgb = pixmap.toImage().pixel(this->mapFromGlobal(QCursor::pos()));
        color = color.fromRgb(rgb);
        setSliders(color);
        setValueInColor();
        circlebg->drawSmallCircle(color);
    }
}

void ColorChooser::mousePressEvent(QMouseEvent *event)
{
    if(picker->isChecked()){
       exitPickerMode();
    }

    QWidget::mousePressEvent(event);
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
    setSliderLabels();
}

void ColorChooser::setHsvSliders(QColor color)
{

    hueSlider->setValue(color.hsvHue());
    saturationSlider->setValue(color.hsvSaturation());
    valueSlider->setValue(color.value());
    this->color = color;
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
    hexEdit->setText(toHex());


}


void ColorChooser::setValueInColor()
{

    circlebg->setValueInColor(color);
}


