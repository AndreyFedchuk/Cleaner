#include "cleaner.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Cleaner w;
    w.show();

    return a.exec();
}
