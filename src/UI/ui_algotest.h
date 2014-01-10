/********************************************************************************
** Form generated from reading UI file 'algotest.ui'
**
** Created: Fri Jan 10 16:48:26 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALGOTEST_H
#define UI_ALGOTEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AlgoTest
{
public:
    QVBoxLayout *verticalLayout;
    QComboBox *AlgoTypeSelect;
    QPushButton *pushButton;
    QLabel *AcceptedLabel;

    void setupUi(QDialog *AlgoTest)
    {
        if (AlgoTest->objectName().isEmpty())
            AlgoTest->setObjectName(QString::fromUtf8("AlgoTest"));
        AlgoTest->resize(400, 157);
        verticalLayout = new QVBoxLayout(AlgoTest);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        AlgoTypeSelect = new QComboBox(AlgoTest);
        AlgoTypeSelect->setObjectName(QString::fromUtf8("AlgoTypeSelect"));

        verticalLayout->addWidget(AlgoTypeSelect);

        pushButton = new QPushButton(AlgoTest);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        AcceptedLabel = new QLabel(AlgoTest);
        AcceptedLabel->setObjectName(QString::fromUtf8("AcceptedLabel"));

        verticalLayout->addWidget(AcceptedLabel);


        retranslateUi(AlgoTest);

        QMetaObject::connectSlotsByName(AlgoTest);
    } // setupUi

    void retranslateUi(QDialog *AlgoTest)
    {
        AlgoTest->setWindowTitle(QApplication::translate("AlgoTest", "Dialog", 0, QApplication::UnicodeUTF8));
        AlgoTypeSelect->clear();
        AlgoTypeSelect->insertItems(0, QStringList()
         << QApplication::translate("AlgoTest", "PDA", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AlgoTest", "CFG", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AlgoTest", "CNF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AlgoTest", "Turing", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AlgoTest", "LLParser", 0, QApplication::UnicodeUTF8)
        );
        pushButton->setText(QApplication::translate("AlgoTest", "XML File", 0, QApplication::UnicodeUTF8));
        AcceptedLabel->setText(QApplication::translate("AlgoTest", "Please select a File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AlgoTest: public Ui_AlgoTest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALGOTEST_H
