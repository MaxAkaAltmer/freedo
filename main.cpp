#include <QtWidgets>
#include "mainwindow.h"
#include "emuthread.h"


int main(int argc, char *argv[])
{
 int rv=0;
 QApplication a(argc, argv);
 MainWindow w;

    //приготовления
    w.setWindowTitle("FreeDO v.2.1+");
    w.move((QApplication::desktop()->width()-w.width())>>1,(QApplication::desktop()->height()-w.height())>>1);


    //запуск приложения
    w.show();//FullScreen();
    //rv=a.exec();
    do
    {
        a.processEvents(QEventLoop::AllEvents);
        Sleep(1);
    }while(w.isVisible());

    //чистка

    return rv;
}
