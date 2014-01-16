#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../LLParser.h"

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
    int startIndex = 0;
    int endIndex = 0;
    int stemSize = 0;
    if(algoType == "CFG"){


    }else if(algoType == "LLParser"){
        unsigned int stemsize = LLP::RNAParser::parse(RNALoop);
        accepted = (stemsize != 0);
    }else if(algoType == "Turing"){
        try {
            TuringPtr tm = generateTM("TMRNA1.xml");
            std::tuple<bool, Tape> booltape = tm->processAndGetTape(RNALoop);
            accepted = std::get<0>(booltape);
            if (accepted) {
                RNALoopAdv = RNAString(std::get<1>(booltape));
                std::cout << RNALoopAdv << std::endl;

            }
            if (!accepted) { //Check if substrings are stem loops
                int subStringSize = RNALoop.size();
                int unusedNucleotides = 0;     //number of nucleotides not in tested substring
                while (subStringSize > 4) {  //Min size of stem loop is 4
                    if (accepted)
                        break;
                    subStringSize--;
                    unusedNucleotides++;
                    for (int i = 0; i <= (unusedNucleotides + 1); i++) {
                        std::tuple<bool, Tape> booltape = tm->processAndGetTape(RNALoop.substr(i, subStringSize));
                        accepted = std::get<0>(booltape);
                        if (accepted) {
                            RNALoopAdv = RNAString(std::get<1>(booltape));
                            for (int j = 0; j < i; j++) {
                                RNALoopAdv.push_front(RNALoop[j]);
                            }
                            for (int j = 0; j < unusedNucleotides - i - 1; j++) {
                                RNALoopAdv.push_back(RNALoop[RNALoop.size() -1 -j]);
                            }
                            break;
                        }
                    }
                }
            }
            if (accepted) {
                startIndex = RNALoopAdv.getLoopStartIndex();
                endIndex = RNALoopAdv.getLoopEndIndex();
                stemSize = RNALoopAdv.getStemSize();
                std::cout << RNALoopAdv << std::endl;
            }
        }
        catch (std::runtime_error& e) {
            //invalid string input
            std::cout << e.what() << std::endl;
            RNALoopAdv = RNAString();
            accepted = false;
        }

    }else{
        QMessageBox::information(NULL, "Problem", "This algorithm for processing the RSL is not (yet avaible).");
    }

    if(accepted == true){
    	ui->VisualizeButton->setDisabled(false);
    	QMessageBox::information(NULL, "Accepted", "This loop was accepted.");
    	if(algoType == "Turing"){
    		this->fVisualizerLoop = RNALoopAdv.getString();
    	}else{
    		this->fVisualizerLoop = visualizerLoop;
    	}
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

