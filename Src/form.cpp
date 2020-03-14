#include "form.h"
#include "ui_form.h"
//#include "dfindbook.h"

Form::Form(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

	ui->pushButton->setEnabled(true);
	ui->pushButton_2->setEnabled(false);
	ui->pushButton_3->setEnabled(false);
	ui->pushButton_4->setEnabled(false);

	//CammerWidget *subui;
	//subui = new Form();
	//connect(subui, SIGNAL(SendUpdateLCDMsg(int)), this, SLOT(UpdateLCD()));
	connect(ui->widget, SIGNAL(SendUpdateLCDMsg(int)), this, SLOT(UpdateLCD(int)));
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
////	ICameraPtr cameraSptr;
////	//发现设备
////	CSystem &systemObj = CSystem::getInstance();
////	TVector<ICameraPtr> vCameraPtrList;
////	bool bRet = systemObj.discovery(vCameraPtrList);
////
////	if (!bRet)
////	{
////		QMessageBox::warning(NULL, "warning", "发现设备失败\n", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
////		return;
////	}
////
////	if (0 == vCameraPtrList.size())
////	{
////        QMessageBox::warning(NULL, "warning", "发现摄像头失败\n");
////		return;
////	}
////
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
	ui->widget->setLabelText("test");
	ui->widget->testRun();

    try {
        ui->widget->CameraCheck();

        bool camera_open = ui->widget->CameraOpen();
//        if(!camera_open)
//        {
//            QMessageBox::warning(NULL, "warning", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//        }
        ui->widget->CameraStart();

        // 设置曝光
        ui->widget->SetExposeTime(10000);

        // 设置增益
        ui->widget->SetAdjustPlus(5);

        // 设置连续拉流。
        ui->widget->CameraChangeTrig(CammerWidget::trigContinous);

        //// 设置软触发拉流
        //ui->widget->CameraChangeTrig(CammerWidget::trigSoftware);
		//// 设置硬件触发拉流
		//ui->widget->CameraChangeTrig(CammerWidget::trigLine);
        //// 软触发100次
        //for (int i = 0; i<100; i++)
        //{
        //	ui->widget->ExecuteSoftTrig();
        //}
    } catch (Exception e) {
        QMessageBox::warning(NULL, "warning in open camera", e.what(), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }	
}

void Form::on_pushButton_2_clicked()
{
    ui->widget->CameraStop();
    ui->widget->CameraClose();
	ui->pushButton->setEnabled(true);
	ui->pushButton_2->setEnabled(false);
	ui->pushButton_3->setEnabled(false);
	ui->pushButton_4->setEnabled(false);
}

void Form::on_pushButton_3_clicked()//开始识别
{
	ui->pushButton->setEnabled(false);
	ui->pushButton_2->setEnabled(false);
	ui->pushButton_3->setEnabled(false);
	ui->pushButton_4->setEnabled(true);
	//将相机改为硬件触发，每触发一次执行一次判断
	ui->widget->CameraChangeTrig(CammerWidget::trigLine);
	//ui->widget->bookdetection();
	
}

void Form::on_pushButton_4_clicked()//停止识别
{
	ui->pushButton->setEnabled(false);
	ui->pushButton_2->setEnabled(true);
	ui->pushButton_3->setEnabled(true);
	ui->pushButton_4->setEnabled(false);
	//恢复相机为持续拉流
	ui->widget->CameraChangeTrig(CammerWidget::trigContinous);

}

void Form::closeEvent(QCloseEvent * event)
{
	ui->widget->CameraStop();

	ui->widget->CameraClose();
}

void Form::UpdateLCD(int LCDNum) {	
	//ui->lcdNumber->setDigitCount(10);
	//ui->lcdNumber_2->setDigitCount(50);
	//ui->lcdNumber_3->setDigitCount(60);
	ui->lcdNumber->display(20);
	ui->lcdNumber_2->display(50);
	ui->lcdNumber_3->display(60);
	//if (LCDNum == 1) {
	//	ui->lcdNumber_2->setDigitCount(50);
	//	ui->lcdNumber_3->setDigitCount(0);
	//}
	//else {
	//	ui->lcdNumber_2->setDigitCount(0);
	//	ui->lcdNumber_3->setDigitCount(50);
	//}
}

////A.cpp
//A::A()
//{
//	b = new A;
//	connect(b, SIGNAL(toA([ParamList])), this, SLOT(fromB([ParamList])));
//}
//void A::fromB([ParamList])
//{
//	//get[ParamList]
//}