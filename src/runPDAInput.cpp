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

    while(true){
		char option = -1;

		while(option == -1){
			std::cout << "What do you want to do?" << std::endl;
			std::cout << "[0] Check if a string is accepted by the PDA." << std::endl;
			std::cout << "[1] Write the PDA to a dotfile" << std::endl;

			std::cin >> option;
			if(option == '0'){
				option = 0;
				break;
			}else if(option == '1'){
				option = 1;
				break;
			}else{
				option = -1;
				std::cout << "Try again" << std::endl;
			}
		}

		if(option == 0){
			// String reading
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
		}else if(option == 1){
			std::cout << "Please enter the name for the dotfile (type 'quit' to quit program):" << std::endl;
			std::string input;
			std::cin >> input;

			pda->toDotFile(input);
		}
    }
    delete pda;
    return 0;
}
