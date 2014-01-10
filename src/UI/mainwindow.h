#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "algotest.h"

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

    void on_OpenAlgoWindowButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
