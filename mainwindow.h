#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/objdetect.hpp>
#include <opencv4/opencv2/imgproc/types_c.h>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/imgproc/types_c.h>
#include <QTextStream>
#include <stdio.h>
#include <QButtonGroup>
#include <QDebug>
#include <QComboBox>
#include <JetsonGPIO.h>
#include <QThread>
using namespace cv;
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void BinarzationStack();

private slots:
    void camera_select();
    //打開摄像头
    void on_cam_start_clicked();
    //关闭摄像头
    void on_cam_stop_clicked();
    //获取摄像头界面
    void importFrame();
    //設置曝光值
    void on_set_metime_value_clicked();
    //曝光模式选择
    void select_expmode();
    //图像方向设置
    void image_direction();
    //帧率
    void camera_frame_rate();
    //重构label
    void rest_cam_view();
    //手动模拟增益
    void on_dmeagain_valueChanged(const QString &arg1);
    //手动数字增益
    void on_dmedgain_valueChanged(const QString &arg1);

    void on_saveparam_clicked();
//    void on_pushButton_clicked();

//CAMERA1
    //获取摄像头界面
    void importFrame_1();

    //保存图片
    void saveImage();
    void saveImage_1();

    void on_syntesis_img_clicked();
    //抓取图片
    void on_addpic_clicked();

private:
    Ui::MainWindow *ui;
    cv::VideoCapture cap;
    cv::VideoCapture cap_1;
    QTimer *timer;
    QTimer *timer_1;
    QTimer *timer_img;
    Mat frame;
    Mat frame_1;
    Mat img;
    QButtonGroup *expmodeGroup;
    QButtonGroup *imagedirectionGroup;
    QButtonGroup *expmodeGroup_1;
    QButtonGroup *imagedirectionGroup_1;
    bool ispic = false;

    void param();

    void render();//参数渲染
    void init_expmode();//初始曝光模式选择
    void init_metime();//初始曝光时间
    void init_medgain(); //初始数字增益参数
    void init_meagain(); //初始模拟增益参数
    void init_image_direction();
    void ban();  //禁止点击
    void noban();

    void render_1();//参数渲染
    void init_expmode_1();//初始曝光模式选择
    void init_metime_1();//初始曝光时间
    void init_medgain_1(); //初始化数字增益参数
    void init_meagain_1(); //初始化模拟增益参数
    void init_image_direction_1();

    void on_cam_start_1_clicked();
    void on_cam_stop_1_clicked();



    //設置曝光值
    void on_set_metime_value_1_clicked();
    //曝光模式选择
    void select_expmode_1();
    //图像方向设置
    void image_direction_1();
    //帧率
    void camera_frame_rate_1();
    //重构label
    void rest_cam_view_1();
    //手动模拟增益
    void on_dmeagain_1_valueChanged(const QString &arg1);
    //手动数字增益
    void on_dmedgain_1_valueChanged(const QString &arg1);
    void on_saveparam_1_clicked();


    void ImageCorrosion(Mat img);
    int OustAlgorithm(Mat img);
    void testgpio();

};
#endif // MAINWINDOW_H
