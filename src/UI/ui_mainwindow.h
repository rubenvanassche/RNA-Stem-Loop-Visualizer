/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Jan 16 14:32:41 2014
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
    QLabel *label;
    QPlainTextEdit *Input;
    QLabel *label_2;
    QComboBox *AnalyzeTypeSelect;
    QPushButton *VisualizeButton;
    QPushButton *AnalyzeButton;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(397, 309);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(16, 12, 94, 16));
        Input = new QPlainTextEdit(centralWidget);
        Input->setObjectName(QString::fromUtf8("Input"));
        Input->setGeometry(QRect(12, 35, 373, 163));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(12, 218, 98, 16));
        AnalyzeTypeSelect = new QComboBox(centralWidget);
        AnalyzeTypeSelect->insertItems(0, QStringList()
         << QString::fromUtf8("CFG")
         << QString::fromUtf8("LLParser")
         << QString::fromUtf8("Turing")
         << QString::fromUtf8("PDA")
        );
        AnalyzeTypeSelect->setObjectName(QString::fromUtf8("AnalyzeTypeSelect"));
        AnalyzeTypeSelect->setGeometry(QRect(280, 210, 108, 26));
        VisualizeButton = new QPushButton(centralWidget);
        VisualizeButton->setObjectName(QString::fromUtf8("VisualizeButton"));
        VisualizeButton->setEnabled(false);
        VisualizeButton->setGeometry(QRect(10, 250, 191, 32));
        AnalyzeButton = new QPushButton(centralWidget);
        AnalyzeButton->setObjectName(QString::fromUtf8("AnalyzeButton"));
        AnalyzeButton->setGeometry(QRect(198, 250, 191, 32));
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
        label->setText(QApplication::translate("MainWindow", "RNA Stem Loop:", 0, QApplication::UnicodeUTF8));
        Input->setPlainText(QString());
        label_2->setText(QApplication::translate("MainWindow", "Algorithm to use:", 0, QApplication::UnicodeUTF8));
        VisualizeButton->setText(QApplication::translate("MainWindow", "Visual RSL", 0, QApplication::UnicodeUTF8));
        AnalyzeButton->setText(QApplication::translate("MainWindow", "Analyze", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
