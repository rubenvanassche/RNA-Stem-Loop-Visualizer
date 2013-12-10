#include "PDA.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Please provide the name of an xml file describing a PDA as command line argument!" << std::endl;
        return 0;
    }

    PDA *pda;

    try {
        PDA pdareal(argv[1]);
        pda = &pdareal;
    }
    catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    while (true) {
        std::cout << "Please enter a string to be processed by PDA (type 'quit' to quit program): " << std::endl;;
        std::string input;
        std::getline(std::cin, input);
        bool answer;
        if (input == "quit")
            return 0;
        try {
            answer = pda->process(input);
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
