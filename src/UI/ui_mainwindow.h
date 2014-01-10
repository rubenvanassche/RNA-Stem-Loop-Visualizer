/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Jan 10 14:52:24 2014
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
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *Input;
    QPushButton *AnalyzeButton;
    QComboBox *AnalyzeTypeSelect;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        Input = new QPlainTextEdit(centralWidget);
        Input->setObjectName(QString::fromUtf8("Input"));

        verticalLayout->addWidget(Input);

        AnalyzeButton = new QPushButton(centralWidget);
        AnalyzeButton->setObjectName(QString::fromUtf8("AnalyzeButton"));

        verticalLayout->addWidget(AnalyzeButton);

        AnalyzeTypeSelect = new QComboBox(centralWidget);
        AnalyzeTypeSelect->setObjectName(QString::fromUtf8("AnalyzeTypeSelect"));

        verticalLayout->addWidget(AnalyzeTypeSelect);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 22));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "RNA Stem Loop Analyzer", 0, QApplication::UnicodeUTF8));
        Input->setPlainText(QApplication::translate("MainWindow", "Blablabla", 0, QApplication::UnicodeUTF8));
        AnalyzeButton->setText(QApplication::translate("MainWindow", "Analyze", 0, QApplication::UnicodeUTF8));
        AnalyzeTypeSelect->clear();
        AnalyzeTypeSelect->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "PDA", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "LLParser", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Turing", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
