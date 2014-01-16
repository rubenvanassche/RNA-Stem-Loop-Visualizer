/* 
 * LLParserInputOutput.cpp
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

namespace LLP {
void readInput(
    const std::string inputFile,
    std::set<char>& CFGTerminals,         
    std::set<char>& CFGVariables,
    std::multimap<char, SymbolString>& CFGProductions, 
    char& CFGStartsymbol,
    unsigned int& lookahead
    ) {
    std::ifstream fin (inputFile, std::ifstream::in);
   
    if (!fin){
        throw "Error: can't read input file";   
    } 

    bool checks[5];
    try {
        while (!fin.eof()) {
            std::string line;
            std::getline(fin, line);

            if (line == "# Terminals #") {
                checks[0] = true;
                std::getline(fin, line);
                while (line != "") {
                    if (line.length() == 1) CFGTerminals.insert(line[0]);
                    else throw 0;
                    if(DEBUG) std::cout << "Terminal: " << line[0] << std::endl;
                    std::getline(fin, line);
                }
                continue;
            }
            if (line == "# Variables #") {
                checks[1] = true;
                std::getline(fin, line);
                while (line != "") {
                    if (line.length() == 1) CFGVariables.insert(line[0]);
                    else throw 0;
                    if(DEBUG) std::cout << "Variable: " << line[0] << std::endl;
                    std::getline(fin, line);
                }
                continue;
            }
            if (line == "# Startsymbol #") {
                checks[2] = true;
                std::getline(fin, line);
                if (line.length() == 1) CFGStartsymbol = line[0];
                else throw 0;
                if(DEBUG) std::cout << "Startsymbol: " << line[0] << std::endl;
                continue;

            }
            if (line == "# Productions #") {
                checks[3] = true;
                std::getline(fin, line);
                while (line != "") {
                    unsigned int found = line.find_first_of(":");
                    if (found == 1) CFGProductions.insert(std::pair<char, SymbolString>(line[0], line.substr(found+1)));
                    else throw 0;
                    if(DEBUG) std::cout << "Production: " << line.substr(0, found) << " -> " << line.substr(found+1) << std::endl;
                    std::getline(fin, line);
                }
                continue;
            }
            if (line == "# Lookahead #") {
                checks[4] = true;
                std::getline(fin, line);
                unsigned int temp;
                std::stringstream ss (std::stringstream::in | std::stringstream::out);
                ss << line;
                ss >> temp;
                lookahead = temp;
                if(DEBUG) std::cout << "Lookahead: " << lookahead << std::endl;
                continue;
            }
            if (line == "") {
                continue;
            }
            
            std::cout << line << std::endl;
            throw 0;
        }
    }
    catch (...) {
        throw "Error: wrong format";
    }

    for (unsigned int i = 0; i < 5; i++) {
        if (!checks[i]) throw "Error: not all required elements are found";
    }   

    fin.close();
}

void writeOutput(const std::string outputFile, const LLParser& parser) {
    if(DEBUG) std::cout << parser;
    std::ofstream fout(outputFile);
    fout << parser; 
    fout.close();
}

void processInput(std::string inputFile, std::string outputFile, bool output) {
    std::set<char> CFGTerminals;         
    std::set<char> CFGVariables;
    std::multimap<char, SymbolString> CFGProductions;
    char CFGStartsymbol;
    unsigned int lookahead;

    if (output) std::cout << "input: " << inputFile << std::endl;
    if (output) std::cout << "output: " << outputFile << std::endl << std::endl;

    try {
        if (output) std::cout << "[start    ] reading input" << COLOR1 << std::endl;
        readInput(inputFile, CFGTerminals, CFGVariables, CFGProductions, CFGStartsymbol, lookahead);
        if (output) std::cout << RESETCOLOR << "[       ok] reading input" << std::endl;

        if (output) std::cout << "[start    ] generating parse table" << COLOR1 << std::endl;
        LLParser parser (CFGTerminals, CFGVariables, CFGProductions, CFGStartsymbol, lookahead);
        if (output) std::cout << RESETCOLOR << "[       ok] generating parse table" << std::endl;

        if (output) std::cout << "[start    ] writing output" << COLOR1 << std::endl;
        writeOutput(outputFile, parser);
        if (output) std::cout << RESETCOLOR << "[       ok] writing output" << std::endl;
    }
    catch (const char* err){
        std::cerr << err << std::endl;
    }
}
}
