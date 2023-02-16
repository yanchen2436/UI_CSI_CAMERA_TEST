#include "mythread.h"
#include <QDebug>
#include "camera_gpio.h"


MyThread::MyThread(QObject *parent) : QThread(parent)
{

}

void MyThread::run()
{
    camera_gpio* use_gpio = new camera_gpio;
    use_gpio->gpio_start();
//    qDebug() << gpio_value;
    qDebug() << "当前线程对象的地址: " << QThread::currentThread();
//    MainWindow* mw = new MainWindow();
//    mw->BinarzationStack();
}
