#include "PDA.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Please provide the name of an xml file describing a PDA as command line argument!" << std::endl;
        return 0;
    }

    PDA *pda;

    try {
        pda = new PDA(argv[1]);
    }
    catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    while (true) {
        std::cout << "Please enter a string to be processed by PDA (type 'quit' to quit program): " << std::endl;;
        std::string input;
        std::cin >> input;
        bool answer;
        if (input == "quit"){
        	delete pda;
            return 0;
        }

        try {
            answer = pda->process(input);
        }
        catch (std::runtime_error& e) {
            std::cout << e.what() << std::endl;
            continue;
        }
        if (answer)
            std::cout << "String " << input << " accepted!" << std::endl;
        else
            std::cout << "String " << input << " NOT accepted!" << std::endl;
    }
    delete pda;
    return 0;
}
