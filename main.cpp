#include "colorchooser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColorChooser w;
    w.show();

    return a.exec();
}
