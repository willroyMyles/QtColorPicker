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
    setAttribute(Qt::WA_TranslucentBackground);
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
    color = color.fromRgb(255,255,255);
    setSliders(color);

}

ColorChooser::~ColorChooser()
{
    delete ui;
}

void ColorChooser::changeBackgroundColorOfDisplayWidgetHsv()
{

    color.setHsv(hSpin->value(),sSpin->value(), vSpin->value());

    circlebg->drawSmallCircle(color);
    setValueInColor();
    setRgbSliders(color);


}
void ColorChooser::changeBackgroundColorOfDisplayWidgetRgb()
{
    color.setRgb(rSpin->value(),gSpin->value(), bSpin->value());
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
    QHBoxLayout *rSliderSpin = new QHBoxLayout();
    QHBoxLayout *gSliderSpin = new QHBoxLayout();
    QHBoxLayout *bSliderSpin = new QHBoxLayout();
    QHBoxLayout *hSliderSpin = new QHBoxLayout();
    QHBoxLayout *sSliderSpin = new QHBoxLayout();
    QHBoxLayout *vSliderSpin = new QHBoxLayout();
    groupBox->setLayout(vLayout);

    rgbLayout->setSpacing(0);
    hsvLayout->setSpacing(0);

    colorDisplay = new QWidget();
    rgbHolder = new QWidget();
    rgbHolder->setLayout(rgbLayout);

    hsvHolder= new QWidget();
    hsvHolder->setLayout(hsvLayout);

    hexHolder= new QWidget();
    hexEdit = new QLineEdit();
    hexHolder->setLayout(hexLayout);
    hexLayout->addWidget(hexEdit);

    cancel = new QPushButton("cancel");
    select = new QPushButton("Select");
    picker = new QPushButton("pick");
    picker->setCheckable(true);
    picker->setChecked(false);
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
    //adjustSlider->setInvertedAppearance(true);
    connect(valueSlider,&CustomSlider::valueChanged,[=](){
        adjustSlider->setValue(-valueSlider->value());
    });
    connect(adjustSlider,&CustomSlider::valueChanged,[=](){
        valueSlider->setValue(-adjustSlider->value());
    });

    rSpin = new QSpinBox();
    rSpin->setRange(0,255);
    gSpin = new QSpinBox();
    gSpin->setRange(0,255);
    bSpin = new QSpinBox();
    bSpin->setRange(0,255);
    hSpin = new QSpinBox();
    hSpin->setRange(0,360);
    sSpin = new QSpinBox();
    sSpin->setRange(0,255);
    vSpin = new QSpinBox();
    vSpin->setRange(0,255);

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
    adjustSlider->setRange(-255,0);
    hueSlider->setMaximum(360);

    colorLayout->addSpacing(30);
    colorLayout->addWidget(colorDisplay);
    colorLayout->addWidget(adjustSlider);
    colorLayout->addSpacing(50);

    rSliderSpin->addWidget(redSlider);
    rSliderSpin->addWidget(rSpin);
    gSliderSpin->addWidget(greenSlider);
    gSliderSpin->addWidget(gSpin);
    bSliderSpin->addWidget(blueSlider);
    bSliderSpin->addWidget(bSpin);
    hSliderSpin->addWidget(hueSlider);
    hSliderSpin->addWidget(hSpin);
    sSliderSpin->addWidget(saturationSlider);
    sSliderSpin->addWidget(sSpin);
    vSliderSpin->addWidget(valueSlider);
    vSliderSpin->addWidget(vSpin);

    rgbLayout->addLayout(rSliderSpin);
    rgbLayout->addLayout(gSliderSpin);
    rgbLayout->addLayout(bSliderSpin);
    hsvLayout->addLayout(hSliderSpin);
    hsvLayout->addLayout(sSliderSpin);
    hsvLayout->addLayout(vSliderSpin);
    //    rgbLayout->addWidget(redSlider);
    //    rgbLayout->addWidget(greenSlider);
    //    rgbLayout->addWidget(blueSlider);
    //    hsvLayout->addWidget(hueSlider);
    //    hsvLayout->addWidget(saturationSlider);
    //    hsvLayout->addWidget(valueSlider);


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
    connect(cancel, &QPushButton::pressed, [this]() {
        destroy(this);
        exit(0);
    });
    connect(select, &QPushButton::pressed, [this]() {
        destroy(this);
        exit(0);
    });

    connect(picker, SIGNAL(toggled(bool)),this,SLOT(pickerMode(bool)));

    connect(hueSlider, &CustomSlider::sliderPressed,[=](){
        hSpin->setValue(hueSlider->value());
    });
    connect(saturationSlider, &CustomSlider::sliderPressed,[=](){
        sSpin->setValue(saturationSlider->value());
    });
    connect(valueSlider, &CustomSlider::sliderPressed,[=](){
        vSpin->setValue(valueSlider->value());
    });
    connect(adjustSlider, &CustomSlider::sliderPressed,[=](){
        vSpin->setValue(valueSlider->value());
    });
    connect(redSlider, &CustomSlider::sliderPressed,[=](){
        rSpin->setValue(redSlider->value());
    });
    connect(greenSlider, &CustomSlider::sliderPressed,[=](){
        gSpin->setValue(greenSlider->value());
    });
    connect(blueSlider,&CustomSlider::sliderPressed,[=](){
        bSpin->setValue(blueSlider->value());
    });


    connect(hueSlider, &CustomSlider::sliderMoved,[=](){
        hSpin->setValue(hueSlider->value());
    });
    connect(saturationSlider, &CustomSlider::sliderMoved,[=](){
        sSpin->setValue(saturationSlider->value());
    });
    connect(valueSlider, &CustomSlider::sliderMoved,[=](){
        vSpin->setValue(valueSlider->value());
    });
    connect(adjustSlider, &CustomSlider::sliderMoved,[=](){
        vSpin->setValue(valueSlider->value());
    });
    connect(redSlider, &CustomSlider::sliderMoved,[=](){
        rSpin->setValue(redSlider->value());
    });
    connect(greenSlider, &CustomSlider::sliderMoved,[=](){
        gSpin->setValue(greenSlider->value());
    });
    connect(blueSlider,&CustomSlider::sliderMoved,[=](){
        bSpin->setValue(blueSlider->value());
    });



    connect(circlebg, SIGNAL(positionChanged(QColor)), this,SLOT(setSliders(QColor)));

    connect(rSpin, SIGNAL(valueChanged(int)),this, SLOT(changeBackgroundColorOfDisplayWidgetRgb()));
    connect(gSpin, SIGNAL(valueChanged(int)),this, SLOT(changeBackgroundColorOfDisplayWidgetRgb()));
    connect(bSpin, SIGNAL(valueChanged(int)),this, SLOT(changeBackgroundColorOfDisplayWidgetRgb()));
    connect(hSpin, SIGNAL(valueChanged(int)),this, SLOT(changeBackgroundColorOfDisplayWidgetHsv()));
    connect(sSpin, SIGNAL(valueChanged(int)),this, SLOT(changeBackgroundColorOfDisplayWidgetHsv()));
    connect(vSpin, SIGNAL(valueChanged(int)),this, SLOT(changeBackgroundColorOfDisplayWidgetHsv()));
    //        connect(redSlider, SIGNAL(valueChanged(int)),rSpin,SLOT(setValue(int)));
    //        connect(greenSlider, SIGNAL(valueChanged(int)),gSpin,SLOT(setValue(int)));
    //        connect(blueSlider, SIGNAL(valueChanged(int)),bSpin,SLOT(setValue(int)));
    //        connect(hueSlider, SIGNAL(valueChanged(int)),hSpin,SLOT(setValue(int)));
    //        connect(saturationSlider, SIGNAL(valueChanged(int)),sSpin,SLOT(setValue(int)));
    //        connect(valueSlider, SIGNAL(valueChanged(int)),vSpin,SLOT(setValue(int)));

    connect(hexEdit,SIGNAL(returnPressed()),this,SLOT(setColorFromHex()));

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
                   "QSlider::groove:vertical{background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 white, stop: 1 black); border-radius: 5px;}"
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
    setGeometry(desktop->width()/2 - gWidth/2,desktop->height()/2 - gHeight/2,gWidth,gHeight);
    setMouseTracking(false);
    repaint();
    picker->setChecked(false);
}




void ColorChooser::enterPickerMode()
{
    setGeometry(0,0,desktop->width(),desktop->height());
    groupBox->setGeometry(x,y,gWidth,gHeight);
    hide();
    pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    show();
    setMouseTracking(true);
    // picker->setChecked(true);

}

void ColorChooser::pickerMode(bool ye)
{
    if(ye){
        enterPickerMode();
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
    blockSignals(true);
    setRgbSliders(color);
    setHsvSliders(color);
    blockSignals(false);
}

void ColorChooser::setRgbSliders(QColor color)
{

    redSlider->setSliderPosition(color.red());
    greenSlider->setSliderPosition(color.green());
    blueSlider->setSliderPosition(color.blue());
    rSpin->blockSignals(true);
    rSpin->setValue(color.red());
    rSpin->blockSignals(false);
    gSpin->blockSignals(true);
    gSpin->setValue(color.green());
    gSpin->blockSignals(false);
    bSpin->blockSignals(true);
    bSpin->setValue(color.blue());
    bSpin->blockSignals(false);
    this->color = color;
    setSliderLabels();
}

void ColorChooser::setHsvSliders(QColor color)
{

    hueSlider->setSliderPosition(color.hsvHue());
    saturationSlider->setSliderPosition(color.hsvSaturation());
    valueSlider->setSliderPosition(color.value());
    hSpin->blockSignals(true);
    hSpin->setValue(color.hsvHue());
    hSpin->blockSignals(false);
    sSpin->blockSignals(true);
    sSpin->setValue(color.hsvSaturation());
    sSpin->blockSignals(false);
    vSpin->blockSignals(true);
    vSpin->setValue(color.value());
    vSpin->blockSignals(false);
    this->color = color;
    setSliderLabels();

}

void ColorChooser::setColorFromHex()
{
    color.setNamedColor(hexEdit->text());
    setSliders(color);
    setValueInColor();
    circlebg->drawSmallCircle(color);

}

void ColorChooser::setSliderLabels()
{
    circlebg->drawSmallCircle(color);
    hexEdit->setText(color.name());
}


void ColorChooser::setValueInColor()
{

    circlebg->setValueInColor(color);
}


