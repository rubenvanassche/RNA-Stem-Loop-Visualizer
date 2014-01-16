#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->VisualizeButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_AnalyzeButton_clicked(){
    ui->VisualizeButton->setDisabled(true);
    ui->AnalyzeButton->setDisabled(true);
    ui->AnalyzeButton->setText("Working ...");
    this->fVisualizerLoop = "";

    QString loop = ui->Input->toPlainText();
    if(loop.isEmpty()){
        QMessageBox::information(NULL, "Problem", "Please provide A RSL.");

        ui->AnalyzeButton->setEnabled(true);
        ui->AnalyzeButton->setText("Analyze");
        return;
    }

    const QString algoType =  ui->AnalyzeTypeSelect->currentText();

    std::string RNALoop = loop.toStdString(); // Work with this to start your algorithm
    bool accepted = true; // Change this to true if the loop is accepted
    std::string visualizerLoop = ""; // Store the loop for visualizing here
    // That's it!
    RNAString RNALoopAdv;
    if(algoType == "CFG"){


    }else if(algoType == "LLParser"){

    }else if(algoType == "Turing"){
        TuringPtr tm = generateTM("TMRNA1.xml");
        std::tuple<bool, Tape> booltape = tm->processAndGetTape(RNALoop);
        RNALoopAdv = RNAString(std::get<1>(booltape));
        accepted = std::get<0>(booltape);

    }else{
        QMessageBox::information(NULL, "Problem", "This algorithm for processing the RSL is not (yet avaible).");
    }

    if(accepted == true){
    	ui->VisualizeButton->setDisabled(false);
    	QMessageBox::information(NULL, "Accepted", "This loop was accepted.");
    	this->fVisualizerLoop = visualizerLoop;
    }else{
    	QMessageBox::information(NULL, "Not Accepted", "This loop was not accepted.");
    }

    ui->AnalyzeButton->setEnabled(true);
    ui->AnalyzeButton->setText("Analyze");
}

void MainWindow::on_VisualizeButton_clicked(){
	std::string command = " ./RNA-Stem-Loop-Visualizer '" + this->fVisualizerLoop + "'";
	system(command.c_str());
}

