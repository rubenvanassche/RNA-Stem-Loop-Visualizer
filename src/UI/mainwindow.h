#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <stdlib.h>
#include <QMessageBox>
#include <iostream>
#include "../Turing.h"
#include "../RNAString.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_AnalyzeButton_clicked();
    void on_VisualizeButton_clicked();

private:
    Ui::MainWindow *ui;
    std::string fVisualizerLoop = "";
};

#endif // MAINWINDOW_H
