#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QButtonGroup>
#include <QDebug>
#include <QMessageBox>
#include <QComboBox>
#include <QStringList>
#include <QTimer>
#include <QTime>
#include "mythread.h"

using namespace cv;
int count = 1;
int count1 = 1;
int count2 = 1;
int count3 = 1;
int flag = 0;
int flagcamera = 0;
int flag1 = 0;
int cnt = 1;
float judgevalu = 240.0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //画面处理
    ui->setupUi(this);
    //摄像头选择
    QStringList cameralist;
    cameralist << "cam 0" << "cam 1";
    ui->comboBox_name->addItems(cameralist);
    connect(ui->comboBox_name,SIGNAL(currentIndexChanged(int)),this,SLOT(camera_select()));
    QStringList strlist;
    strlist<<"1280*1080@max45fps"<<"1080*1280@max45fps"<<"1280*720@max60fps"<<"720*1280@max60fps"<<"640*480@max120fps"<<"480*640@max120fps";
    ui->frame_rate->addItems(strlist);
    param();
    ui->frame_rate->setEnabled(false);
    testgpio();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//参数设置
void MainWindow::param(){
    if(flagcamera == 0){
        //camera0曝光模式选择
        expmodeGroup = new QButtonGroup(this);
        expmodeGroup->addButton(ui->AutoExpmode,0);
        expmodeGroup->addButton(ui->ManualExpmode,1);
        //ui->ManualExpmode->setChecked(true);
        //ui->MetimeValue->setText("1000");
        connect(ui->AutoExpmode,SIGNAL(clicked(bool)),this,SLOT(select_expmode()));
        connect(ui->ManualExpmode,SIGNAL(clicked(bool)),this,SLOT(select_expmode()));
        //camera0图像方向选择
        imagedirectionGroup = new QButtonGroup(this);
        imagedirectionGroup->addButton(ui->image_direction_normal,0);
        imagedirectionGroup->addButton(ui->image_direction_lr,1);
        imagedirectionGroup->addButton(ui->image_direction_ud,2);
        imagedirectionGroup->addButton(ui->image_direction_udlr,3);
        //ui->image_direction_normal->setChecked(true);
        connect(ui->image_direction_normal,SIGNAL(clicked(bool)),this,SLOT(image_direction()));
        connect(ui->image_direction_lr,SIGNAL(clicked(bool)),this,SLOT(image_direction()));
        connect(ui->image_direction_ud,SIGNAL(clicked(bool)),this,SLOT(image_direction()));
        connect(ui->image_direction_udlr,SIGNAL(clicked(bool)),this,SLOT(image_direction()));
        //camera0帧率
        connect(ui->frame_rate,SIGNAL(currentIndexChanged(int)),this,SLOT(camera_frame_rate()));
    //    on_cam_start_clicked();
        init_expmode();
        if(flag == 0){
            ui->frame_rate->setEnabled(false);
        }else{
            ui->frame_rate->setEnabled(true);
        }
    }else{
        //camera1曝光模式选择
        expmodeGroup_1 = new QButtonGroup(this);
        expmodeGroup_1->addButton(ui->AutoExpmode,0);
        expmodeGroup_1->addButton(ui->ManualExpmode,1);
        //ui->ManualExpmode->setChecked(true);
        //ui->MetimeValue_1->setText("1000");
        connect(ui->AutoExpmode,SIGNAL(clicked(bool)),this,SLOT(select_expmode()));
        connect(ui->ManualExpmode,SIGNAL(clicked(bool)),this,SLOT(select_expmode()));
        imagedirectionGroup_1 = new QButtonGroup(this);
        //camera1图像方向选择
        imagedirectionGroup_1->addButton(ui->image_direction_normal,0);
        imagedirectionGroup_1->addButton(ui->image_direction_lr,1);
        imagedirectionGroup_1->addButton(ui->image_direction_ud,2);
        imagedirectionGroup_1->addButton(ui->image_direction_udlr,3);
        //ui->image_direction_normal->setChecked(true);
        connect(ui->image_direction_normal,SIGNAL(clicked(bool)),this,SLOT(image_direction()));
        connect(ui->image_direction_lr,SIGNAL(clicked(bool)),this,SLOT(image_direction()));
        connect(ui->image_direction_ud,SIGNAL(clicked(bool)),this,SLOT(image_direction()));
        connect(ui->image_direction_udlr,SIGNAL(clicked(bool)),this,SLOT(image_direction()));

        //camera1帧率
        connect(ui->frame_rate,SIGNAL(currentIndexChanged(int)),this,SLOT(camera_frame_rate()));
        init_expmode_1();
        if(flag1 == 0){
            ui->frame_rate->setEnabled(false);
        }else{
            ui->frame_rate->setEnabled(true);
        }
    }
}

//打开摄像头
void MainWindow::on_cam_start_clicked()
{
    if(flagcamera == 0){
        if(flag==0){
            cap.open(0);
            flag=1;
        }
        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(importFrame()));
        timer->start(1);
        ui->frame_rate->setEnabled(true);
        //camera_gpio* a = new camera_gpio();
        //a->signalHandler();


    }else if(flagcamera == 1){
        on_cam_start_1_clicked();
    }else{

    }

}

//关闭摄像头
void MainWindow::on_cam_stop_clicked()
{
    if(flagcamera==0){
        timer->stop();
        if(ispic == true){
            timer_img->stop();
            ispic = false;
            ui->addpic->setText("start");
        }
        flag=0;
        cap.release();
        ui->Camera_view->clear();
        ui->Camera_view->setText("CAMERA0");
        ui->frame_rate->setEnabled(false);
    }else{
        on_cam_stop_1_clicked();
    }

}

//选择摄像头
void MainWindow::camera_select(){
    if(ui->comboBox_name->currentIndex()==0){
        flagcamera=0;
        param();
        qDebug() << flagcamera;
    }else{
        flagcamera=1;
        param();
        qDebug() << flagcamera;
    }
}

//构建画面
void MainWindow::importFrame(){
    cap >> frame;
    QImage  image = QImage((uchar*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
    ui->Camera_view->setPixmap(QPixmap::fromImage(image));
    ui->Camera_view->show();
//    if(count1<=300){
//        QString countstr = QString("%1").arg(count1);
//        QString picstr = "/home/nano/Desktop/img/camera0/"+countstr+".jpg";
//        image.save(picstr);
//    }
//    count1++;
}

//手动曝光时间参数值设置
void MainWindow::on_set_metime_value_clicked()
{
    if(flagcamera==0){
        QString value = ui->MetimeValue->text();
        double judgevalue = value.toDouble();
        if(judgevalue>=0 && judgevalue<=1000000){
            QProcess *setMetime = new QProcess(this);
            setMetime->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
            QString command = "./cs_mipi_i2c.sh";
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f metime -p1 " + value;
            setMetime->start(command,arrayCmdStr);
        }else{
            QMessageBox::warning(this,"warning","range:[0-1000000]");
        }
    }else{
        on_set_metime_value_1_clicked();
    }

}

//曝光模式选择
void MainWindow::select_expmode(){
    if(flagcamera == 0){
        if(expmodeGroup->checkedId()==0){
            ui->MetimeValue->setReadOnly(true);
            ui->MetimeValue->setEnabled(false);
            ui->set_metime_value->setEnabled(false);
            ui->dmedgain->setEnabled(false);
            ui->dmeagain->setEnabled(false);
            QProcess *setAutoExpmode = new QProcess(this);
            setAutoExpmode->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
            QString command = "./cs_mipi_i2c.sh";
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f expmode -p1 0";
            setAutoExpmode->start(command,arrayCmdStr);
        }else{
            ui->MetimeValue->setReadOnly(false);
            ui->MetimeValue->setEnabled(true);
            ui->set_metime_value->setEnabled(true);
            ui->dmedgain->setEnabled(true);
            ui->dmeagain->setEnabled(true);
            QProcess *setManualExpmode = new QProcess(this);
            setManualExpmode->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
            QString command = "./cs_mipi_i2c.sh";
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f expmode -p1 1";
            setManualExpmode->start(command,arrayCmdStr);
            render();
        }
    }else{
        select_expmode_1();
    }
}

//图片方向
void MainWindow::image_direction(){
    if(flagcamera == 0){
        QProcess *image_directions = new QProcess(this);
        image_directions->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
        QString command = "./cs_mipi_i2c.sh";
        if(imagedirectionGroup->checkedId()==0){
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f imagedir -p1 0";
            image_directions->start(command,arrayCmdStr);
        }else if(imagedirectionGroup->checkedId()==1){
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f imagedir -p1 1";
            image_directions->start(command,arrayCmdStr);
        }else if(imagedirectionGroup->checkedId()==2){
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f imagedir -p1 2";
            image_directions->start(command,arrayCmdStr);
        }else{
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f imagedir -p1 3";
            image_directions->start(command,arrayCmdStr);
        }
    }else{
        image_direction_1();
    }

}

//重构CAMERA0
void MainWindow::rest_cam_view(){
    if(flagcamera == 0){
        if(flag == 1){
            timer->stop();
            flag = 0;
            cap.release();
            //frame.release();
            int remaining = timer->remainingTime();
            ui->Camera_view->clear();
            timer->setInterval(remaining);
            on_cam_start_clicked();
        }else{
            on_cam_start_clicked();
        }


    }else{
        rest_cam_view_1();
    }
}

//帧率
void MainWindow::camera_frame_rate(){
    if(flagcamera == 0){
        QProcess *frame_rate_str = new QProcess(this);
        frame_rate_str->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
        QString command = "./cs_mipi_i2c.sh";
    //    qDebug() << ui->frame_rate->currentIndex();
        if(ui->frame_rate->currentIndex()==0){
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f videofmt -p1 1280 -p2 1080 -p3 45";
            frame_rate_str->start(command,arrayCmdStr);
        }else if(ui->frame_rate->currentIndex()==1){
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f videofmt -p1 1080 -p2 1280 -p3 45";
            frame_rate_str->start(command,arrayCmdStr);
        }else if(ui->frame_rate->currentIndex()==2){
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f videofmt -p1 1280 -p2 720 -p3 60";
            frame_rate_str->start(command,arrayCmdStr);
        }else if(ui->frame_rate->currentIndex()==3){
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f videofmt -p1 720 -p2 1280 -p3 60";
            frame_rate_str->start(command,arrayCmdStr);
        }else if(ui->frame_rate->currentIndex()==4){
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f videofmt -p1 640 -p2 480 -p3 120";
            frame_rate_str->start(command,arrayCmdStr);
        }else{
            QStringList arrayCmdStr;
            arrayCmdStr<<" -b 7 -w -f videofmt -p1 480 -p2 640 -p3 120";
            frame_rate_str->start(command,arrayCmdStr);
        }
        rest_cam_view();
    }else{
        camera_frame_rate_1();
    }

}

void MainWindow::render(){
    init_metime();
    init_medgain();
    init_meagain();
    init_image_direction();
}

//初始曝光模式选择
void MainWindow::init_expmode(){
    QProcess *init_expmode = new QProcess(this);
    init_expmode->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr << " -b 7 -r -f expmode";
    init_expmode->start(command,arrayCmdStr);
    init_expmode->waitForStarted();
    init_expmode->waitForFinished();
    QString res = init_expmode->readAllStandardOutput();
    QStringList str_list = res.split(" ");
    QString exmode = str_list[3];
    QString exmodeValue = exmode.at(0);
    if(exmodeValue == "1"){
        ui->ManualExpmode->setChecked(true);
        render();
        ui->MetimeValue->setEnabled(true);
        ui->dmedgain->setEnabled(true);
        ui->dmeagain->setEnabled(true);
    }else{
        ui->AutoExpmode->setChecked(true);
        ui->MetimeValue->setEnabled(false);
        ui->dmedgain->setEnabled(false);
        ui->dmeagain->setEnabled(false);
    }
}

//曝光时间初始渲染
void MainWindow::init_metime(){
    QProcess *init_medgain = new QProcess(this);
    init_medgain->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr << " -b 7 -r -f metime";
    init_medgain->start(command,arrayCmdStr);
    init_medgain->waitForStarted();
    init_medgain->waitForFinished();
    QString res = init_medgain->readAllStandardOutput();
    QStringList str_list = res.split(" ");
    QString metime = str_list[3];
    ui->MetimeValue->setText(metime);
}

//数字增益初始渲染
void MainWindow::init_medgain(){
    QProcess *init_medgain = new QProcess(this);
    init_medgain->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr << " -b 7 -r -f medgain";
    init_medgain->start(command,arrayCmdStr);
    init_medgain->waitForStarted();
    init_medgain->waitForFinished();
    QString res = init_medgain->readAllStandardOutput();
    QStringList str_list = res.split(" ");
    QString medgain = str_list[3];
//    qDebug() << medgain;
    double med = medgain.toDouble();
    ui->dmedgain->setValue(med);
}

//模拟增益初始渲染
void MainWindow::init_meagain(){
    QProcess *init_medgain = new QProcess(this);
    init_medgain->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr << " -b 7 -r -f meagain";
    init_medgain->start(command,arrayCmdStr);
    init_medgain->waitForStarted();
    init_medgain->waitForFinished();
    QString res = init_medgain->readAllStandardOutput();
    QStringList str_list = res.split(" ");
    QString medgain = str_list[3];
//    qDebug() << medgain;
    double med = medgain.toDouble();
    ui->dmeagain->setValue(med);
}

//图像方向初始渲染
void MainWindow::init_image_direction(){
    QProcess *init_image_direction = new QProcess(this);
    init_image_direction->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr << " -b 7 -r -f imagedir";
    init_image_direction->start(command,arrayCmdStr);
    init_image_direction->waitForStarted();
    init_image_direction->waitForFinished();
    QString res = init_image_direction->readAllStandardOutput();
    QStringList str_list = res.split(" ");
    QString image_dir = str_list[3];
    QString image_dir_value = image_dir.at(0);
    if(image_dir_value == "0"){
        ui->image_direction_normal->setChecked(true);
    }else if(image_dir_value == "1"){
        ui->image_direction_lr->setChecked(true);
    }else if(image_dir_value == "2"){
        ui->image_direction_ud->setChecked(true);
    }else{
        ui->image_direction_udlr->setChecked(true);
    }
}

//手动模拟增益
void MainWindow::on_dmeagain_valueChanged(const QString &arg1)
{
    if(flagcamera == 0){
        QStringList str_list = arg1.split(".");
        QString x = str_list[0];
        QString y = str_list[1];
        QProcess *frame_rate_str = new QProcess(this);
        frame_rate_str->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
        QString command = "./cs_mipi_i2c.sh";
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 7 -w -f meagain -p1 "+x+" -p2 "+y;
        frame_rate_str->start(command,arrayCmdStr);
    }else{
        on_dmeagain_1_valueChanged(arg1);
    }
}

//手动数字增益
void MainWindow::on_dmedgain_valueChanged(const QString &arg1)
{
    if(flagcamera == 0){
        QStringList str_list = arg1.split(".");
        QString x = str_list[0];
        QString y = str_list[1];
        QProcess *frame_rate_str = new QProcess(this);
        frame_rate_str->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
        QString command = "./cs_mipi_i2c.sh";
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 7 -w -f medgain -p1 "+x+" -p2 "+y;
        frame_rate_str->start(command,arrayCmdStr);
    }else{
        on_dmedgain_1_valueChanged(arg1);
    }

}

//断电保存参数
void MainWindow::on_saveparam_clicked()
{
    if(flagcamera == 0){
        QProcess *save = new QProcess(this);
        save->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
        QString command = "./cs_mipi_i2c.sh";
        QStringList arrayCmdStr;
        arrayCmdStr << " -b 7 -w -f paramsave";
        save->start(command,arrayCmdStr);
        qDebug() << "camera0参数保存成功";
    }else{
        on_saveparam_1_clicked();
    }

}

void MainWindow::ban(){
    ui->frame_rate->setEnabled(false);
    ui->AutoExpmode->setEnabled(false);
    ui->ManualExpmode->setEnabled(false);
    ui->MetimeValue->setReadOnly(true);
    ui->MetimeValue->setEnabled(false);
    ui->set_metime_value->setEnabled(false);
    ui->dmedgain->setEnabled(false);
    ui->dmeagain->setEnabled(false);
    ui->image_direction_lr->setEnabled(false);
    ui->image_direction_normal->setEnabled(false);
    ui->image_direction_normal->setEnabled(false);
    ui->image_direction_ud->setEnabled(false);
    ui->image_direction_udlr->setEnabled(false);
}

void MainWindow::noban(){
    ui->frame_rate->setEnabled(true);
    ui->AutoExpmode->setEnabled(true);
    ui->ManualExpmode->setEnabled(true);
    ui->MetimeValue->setReadOnly(false);
    ui->MetimeValue->setEnabled(true);
    ui->set_metime_value->setEnabled(true);
    ui->dmedgain->setEnabled(true);
    ui->dmeagain->setEnabled(true);
    ui->image_direction_lr->setEnabled(true);
    ui->image_direction_normal->setEnabled(true);
    ui->image_direction_normal->setEnabled(true);
    ui->image_direction_ud->setEnabled(true);
    ui->image_direction_udlr->setEnabled(true);
}


//CAMERA1打开摄像头
void MainWindow::on_cam_start_1_clicked()
{
    if(flag1 == 0){
        cap_1.open(1);
        flag1 = 1;
    }
    timer_1 = new QTimer(this);
    connect(timer_1,SIGNAL(timeout()),this,SLOT(importFrame_1()));
    timer_1->start(1);
    ui->frame_rate->setEnabled(true);
}

//CAMERA1关闭摄像头
void MainWindow::on_cam_stop_1_clicked()
{
    timer_1->stop();
    if(ispic == true){
        timer_img->stop();
        ispic = false;
        ui->addpic->setText("start");
    }
    flag1 = 0;
    cap_1.release();
    ui->Camera_view_1->clear();
    ui->Camera_view_1->setText("CAMERA1");
    ui->frame_rate->setEnabled(false);
}

//断电保存CAMERA1
void MainWindow::on_saveparam_1_clicked()
{
    QProcess *save = new QProcess(this);
    save->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr << " -b 8 -w -f paramsave";
    save->start(command,arrayCmdStr);
    qDebug() << "camera1参数保存成功";
}

//构建CAMERA1画面
void MainWindow::importFrame_1(){
    cap_1 >> frame_1;
    QImage  image = QImage((uchar*)frame_1.data,frame_1.cols,frame_1.rows,QImage::Format_RGB888).rgbSwapped();
    ui->Camera_view_1->setPixmap(QPixmap::fromImage(image));
    ui->Camera_view_1->show();
//    if(count2 <= 300){
//        QString countstr = QString("%1").arg(count2);
//        QString picstr = "/home/nano/Desktop/img/camera1/"+countstr+".jpg";
//        image.save(picstr);
//    }
//    count2++;
}

//CAMERA1手动曝光时间参数值设置
void MainWindow::on_set_metime_value_1_clicked()
{
    QString value = ui->MetimeValue->text();
    double judgevalue = value.toDouble();
    if(judgevalue >= 0 && judgevalue <= 1000000){
        QProcess *setMetime = new QProcess(this);
        setMetime->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
        QString command = "./cs_mipi_i2c.sh";
        QStringList arrayCmdStr;
        arrayCmdStr << " -b 8 -w -f metime -p1 " + value;
        setMetime->start(command,arrayCmdStr);
    }else{
        QMessageBox::warning(this,"warning","range:[0-1000000]");
    }
}

//CAMERA1曝光模式选择
void MainWindow::select_expmode_1(){
    if(expmodeGroup_1->checkedId() == 0){
        ui->MetimeValue->setReadOnly(true);
        ui->MetimeValue->setEnabled(false);
        ui->set_metime_value->setEnabled(false);
        ui->dmedgain->setEnabled(false);
        ui->dmeagain->setEnabled(false);
        QProcess *setAutoExpmode = new QProcess(this);
        setAutoExpmode->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
        QString command = "./cs_mipi_i2c.sh";
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 8 -w -f expmode -p1 0";
        setAutoExpmode->start(command,arrayCmdStr);
    }else{
        ui->MetimeValue->setReadOnly(false);
        ui->MetimeValue->setEnabled(true);
        ui->set_metime_value->setEnabled(true);
        ui->dmedgain->setEnabled(true);
        ui->dmeagain->setEnabled(true);
        QProcess *setManualExpmode = new QProcess(this);
        setManualExpmode->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
        QString command = "./cs_mipi_i2c.sh";
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 8 -w -f expmode -p1 1";
        setManualExpmode->start(command,arrayCmdStr);
        render_1();
    }
}

//CAMERA1图片方向
void MainWindow::image_direction_1(){
    QProcess *image_directions = new QProcess(this);
    image_directions->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    if(imagedirectionGroup_1->checkedId()==0){
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 8 -w -f imagedir -p1 0";
        image_directions->start(command,arrayCmdStr);
    }else if(imagedirectionGroup_1->checkedId()==1){
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 8 -w -f imagedir -p1 1";
        image_directions->start(command,arrayCmdStr);
    }else if(imagedirectionGroup_1->checkedId()==2){
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 8 -w -f imagedir -p1 2";
        image_directions->start(command,arrayCmdStr);
    }else{
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 8 -w -f imagedir -p1 3";
        image_directions->start(command,arrayCmdStr);
    }
}

//CAMERA1重构
void MainWindow::rest_cam_view_1(){
    if(flag1 == 1){
        timer_1->stop();
        flag1 = 0;
        cap_1.release();
        //frame_1.release();
        int remaining = timer_1->remainingTime();
        ui->Camera_view_1->clear();
        timer_1->setInterval(remaining);
        on_cam_start_1_clicked();
    }else{
        on_cam_start_1_clicked();
    }

}

//CAMERA1帧率
void MainWindow::camera_frame_rate_1(){
    QProcess *frame_rate_str = new QProcess(this);
    frame_rate_str->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    if(ui->frame_rate->currentIndex()==0){
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 8 -w -f videofmt -p1 1280 -p2 1080 -p3 45";
        frame_rate_str->start(command,arrayCmdStr);
    }else if(ui->frame_rate->currentIndex()==1){
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 8 -w -f videofmt -p1 1080 -p2 1280 -p3 45";
        frame_rate_str->start(command,arrayCmdStr);
    }else if(ui->frame_rate->currentIndex()==2){
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 8 -w -f videofmt -p1 1280 -p2 720 -p3 60";
        frame_rate_str->start(command,arrayCmdStr);
    }else if(ui->frame_rate->currentIndex()==3){
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 8 -w -f videofmt -p1 720 -p2 1280 -p3 60";
        frame_rate_str->start(command,arrayCmdStr);
    }else if(ui->frame_rate->currentIndex()==4){
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 8 -w -f videofmt -p1 640 -p2 480 -p3 120";
        frame_rate_str->start(command,arrayCmdStr);
    }else{
        QStringList arrayCmdStr;
        arrayCmdStr<<" -b 8 -w -f videofmt -p1 480 -p2 640 -p3 120";
        frame_rate_str->start(command,arrayCmdStr);
    }
    rest_cam_view_1();
}

void MainWindow::render_1(){
    init_metime_1();
    init_medgain_1();
    init_meagain_1();
    init_image_direction_1();
}

//初始曝光模式选择
void MainWindow::init_expmode_1(){
    QProcess *init_expmode_1 = new QProcess(this);
    init_expmode_1->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr << " -b 8 -r -f expmode";
    init_expmode_1->start(command,arrayCmdStr);
    init_expmode_1->waitForStarted();
    init_expmode_1->waitForFinished();
    QString res = init_expmode_1->readAllStandardOutput();
    QStringList str_list = res.split(" ");
    QString exmode = str_list[3];
    QString exmodeValue = exmode.at(0);
    qDebug() << exmodeValue;
    if(exmodeValue == "1"){
        ui->ManualExpmode->setChecked(true);
        render_1();
        ui->MetimeValue->setEnabled(true);
        ui->dmedgain->setEnabled(true);
        ui->dmeagain->setEnabled(true);
    }else{
        ui->AutoExpmode->setChecked(true);
        ui->MetimeValue->setEnabled(false);
        ui->dmedgain->setEnabled(false);
        ui->dmeagain->setEnabled(false);
    }
}

//CAMERA1曝光时间初始化渲染
void MainWindow::init_metime_1(){
    QProcess *init_metime_1 = new QProcess(this);
    init_metime_1->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr << " -b 8 -r -f metime";
    init_metime_1->start(command,arrayCmdStr);
    init_metime_1->waitForStarted();
    init_metime_1->waitForFinished();
    QString res_1 = init_metime_1->readAllStandardOutput();
    QStringList str_list_1 = res_1.split(" ");
    QString metime_1 = str_list_1[3];
    ui->MetimeValue->setText(metime_1);
}

//CAMERA1数字增益初始化渲染
void MainWindow::init_medgain_1(){
    QProcess *init_medgain_1 = new QProcess(this);
    init_medgain_1->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr << " -b 8 -r -f medgain";
    init_medgain_1->start(command,arrayCmdStr);
    init_medgain_1->waitForStarted();
    init_medgain_1->waitForFinished();
    QString res_1 = init_medgain_1->readAllStandardOutput();
    QStringList str_list_1 = res_1.split(" ");
    QString medgain_1 = str_list_1[3];
    double med_1 = medgain_1.toDouble();
    ui->dmedgain->setValue(med_1);
}

//CAMERA1模拟增益初始化渲染
void MainWindow::init_meagain_1(){
    QProcess *init_medgain_1 = new QProcess(this);
    init_medgain_1->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr << " -b 8 -r -f meagain";
    init_medgain_1->start(command,arrayCmdStr);
    init_medgain_1->waitForStarted();
    init_medgain_1->waitForFinished();
    QString res_1 = init_medgain_1->readAllStandardOutput();
    QStringList str_list_1 = res_1.split(" ");
    QString medgain_1 = str_list_1[3];
    double med_1 = medgain_1.toDouble();
    ui->dmeagain->setValue(med_1);
}

//图像方向初始渲染
void MainWindow::init_image_direction_1(){
    QProcess *init_image_direction_1 = new QProcess(this);
    init_image_direction_1->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr << " -b 8 -r -f imagedir";
    init_image_direction_1->start(command,arrayCmdStr);
    init_image_direction_1->waitForStarted();
    init_image_direction_1->waitForFinished();
    QString res = init_image_direction_1->readAllStandardOutput();
    QStringList str_list = res.split(" ");
    QString image_dir = str_list[3];
    QString image_dir_value = image_dir.at(0);
    if(image_dir_value == "0"){
        ui->image_direction_normal->setChecked(true);
    }else if(image_dir_value == "1"){
        ui->image_direction_lr->setChecked(true);
    }else if(image_dir_value == "2"){
        ui->image_direction_ud->setChecked(true);
    }else{
        ui->image_direction_udlr->setChecked(true);
    }
}

//CAMERA1手动模拟增益
void MainWindow::on_dmeagain_1_valueChanged(const QString &arg1)
{
    //qDebug() << arg1;
    QStringList str_list = arg1.split(".");
    QString x = str_list[0];
    QString y = str_list[1];
    QProcess *frame_rate_str = new QProcess(this);
    frame_rate_str->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr<<" -b 8 -w -f meagain -p1 "+x+" -p2 "+y;
    frame_rate_str->start(command,arrayCmdStr);
//    qDebug() << command << " " << arrayCmdStr;
}

//CAMERA1手动数字增益
void MainWindow::on_dmedgain_1_valueChanged(const QString &arg1)
{
    QStringList str_list = arg1.split(".");
    QString x = str_list[0];
    QString y = str_list[1];
    QProcess *frame_rate_str = new QProcess(this);
    frame_rate_str->setWorkingDirectory("/home/nano/nvidia_jetson_veye_bsp-master/i2c_cmd/bin");
    QString command = "./cs_mipi_i2c.sh";
    QStringList arrayCmdStr;
    arrayCmdStr<<" -b 8 -w -f medgain -p1 "+x+" -p2 "+y;
    frame_rate_str->start(command,arrayCmdStr);
}

//抓图片
void MainWindow::on_addpic_clicked()
{
    if(!cap.isOpened()){
        qDebug() << "no";
        return ;
    }
    if(cap.isOpened() && cap_1.isOpened() && ispic == false){
        count1=0;
        count2=0;
        timer_img = new QTimer(this);
        ui->addpic->setText("stop");
        ispic = true;
        connect(timer_img,SIGNAL(timeout()),this,SLOT(saveImage()));
        connect(timer_img,SIGNAL(timeout()),this,SLOT(saveImage_1()));
        timer_img->start(200);
    }else if(cap.isOpened() && cap_1.isOpened() && ispic){
        ui->addpic->setText("start");
        timer_img->stop();
        ispic = false;
    }else{
        return ;
    }
}
void MainWindow::saveImage(){
    //cap >> frame;
    QImage  image = QImage((uchar*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
    if(count1<=300){
        QString countstr = QString("%1").arg(count1);
        QString picstr = "/home/nano/Desktop/img/camera0/"+countstr+".jpg";
        image.save(picstr);
    }
    count1++;
}
void MainWindow::saveImage_1(){
    //cap >> frame;
    QImage  image_1 = QImage((uchar*)frame_1.data,frame_1.cols,frame_1.rows,QImage::Format_RGB888).rgbSwapped();
    if(count2<=300){
        QString countstr = QString("%1").arg(count2);
        QString picstr = "/home/nano/Desktop/img/camera1/"+countstr+".jpg";
        image_1.save(picstr);
    }
    count2++;
}


void MainWindow::on_syntesis_img_clicked()
{
    BinarzationStack();
}

//图片合成
void MainWindow::BinarzationStack(){
    int heights,widths;
    Point binarizationpoints;
    std::vector<Point> pointsvalue;
    std::vector<uchar>colorvalue;
    while(cnt<500){
        QString str = QString("%1").arg(cnt);
        QString strs = "/home/nano/images/img"+str+".jpg";
        Mat img = imread(strs.toUtf8().data());
        if(img.empty()){
            break;
        }
        Mat gray_img;
        cvtColor(img,gray_img,COLOR_BGR2GRAY,0);
        heights = gray_img.rows;
        widths = gray_img.cols;
        for(int i=0;i<heights;i++){
            for(int j=0;j<widths;j++){
                float vlaue = gray_img.at<uchar>(i,j);
                if(vlaue>judgevalu){
                    gray_img.at<uchar>(i,j)=255;
                    colorvalue.push_back(gray_img.at<uchar>(i,j));
                    binarizationpoints.x=i;binarizationpoints.y=j;
                    pointsvalue.push_back(binarizationpoints);
                }
            }
        }
        cnt++;
        gray_img.release();
        img.release();
    }
    qDebug()<<heights<<widths<<pointsvalue.size()<<colorvalue.size();

//    Mat stack_img = imread("/home/nano/3.jpg");
    Mat stack_img(heights,widths,CV_8UC3);
    cvtColor(stack_img,stack_img,COLOR_BGR2GRAY,0);
    for(int i=0;i<heights;i++){
        for(int j=0;j<widths;j++){
            stack_img.at<uchar>(i,j)=0;
        }
    }
    for(int k=0;k<pointsvalue.size();k++){
        stack_img.at<uchar>(pointsvalue[k].x,pointsvalue[k].y)=colorvalue[k];

    }
    imshow("1",stack_img);
    imwrite("/home/nano/Desktop/img/test/合成.png",stack_img);
    pointsvalue.clear();
    colorvalue.clear();

    ImageCorrosion(stack_img);
}
//图像腐蚀
void MainWindow::ImageCorrosion(Mat img){
    uchar convolutionKernel[8][8];
    int width = img.cols;
    //float maxValue=0;
    memset(convolutionKernel,255,sizeof(convolutionKernel));
    Mat k(img.size(), img.type(), Scalar::all(0));
    for (int i = 1; i < img.rows-1; i++){
        uchar *data = img.ptr<uchar>(i);
        uchar *k_mask = k.ptr<uchar>(i);
        for (int j = 1; j < img.cols - 1; j++){
            if (*data > 100){
                k_mask[j] = 255;
                for (int ii = 0; ii < 8; ii++){
                    for (int jj = 0; jj < 8; jj++){
                        if (convolutionKernel[ii][jj] == 0){
                            continue;
                        }
                        uchar flag = *(data + (ii - 1)*width + (jj - 1));
                        if (flag < 100){
                            k_mask[j] = 0;
                            break;
                        }
                    }
                }
           }
           data++;
       }
   }
   img = k;
 //   cvtColor(img,img,CV_BGR2GRAY);
   imshow("2",img);
   imwrite("/home/nano/Desktop/img/test/fushi.png",img);
   Mat dis_img(img.size(),CV_32FC1);
   distanceTransform(img,dis_img,CV_DIST_L2,3);
   normalize(dis_img,dis_img,0,1.0,CV_MINMAX);
   imshow("3",dis_img);
   dis_img.convertTo(dis_img,CV_8UC1);
   //int oustv = OustAlgorithm(dis_img);
   threshold(dis_img,dis_img,0,255,THRESH_BINARY | THRESH_OTSU);
   vector<vector<Point>> contours;
   vector<Vec4i> hierarchy;
   findContours(dis_img,contours,hierarchy,RETR_LIST,CHAIN_APPROX_NONE);

   imshow("4",dis_img);
   qDebug()<<contours.size();
   dis_img.release();
}

void MainWindow::testgpio(){
    qDebug() << "主线程对象地址:  " << QThread::currentThread();
    // 创建子线程
    MyThread* subThread = new MyThread;
    subThread->start();


//    connect(subThread, &MyThread::curNumber, this, [=](int num)
//    {
//        ui->label->setNum(num);
//    });

//    connect(ui->startBtn, &QPushButton::clicked, this, [=]()
//    {
//        // 启动子线程
//        subThread->start();
//    });
}
//Otsu阀值分割
//int MainWindow::OustAlgorithm(Mat img){
//    int T;
//    float Oustvalue=0;
//    float weight0=0,average0=0,weight1=0,average1=0;
//    float histogram[256];
//    uchar *data = img.data;
//    float totalpixels = img.cols*img.rows;
//    memset(histogram,0,256*sizeof(float));
//    for(int i=0;i<img.rows;i++){
//        for(int j=0;j<img.cols;j++){
//            histogram[data[i*img.step+j]]++;
//        }
//    }
//    for(int i=0;i<255;i++){
//        weight0=0;average0=0;weight1=0;average1=0;
//        for(int j=0;j<=i;j++){
//            weight0+= histogram[j];
//            average0+= histogram[j]*j;
//        }
//        if(weight0==0){
//            break;
//        }
//        average0 = average0/weight0;
//        weight0 = weight0/totalpixels;
//        for(int k=i+1;k<255;k++){
//            weight1+=histogram[k];
//            average1+=histogram[k]*k;
//        }
//        if(weight1==0){
//            break;
//        }
//        average1 = average1/weight1;
//        weight1 = weight1/totalpixels;
//        float Oustvalue1 = weight0*weight1*(average0-average1)*(average0-average1);
//        if(Oustvalue<Oustvalue1){
//            Oustvalue = Oustvalue1;
//            T=i;
//        }
//    }
//    return T;
//}
