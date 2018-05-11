#ifndef COLORCHOOSER_H
#define COLORCHOOSER_H

#include <QProxyStyle>
#include <QWidget>
#include <qdesktopwidget.h>

namespace Ui {
class ColorChooser;
}
class ColorCircle;
class ColorChooser : public QWidget
{
    Q_OBJECT

public:
    explicit ColorChooser(QWidget *parent = 0);
    ~ColorChooser();

private slots:
    void changeBackgroundColorOfDisplayWidget();
    void showColorChooser();

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
