#ifndef CAMERA_GPIO_H
#define CAMERA_GPIO_H
#include <QDebug>

class camera_gpio
{
public:
    camera_gpio();
    void delay(int s);
//    void signalHandler();
    void cleanup();
    void gpio_start();
};

#endif // CAMERA_GPIO_H
