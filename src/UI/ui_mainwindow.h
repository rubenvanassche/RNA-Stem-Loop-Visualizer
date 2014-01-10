/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Jan 10 17:00:54 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QPushButton *OpenAlgoWindowButton;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *AnalyzeButton;
    QPlainTextEdit *Input;
    QLabel *label_2;
    QComboBox *AnalyzeTypeSelect;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(397, 384);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        OpenAlgoWindowButton = new QPushButton(centralWidget);
        OpenAlgoWindowButton->setObjectName(QString::fromUtf8("OpenAlgoWindowButton"));

        gridLayout->addWidget(OpenAlgoWindowButton, 14, 2, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setEnabled(false);

        gridLayout->addWidget(pushButton, 14, 1, 1, 1);

        AnalyzeButton = new QPushButton(centralWidget);
        AnalyzeButton->setObjectName(QString::fromUtf8("AnalyzeButton"));

        gridLayout->addWidget(AnalyzeButton, 14, 0, 1, 1);

        Input = new QPlainTextEdit(centralWidget);
        Input->setObjectName(QString::fromUtf8("Input"));

        gridLayout->addWidget(Input, 3, 0, 1, 3);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        AnalyzeTypeSelect = new QComboBox(centralWidget);
        AnalyzeTypeSelect->insertItems(0, QStringList()
         << QString::fromUtf8("PDA")
         << QString::fromUtf8("LLParser")
         << QString::fromUtf8("Turing")
        );
        AnalyzeTypeSelect->setObjectName(QString::fromUtf8("AnalyzeTypeSelect"));

        gridLayout->addWidget(AnalyzeTypeSelect, 0, 2, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 397, 22));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "RNA Stem Loop Analyzer", 0, QApplication::UnicodeUTF8));
        OpenAlgoWindowButton->setText(QApplication::translate("MainWindow", "Algorithms", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "RNA Stem Loop", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Visual RSL", 0, QApplication::UnicodeUTF8));
        AnalyzeButton->setText(QApplication::translate("MainWindow", "Analyze", 0, QApplication::UnicodeUTF8));
        Input->setPlainText(QString());
        label_2->setText(QApplication::translate("MainWindow", "Algorithm to use:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
