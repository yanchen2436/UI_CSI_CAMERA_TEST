#include "camera_gpio.h"
#include <chrono>
#include <thread>
#include <signal.h>
#include <JetsonGPIO.h>
#include "mainwindow.h"

using namespace std;
//static bool end_this_program = false;
static bool flag = false;
camera_gpio::camera_gpio()
{

}

void camera_gpio::gpio_start(){
    int output_pin = 7;
    int input_pin = 18;
    int oldgpio = 1;
    GPIO::setmode(GPIO::BOARD);
    GPIO::setup(output_pin,GPIO::OUT, GPIO::LOW);
    //qDebug() << "outputting" << out_curr << "to pin";
    GPIO::setup(input_pin,GPIO::IN);
    while (1) {

        int in_curr = GPIO::input(input_pin);
        qDebug() << "inputting" << in_curr << "to pin";
        delay(1);
        if(in_curr == 1 && oldgpio == 0){
            MainWindow* mw = new MainWindow();
            mw->BinarzationStack();
        }
//        if(in_curr == 1){
//            MainWindow* mw = new MainWindow();
//            mw->on_addpic_clicked();
//        }
        oldgpio = in_curr;

    }
    cleanup();
//    int curr_value = GPIO::HIGH;
//    qDebug() << "Outputting" << curr_value << "to pin";

//    while(!end_this_program){
//        delay(1);
//        qDebug() << "Outputting" << curr_value << "to pin";
//        qDebug() << output_pin;
//        GPIO::output(output_pin,curr_value);
//        curr_value ^= GPIO::HIGH;
//    }
}

void camera_gpio::delay(int s){
    this_thread::sleep_for(chrono::seconds(s));
}

//void camera_gpio::signalHandler(){
//    qDebug() << end_this_program;
//    end_this_program = true;
//}
void camera_gpio::cleanup(){
    GPIO::cleanup();
}
