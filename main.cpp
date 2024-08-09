#include "Dialog/mainwindow.h"
#include "DataStructure/MSystemInfo.h"
#include <QApplication>
#include <QSplashScreen>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    //qDebug() << "1";

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("Icon/recognition.png"));
    splash->show();

    //qDebug() << "2";

    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->showMessage(QObject::tr("Setting up the main window..."), topRight, Qt::white);

    //qDebug() << "3";

    splash->showMessage(QObject::tr("Loading data..."), topRight, Qt::white);
    MSystemInfo::Instance()->init();

    //qDebug() << "4";

    splash->showMessage(QObject::tr("Connecting device..."), topRight, Qt::white);
    MSystemInfo::Instance()->open();

    //qDebug() << "5";

    MainWindow w;
    w.show();

    splash->finish(&w);
    delete splash;

    return a.exec();
}
