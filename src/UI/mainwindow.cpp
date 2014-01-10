#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_AnalyzeButton_clicked(){
    QString loop = ui->Input->toPlainText();
    if(loop.isEmpty()){
        QMessageBox::information(NULL, "Problem", "Please provide A RSL.");
    }

    ui->AnalyzeButton->setDisabled(true);
    ui->AnalyzeButton->setText("Working ...");
    ui->OpenAlgoWindowButton->setDisabled(true);

    const QString algoType =  ui->AnalyzeTypeSelect->currentText();
    if(algoType == "PDA"){

    }else if(algoType == "LLParser"){

    }else if(algoType == "Turing"){

    }else{
        QMessageBox::information(NULL, "Problem", "This algorithm for processing the RSL is not (yet avaible).");
    }

    ui->AnalyzeButton->setEnabled(true);
    ui->AnalyzeButton->setText("Analyze");
    ui->OpenAlgoWindowButton->setEnabled(true);
}



void MainWindow::on_OpenAlgoWindowButton_clicked()
{
    AlgoTest* algow = new AlgoTest(); // Be sure to destroy you window somewhere
    algow->show();
}
