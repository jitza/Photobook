#include "imagecollection.h"
#include "imagedialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageDialog w;
    w.show();
    return a.exec();
}
