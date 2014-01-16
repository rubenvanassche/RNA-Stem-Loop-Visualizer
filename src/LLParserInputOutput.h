/*
 * LLParserInputOutput.h
 *
 * Copyright (C) 2013 Pieter Lauwers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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
 * Last modified: 2 December 2013
 * By: Pieter Lauwers
 */

#ifndef LLPARSERIO_H
#define LLPARSERIO_H

#include "LLParser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#define RESETCOLOR "\033[0m"
#define COLOR1  "\033[22;37m"

namespace LLP {

/**
 * @brief Converts the given input into the different elements of a grammar.
 *
 * @param inputFile The (relative) path to the inputfile
 * @param CFGTerminals Output: container for the terminals of the CFG
 * @param CFGVariables Output: container for the variables of the CFG
 * @param CFGProductions Output: container for the productions of the CFG
 * @param dimension Output: the dimension of the table, thus the size of the lookahead (k)
 */
void readInput(
    const std::string inputFile,
    std::set<char>& CFGTerminals,         
    std::set<char>& CFGVariables,
    std::multimap<char, SymbolString>& CFGProductions, 
    char& CFGStartsymbol,
    unsigned int& lookahead
    );

/**
 * @brief Writes the table of the given parser to a file.
 *
 * @param outputFile The (relative) path to the outputfile
 * @param parser An LLParser
 */
void writeOutput(const std::string outputFile, const LLParser& parser);

/**
 * @brief Processes an inputfile containing a grammar to an outputfile containing the parsetable.
 *
 * @param inputFile The (relative) path to the inputfile
 * @param outputFile The (relative) path to the outputfile
 * @param output Whether processes has to be reporterd on the standard output
 */
void processInput(std::string inputFile, std::string outputFile, bool output);

}

#endif /*LLPARSERIO_H*/
