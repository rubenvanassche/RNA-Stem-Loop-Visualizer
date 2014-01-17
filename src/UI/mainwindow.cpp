#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../LLParser.h"
#include "../CNF.h"
#include <sstream>
#include <chrono>
#include <ctime>

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
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

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
    bool accepted = false; // Change this to true if the loop is accepted
    std::string visualizerLoop = RNALoop; //""; // Store the loop for visualizing here
    // That's it!
    int startIndex = 0;
    int endIndex = 0;
    int stemSize = 0;
    if(algoType == "CFG"){
        // it's as easy as a raspberry pi
        std::set<char> terminals = {'A', 'G', 'U', 'C'};
        std::set<char> variables = {'p', 'q'};
        // (I'm using lowercased letters to avoid confusing the diff between
        // variables and terminals
        std::multimap<char, SymbolString> productions = {
            {'p', "AqU"},
            {'p', "UqA"},
            {'p', "CqG"},
            {'p', "GqC"},
            {'p', "ApU"},
            {'p', "UpA"},
            {'p', "CpG"},
            {'p', "GpC"},
            {'q', "A"},
            {'q', "U"},
            {'q', "G"},
            {'q', "C"},
            {'q', "Aq"},
            {'q', "Uq"},
            {'q', "Gq"},
            {'q', "Cq"}
        };

        try {
            // let's build a CNF!
            CNF cnf(terminals, variables, productions, 'p');

            accepted = cnf.CYK(RNALoop); // that's it

            if (accepted) {
                std::string sequence = RNALoop;

                while (!sequence.empty()) {
                    char front = sequence.front();
                    char back = sequence.back();

                    if (
                            (front == 'A' && back == 'U')
                            || (front == 'U' && back == 'A')
                            || (front == 'G' && back == 'C')
                            || (front == 'C' && back == 'G')
                            ) {
                        ++stemSize;
                        ++startIndex;

                        sequence = std::string(sequence.begin() + 1, sequence.end() - 1);
                    } else {
                        endIndex = sequence.size() - 1;
                        break;
                    } // end if-else
                } // end while
            } // end if
        } catch (const std::invalid_argument& ia) {
            // display error message here
        } // end try-catch
    }else if(algoType == "LLParser"){
        unsigned int result=0, begin=0, end=0;
        LLP::RNAParser::parse(RNALoop, result, begin, end);
        accepted = (result != 0);
        stemSize = int(result);
        startIndex = int(begin) + stemSize;
        endIndex = int(end) - stemSize;
    }else if(algoType == "PDA"){
    	try{
    		PDA pda("../data/PDARNA1.xml");
    		const std::string originalLoop = RNALoop;

    		int begin = 0;
    		while(true){
    			accepted = pda.process(RNALoop);
    			if(accepted == true){
    				// Stem found so go to next step
    				break;
    			}else{
    				// Stem not found, remove from each side two blocks
    				RNALoop.erase(0,1);
    				RNALoop.erase(RNALoop.size() - 1,1);
    				begin++;
    			}
    		}

    		// The code below is for the visualizer so it can work with the loop, it's off course better then using the PDA but using the PDA is the point of this course
    		// So above the loop is checked with full functionality by the PDA
    		// Beneth this block we just use "normal" code because the pda hasn't the functionality built in to do this

    		int x = 0;

    		if(accepted == true){
    			std::string::iterator it = RNALoop.begin();
    			std::string::reverse_iterator rIt = RNALoop.rbegin();
    			while(true){
    				if(it == RNALoop.end()){
    					break;
    				}

    				if(*it == 'A' and *rIt == 'U'){
    					// do nothing
    				}else if(*it == 'U' and *rIt == 'A'){
    					// do nothing
    				}else if(*it == 'C' and *rIt == 'G'){
    					// do nothing
    				}else if(*it == 'G' and *rIt == 'C'){
    					// do nothing
    				}else{
    					startIndex = x - begin;
    					endIndex = RNALoop.size()  - x;
    					stemSize = endIndex - startIndex + 1;
    					break;
    				}

    				x++;
    				it++;
    				rIt++;
    			}
    		}

    		// Give the original string back
    		RNALoop = originalLoop;
    	}catch(std::exception &e){
    		std::cout << "There went something wrong: " << e.what() << std::endl;
    	    ui->AnalyzeButton->setEnabled(true);
    	    ui->AnalyzeButton->setText("Analyze");
    		return;
    	}
    }else if(algoType == "Turing"){
        try {
            RNAString RNALoopAdv;  //Will contain string with longest possible loop indicated
            TuringPtr tm = generateTM("TMRNA1.xml");
            int subStringSize = RNALoop.size();
            int unusedNucleotides = 0;     //number of nucleotides not in tested substring
            int maxStemSize = 0;
            while (subStringSize >= 4) {  //Min size of stem loop is 4
                if (subStringSize / 2.0 < maxStemSize)  //Impossible to get bigger stem
                    break;
                for (int i = 0; i <= unusedNucleotides; i++) { //n unused nucleotides -> n+1 possible substrings
                    std::tuple<bool, Tape> booltape = tm->processAndGetTape(RNALoop.substr(i, subStringSize));
                    bool newAccepted = std::get<0>(booltape);  //Indicates whether this substring is a stem loop
                    if (newAccepted) {
                        accepted = true;  //Something accepted == whole thing accepted
                        newAccepted = false;   //reset
                        RNAString newRNALoopAdv = RNAString(std::get<1>(booltape));  //generate new RNAString for current substr
                        if (newRNALoopAdv.getStemSize() <= maxStemSize)    //Largest stem so far?
                            break;
                        RNALoopAdv = newRNALoopAdv;   //If largest stem loop, use as result (so far)
                        for (int j = 0; j < i; j++) {         //Add unused nucleotides to RNAString
                            RNALoopAdv.push_front(RNALoop[j]);
                        }
                        for (int j = 0; j < unusedNucleotides - i; j++) {
                            RNALoopAdv.push_back(RNALoop[RNALoop.size() -1 -j]);
                        }
                        maxStemSize = RNALoopAdv.getStemSize();   //properly set max stem size
                        std::cout << RNALoopAdv.getSize() << std::endl;
                    }
                }
                subStringSize--;
                unusedNucleotides++;

            }
            if (accepted) {
                stemSize = RNALoopAdv.getStemSize();
                startIndex = RNALoopAdv.getLoopStartIndex() + stemSize;
                endIndex = RNALoopAdv.getLoopEndIndex() - stemSize + 1;
                std::cout << RNALoopAdv << std::endl;
            }
        }
        catch (std::runtime_error& e) {
            //invalid string input
            std::cout << e.what() << std::endl;
            accepted = false;
        }

    }else{
        QMessageBox::information(NULL, "Problem", "This algorithm for processing the RSL is not (yet avaible).");
    }

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    if(accepted == true){
    	ui->VisualizeButton->setDisabled(false);
        std::stringstream message;
        message << "This loop was accepted." << std::endl;
        message << "elapsed time: " << elapsed_seconds.count() << "s";
    	QMessageBox::information(NULL, "Accepted", message.str().c_str());
    	/*if(algoType == "Turing"){  I commented this out as distinguishing is no longer necessary, delete if sure it's fine this way
    		this->fVisualizerLoop = RNALoopAdv.getString();
    	}else{
    		this->fVisualizerLoop = visualizerLoop;
    	}*/
        this->fVisualizerLoop = visualizerLoop;
        this->fStartIndex = startIndex;
        this->fEndIndex = endIndex;
        this->fStemSize = stemSize;
    }else{
    	QMessageBox::information(NULL, "Not Accepted", "This loop was not accepted.");
    }

    ui->AnalyzeButton->setEnabled(true);
    ui->AnalyzeButton->setText("Analyze");
}

void MainWindow::on_VisualizeButton_clicked(){
    std::stringstream command;
    command << " ./RNA-Stem-Loop-Visualizer '" + this->fVisualizerLoop + "'";
    command << " " << this->fStemSize << " " << this->fStartIndex << " " << this->fEndIndex;
    std::cout << command.str() << std::endl;
	system(command.str().c_str());
}

