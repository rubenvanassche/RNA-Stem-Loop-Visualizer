#include "algotest.h"
#include "ui_algotest.h"

#include <QFileDialog>
#include <QMessageBox>

AlgoTest::AlgoTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlgoTest)
{
    ui->setupUi(this);
}

AlgoTest::~AlgoTest()
{
    delete ui;
}

void AlgoTest::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.xml)"));
    ui->AcceptedLabel->setText("Working ...");

    const QString algoType =  ui->AlgoTypeSelect->currentText();
    if(algoType == "PDA"){

    }else if(algoType == "LLParser"){

    }else if(algoType == "Turing"){

    }else if(algoType == "CFG"){

    }else if(algoType == "CNF"){

    }else{
        QMessageBox::information(NULL, "Problem", "This algorithm for processing the RSL is not (yet avaible).");
    }
}
