#include "Turing.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Please provide the name of an xml file describing a turing machine as command line argument!" << std::endl;
        return 0;
    }
    TuringPtr TM;

    try {
        TM = generateTM(argv[1]);
    }
    catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    while (true) {
        std::cout << "Please enter a string to be processed by TM (type 'quit' to quit program): " << std::endl;;
        std::string input;
        std::getline(std::cin, input);
        bool answer;
        if (input == "quit")
            return 0;
        try {
            answer = TM->process(input);
        }
        catch (std::runtime_error& e) {
            std::cout << e.what() << std::endl;
            continue;
        }
        if (answer)
            std::cout << "String accepted!" << std::endl;
        else
            std::cout << "String NOT accepted!" << std::endl;
    }
}
