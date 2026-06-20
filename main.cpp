#include <QApplication>
#include "director_work.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Director_work w;
    w.show();
    return a.exec();
}
