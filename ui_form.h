/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include "Dahua.h"

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGridLayout *gridLayout_2;
    CammerWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_4;
    QPushButton *pushButton_2;
    QLabel *label_3;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QSpacerItem *verticalSpacer;
    QLCDNumber *lcdNumber_3;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_2;
    QLCDNumber *lcdNumber;
    QLabel *label;
    QLCDNumber *lcdNumber_2;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->setWindowModality(Qt::WindowModal);
        Form->resize(800, 670);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Form->sizePolicy().hasHeightForWidth());
        Form->setSizePolicy(sizePolicy);
        Form->setMinimumSize(QSize(800, 670));
        Form->setMaximumSize(QSize(1920, 1080));
        gridLayout_2 = new QGridLayout(Form);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        widget = new CammerWidget(Form);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        widget->setMinimumSize(QSize(600, 650));
        widget->setMaximumSize(QSize(1920, 1080));

        gridLayout_2->addWidget(widget, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton_4 = new QPushButton(Form);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        gridLayout->addWidget(pushButton_4, 1, 1, 1, 1);

        pushButton_2 = new QPushButton(Form);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(100, 20));

        gridLayout->addWidget(pushButton_2, 0, 1, 1, 1);

        label_3 = new QLabel(Form);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 7, 0, 1, 1);

        pushButton = new QPushButton(Form);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMaximumSize(QSize(100, 20));

        gridLayout->addWidget(pushButton, 0, 0, 1, 1);

        pushButton_3 = new QPushButton(Form);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 8, 0, 1, 1);

        lcdNumber_3 = new QLCDNumber(Form);
        lcdNumber_3->setObjectName(QStringLiteral("lcdNumber_3"));

        gridLayout->addWidget(lcdNumber_3, 7, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Preferred);

        gridLayout->addItem(verticalSpacer_2, 2, 0, 1, 1);

        label_2 = new QLabel(Form);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 6, 0, 1, 1);

        lcdNumber = new QLCDNumber(Form);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));

        gridLayout->addWidget(lcdNumber, 5, 1, 1, 1);

        label = new QLabel(Form);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 5, 0, 1, 1);

        lcdNumber_2 = new QLCDNumber(Form);
        lcdNumber_2->setObjectName(QStringLiteral("lcdNumber_2"));

        gridLayout->addWidget(lcdNumber_2, 6, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 1, 1, 1);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("Form", "\345\201\234\346\255\242\350\257\206\345\210\253", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Form", "\345\205\263\351\227\255\346\221\204\345\203\217\345\244\264", Q_NULLPTR));
        label_3->setText(QApplication::translate("Form", "\351\224\231\350\257\257\346\225\260", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Form", "\346\211\223\345\274\200\346\221\204\345\203\217\345\244\264", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("Form", "\345\274\200\345\247\213\350\257\206\345\210\253", Q_NULLPTR));
        label_2->setText(QApplication::translate("Form", "\346\255\243\347\241\256\346\225\260", Q_NULLPTR));
        label->setText(QApplication::translate("Form", "\346\200\273\346\225\260", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
