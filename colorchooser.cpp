#include "colorchooser.h"
#include "ui_colorchooser.h"
#include <qdebug.h>
#include <qpainter.h>
#include <qcursor.h>
#include "colorcircle.h"
#include <qrgb.h>

ColorChooser::ColorChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorChooser)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setColorBackground();
    desktop = new QDesktopWidget;
    x = ui->groupBox->geometry().x();
    y = ui->groupBox->geometry().y();

       setConnections();

}

ColorChooser::~ColorChooser()
{
    delete ui;
}

void ColorChooser::changeBackgroundColorOfDisplayWidget()
{
    color.setHsv(ui->hueSlider->value(),ui->saturationSlider->value(), ui->valueSlider->value());
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

void ColorChooser::setConnections()
{
    connect(ui->picker, SIGNAL(clicked(bool)),this, SLOT(showColorChooser()));
    connect(ui->hueSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBackgroundColorOfDisplayWidget()));
    connect(ui->saturationSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBackgroundColorOfDisplayWidget()));
    connect(ui->valueSlider, SIGNAL(valueChanged(int)), this, SLOT(setValueInColor(int)));
    connect(ui->cancel, &QPushButton::pressed, [this]() {
        destroy(this);
        exit(0);

    });
    connect(ui->select, &QPushButton::pressed, [this]() {
        destroy(this);
        exit(0);
    });

    connect(circlebg, SIGNAL(positionChanged(QColor)), this,SLOT(setSliders(QColor)));

}

void ColorChooser::setColorBackground()
{
    circlebg = new ColorCircle(ui->colorDisplay);

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

    qDebug() << h << " " << s   << " " <<v; ;
}

void ColorChooser::setValueInColor(int v)
{
    circlebg->setValueInColor(v);
}


