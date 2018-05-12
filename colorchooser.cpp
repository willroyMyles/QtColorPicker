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
  //  this->setWindowFlags(Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

    desktop = new QDesktopWidget;
 //   setGeometry(0,0,300,300);
    configureDisplay();
    setColorBackground();

    x = ui->groupBox->geometry().x();
    y = ui->groupBox->geometry().y();

       setConnections();
       setStyleForApplication();

}

ColorChooser::~ColorChooser()
{
    delete ui;
}

void ColorChooser::changeBackgroundColorOfDisplayWidget()
{
    color.setHsv(hueSlider->value(),saturationSlider->value(), valueSlider->value());
    circlebg->drawSmallCircle(color);
}

void ColorChooser::showColorChooser()
{
    this->setGeometry(0,0,desktop->width(),desktop->height());
    ui->groupBox->setGeometry(desktop->width()/2 - ui->groupBox->width()/2, desktop->height()/2 - ui->groupBox->height()/2, ui->groupBox->width(), ui->groupBox->height());
    hide();
    this->pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    show();
    setMouseTracking(true);

}

void ColorChooser::configureDisplay()
{
    setGeometry(desktop->width()/2 - 300,desktop->height()/2 - 420,300,420);
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
    hueSlider->setMaximum(359);


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



    connect(ui->picker, SIGNAL(clicked(bool)),this, SLOT(showColorChooser()));
    connect(hueSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBackgroundColorOfDisplayWidget()));
    connect(saturationSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBackgroundColorOfDisplayWidget()));
    connect(valueSlider, SIGNAL(valueChanged(int)), this, SLOT(setValueInColor(int)));
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
    circlebg = new ColorCircle(ui->colorDisplay);

}

void ColorChooser::setStyleForApplication()
{
    setStyle(new CustomStyle(this->style()));
    setStyleSheet( " QSlider::handle:horizontal { height: 10px; border-radius: 1px; margin: 0 4px; background: rgba(100,100,100,0.9);}"
                  // " QSlider::groove { border: 1px solid black; margin: 2px 0px;}"
                   " QSlider::add-page:horizontal {background: rgba(90,90,90,1); border-radius: 3px;}"
                   " QSlider::sub-page:horizonal {background: rgba(0,0,0,1); border-radius: 3px;}");
}

void ColorChooser::mouseMoveEvent(QMouseEvent *event)
{
    if(ui->picker->isChecked()){
        rgb = pixmap.toImage().pixel(this->mapFromGlobal(QCursor::pos()));
        color = color.fromRgb(rgb);
        ui->hueSlider->setValue(color.hsvHue());
        ui->saturationSlider->setValue(color.hsvSaturation());
        ui->valueSlider->setValue(color.value());
    }
}

void ColorChooser::mousePressEvent(QMouseEvent *event)
{
    ui->picker->setChecked(false);
    setGeometry(desktop->width()/2 - ui->groupBox->width()/2, desktop->height()/2 - ui->groupBox->height()/2, ui->groupBox->width(), ui->groupBox->height());
    ui->groupBox->setGeometry(4,4,ui->groupBox->width(), ui->groupBox->height());
    repaint();
}

void ColorChooser::enterEvent(QEvent *)
{

}

void ColorChooser::paintEvent(QPaintEvent *event)
{
    if(ui->picker->isChecked()){
    QPainter painter(this) ;
    painter.drawPixmap(0,0,desktop->width(),desktop->height(),pixmap);
    }
}

void ColorChooser::setSliders(QColor color)
{
    int r,g,b;
    r = color.red();
    g = color.green();
    b = color.blue();
    h = color.hsvHue();
    s = color.hsvSaturation();
    v= color.value();
    l = color.lightness();

    qreal ss = s/255*100;

    ui->hueSlider->setValue(h);
    ui->saturationSlider->setValue(s);
    ui->valueSlider->setValue(v);
    ui->hsl->setText(QString("hsv( %1, %2%, %3%)").arg(h).arg(ss).arg(v));
    ui->rgb->setText(QString("rgb( %1, %2, %3)").arg(r).arg(g).arg(b));

  //  qDebug() << h << " " << s   << " " <<v; ;
}

void ColorChooser::setValueInColor(int v)
{
    circlebg->setValueInColor(v);
}


