/* 
 * runLLParserInput.cpp
 *
 * Copyright (C) 2013 Pieter Lauwers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Last modified: 9 December 2013
 * By: Pieter Lauwers
 */

#include "LLParserInputOutput.h"

using namespace LLP;

void test1() {
    // Tests enumeration of terminals
    std::cout << "Enumeration of terminals:" << std::endl;
    std::vector<SymbolString> result;
    std::vector<SymbolString> symbols;
    symbols.push_back("a");
    symbols.push_back("b");
    symbols.push_back(" ");

    LLTable::enumerate(result, symbols, 3);

    for (unsigned int i = 0; i != result.size(); i++) {
        std::cout << result[i] << std::endl;
    }
}

void test2() {
    // Tests the generating of the parse table
    std::cout << "Generating parse table:" << std::endl;
    std::set<char> CFGTerminals;
    CFGTerminals.insert('x');
    CFGTerminals.insert('y');
    CFGTerminals.insert('z');

    std::set<char> CFGVariables;
    CFGVariables.insert('S');

    std::multimap<char, SymbolString> CFGProductions;
    CFGProductions.insert(std::pair<char, SymbolString>('S', "xSz"));
    CFGProductions.insert(std::pair<char, SymbolString>('S', "ySz"));
    CFGProductions.insert(std::pair<char, SymbolString>('S', EPSILON));

    char CFGStartsymbol = 'S'; 

    unsigned int lookahead = 1;

    LLParser parser = LLParser(CFGTerminals, CFGVariables, CFGProductions, CFGStartsymbol, lookahead);
    std::cout << parser;
}

int main(int argc, char const *argv[]) {
    if (argc <= 1) {
        std::cout << "* Testing LLParser:" << std::endl;

        // Tests:
        test1();
        test2();

        std::cout << "* Done testing." << std::endl;
    }
    else if (argc == 2 or argc == 3) {
        std::string inputFile = argv[1];
        std::string outputFile;
        if (argc == 2) {
            unsigned int found = inputFile.find_last_of("/\\");
            outputFile = inputFile.substr(0, found + 1);
            outputFile += "output.txt";
        }
        else {
            outputFile = argv[2];
        }

        processInput(inputFile, outputFile, true);
    }
    return 0;
}