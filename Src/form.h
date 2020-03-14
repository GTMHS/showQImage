#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QMessageBox>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <io.h>
#include <stdlib.h>
#include <vector>
using namespace std;

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private:
	Ui::Form *ui;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

	void closeEvent(QCloseEvent * event);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

	void UpdateLCD(int LCDNUm);



	////A.h
	//class A
	//{
	//	....
	//private:
	//	B *b;
	//	private slots:
	//	void fromB([ParamList]);
	//}

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
};

#endif // FORM_H
