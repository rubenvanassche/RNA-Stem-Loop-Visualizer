#ifndef ALGOTEST_H
#define ALGOTEST_H

#include <QDialog>

namespace Ui {
class AlgoTest;
}

class AlgoTest : public QDialog
{
    Q_OBJECT
    
public:
    explicit AlgoTest(QWidget *parent = 0);
    ~AlgoTest();


private slots:
    void on_pushButton_clicked();

private:
    Ui::AlgoTest *ui;
};

#endif // ALGOTEST_H
