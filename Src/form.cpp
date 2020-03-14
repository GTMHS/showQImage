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
//	ICameraPtr cameraSptr;
//	//发现设备
//	CSystem &systemObj = CSystem::getInstance();
//	TVector<ICameraPtr> vCameraPtrList;
//	bool bRet = systemObj.discovery(vCameraPtrList);
//
//	if (!bRet)
//	{
//		QMessageBox::warning(NULL, "warning", "发现设备失败\n", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//		return;
//	}
//
//	if (0 == vCameraPtrList.size())
//	{
//        QMessageBox::warning(NULL, "warning", "发现摄像头失败\n");
//		return;
//	}
//
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
	ui->widget->setLabelText("test");
	ui->widget->testRun();
//
//    try {
//        ui->widget->CameraCheck();
//
//        bool camera_open = ui->widget->CameraOpen();
////        if(!camera_open)
////        {
////            QMessageBox::warning(NULL, "warning", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
////        }
//        ui->widget->CameraStart();
//
//        // 设置曝光
//        ui->widget->SetExposeTime(10000);
//
//        // 设置增益
//        ui->widget->SetAdjustPlus(5);
//
//        // 设置连续拉流。
//        ui->widget->CameraChangeTrig(CammerWidget::trigContinous);
//
//        //// 设置软触发拉流
//        //ui->widget->CameraChangeTrig(CammerWidget::trigSoftware);
//		//// 设置硬件触发拉流
//		//ui->widget->CameraChangeTrig(CammerWidget::trigLine);
//        //// 软触发100次
//        //for (int i = 0; i<100; i++)
//        //{
//        //	ui->widget->ExecuteSoftTrig();
//        //}
//    } catch (Exception e) {
//        QMessageBox::warning(NULL, "warning in open camera", e.what(), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//    }	
}

void Form::on_pushButton_2_clicked()
{
    ui->widget->CameraStop();
    ui->widget->CameraClose();
	ui->pushButton->setEnabled(true);
	ui->pushButton_2->setEnabled(false);
	ui->pushButton_3->setEnabled(false);
	ui->pushButton_4->setEnabled(false);
	//https://blog.csdn.net/tom555cat/article/details/17389247
	string dirpath = "G:\\Pic\\";

	_finddata_t file;
	long lf;
	char suffixs[] = "*.bmp";          //要寻找的文件类型
	vector<string> fileNameList;   //文件夹下.amc类型文件的名字向量
	char *p;
	p = (char *)malloc((dirpath.size() + 1) * sizeof(char));
	strcpy(p, dirpath.c_str());

	//获取文件名向量
	if ((lf = _findfirst(strcat(p, suffixs), &file)) == -1l)
	{
		cout << "文件没有找到!\n";
	}
	else
	{
		cout << "\n文件列表:\n";
		do {
			cout << file.name << endl;
			ui->plainTextEdit->appendPlainText(file.name);
			//str是用来保存文件名的字符串string
			string str(file.name);
			fileNameList.push_back(str);
			cout << endl;
		} while (_findnext(lf, &file) == 0);
	}
	_findclose(lf);

	//遍历文件名向量，并进行修改
	string strAdd = "_walk.amc";   //在原文件名的基础上要增加的部分
	for (vector<string>::iterator iter = fileNameList.begin(); iter != fileNameList.end(); ++iter)
	{
		string oldName = dirpath + *iter;
		//str1为原文件名要保留的部分
		string str1;
		str1 = (*iter).substr(0, 3);
		string newName = dirpath + str1 + strAdd;
		cout << "oldName:" << oldName << endl;
		cout << "newName" << newName << endl;

		cout << "oldName size = " << oldName.size() << endl;
		cout << "newName size = " << newName.size() << endl;

		char *oldNamePointer, *newNamePointer;
		oldNamePointer = (char *)malloc((oldName.size() + 1) * sizeof(char));
		newNamePointer = (char *)malloc((newName.size() + 1) * sizeof(char));
		strcpy(oldNamePointer, oldName.c_str());
		strcpy(newNamePointer, newName.c_str());
		cout << oldNamePointer << endl;
		cout << newNamePointer << endl;

		rename(oldNamePointer, newNamePointer);

		free(oldNamePointer);
		free(newNamePointer);
	}
	//system("PAUSE");
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

//int find_all_files(const char * lpPath)
//{
//	char szFind[MAX_PATH];
//	WIN32_FIND_DATA FindFileData;
//	strcpy(szFind, lpPath);
//	strcat(szFind, "\\*.*");
//
//	cout << szFind << endl;
//
//	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
//	if (INVALID_HANDLE_VALUE == hFind)
//		return -1;
//
//	do
//	{
//		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//		{
//			if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0)
//			{
//				//发现子目录，递归之
//				char szFile[MAX_PATH] = { 0 };
//				strcpy(szFile, lpPath);
//				strcat(szFile, "\\");
//				strcat(szFile, FindFileData.cFileName);
//				find_all_files(szFile);
//			}
//		}
//		else
//		{
//			//找到文件，处理之
//			cout << lpPath << "\\" << FindFileData.cFileName << endl;
//
//			string temporaryName = FindFileData.cFileName;
//
//			string oldName(lpPath);
//			oldName += "\\";
//			oldName += temporaryName;
//			string newName = oldName + ".png";
//			//cout<<"newName:"<<newName<<endl;
//			//FindFileData.cFileName = temporaryName;
//			if (rename(oldName.c_str(), newName.c_str()) == 0)
//
//				//printf("Renamed %s to %s.\n", oldName, newName);
//				cout << oldName + "->" + newName + "\n";
//
//			else
//
//				perror("rename");
//
//			//cout<<"new:"<<FindFileData.cFileName<<endl;
//
//		}
//	} while (::FindNextFile(hFind, &FindFileData));
//
//	::FindClose(hFind);
//
//	return 0;
//}
