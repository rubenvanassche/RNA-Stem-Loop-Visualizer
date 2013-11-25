/*
 * LLParser.cpp
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
 * Last modified: 25 November 2013
 * By: Pieter Lauwers
 */

 #include "LLParser.h"
 #include <sstream>
 #include <iostream>

// Implementation of the LLParser methods:
 LLParser::LLParser(
        const std::set<char>& CFGTerminals, 
        const std::set<char>& CFGVariables, 
        const std::multimap<char, SymbolString>& CFGProductions, 
        const char& CFGStartsymbol, 
        const unsigned int lookahead
        ) : parseTable(CFGTerminals, CFGVariables, CFGProductions, 2),  // TEMP: manual dimension
            startsymbol(CFGStartsymbol) {     

}

/*
LLParser::LLParser(const CFG& grammar, unsigned int lookahead) {        // TODO: need getters from CFG

}
*/

bool LLParser::process(const std::string& input) const {
    return false;
}

LLParser::~LLParser() {
    // nothing to destroy
}


// Implementation of the LLTable methods:
LLTable::LLTable(
        const std::set<char>& CFGTerminals,            
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions, 
        const unsigned int dimension
        ) : dimension(dimension), table(generateTable(CFGTerminals, CFGVariables, CFGProductions, dimension)){

}

/*
LLTable::LLTable(const CFG& grammar, unsigned int dimension) {          // TODO: need getters from CFG

}
*/

SymbolString process(const char& topStack, const SymbolString& remainingInput) {
    return SymbolString();
}

LLTable::~LLTable() {
    // nothing to destroy
}

std::map<char, std::map<SymbolString, SymbolString> > LLTable::generateTable(
        const std::set<char>& CFGTerminals,            
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions, 
        const unsigned int dimension
        ) {
    std::map<char, std::map<SymbolString, SymbolString> > table;
    const std::vector<SymbolString> terminalCombinations = getTerminalCombinations(CFGTerminals, dimension);

    // create a "row" for every variable
    for (auto it = CFGVariables.begin(); it != CFGVariables.end(); it++) {
        // for each variable: create an entry for every combination for non-terminals that don't lead to "error"
        table.insert(std::pair<char, std::map<SymbolString, SymbolString> > (*it, generateRow(*it, terminalCombinations, CFGProductions)));
    }

    return table;
}

std::map<SymbolString, SymbolString> LLTable::generateRow(
        const char variable,
        const std::vector<SymbolString>& terminalCombinations,           
        const std::multimap<char, SymbolString>& CFGProductions
        ) {
    std::map<SymbolString, SymbolString> result;

    for (auto terminals_it = terminalCombinations.begin(); terminals_it != terminalCombinations.end(); terminals_it++) {
        // find the matching production rule
        for (auto production_it = CFGProductions.lower_bound(variable); production_it != CFGProductions.upper_bound(variable); production_it++) {
            if (std::get<1>(*production_it).compare(0, (*terminals_it).length(), *terminals_it) == 0)
            {
                // The current production rule matches the lookahead symbols
                result.emplace(*terminals_it, std::get<1>(*production_it));
                break;                                                  // TODO: wat if match occures multiple time? Increase dimension of the table?
            }
        }   
    }

    return result;
}

std::vector<SymbolString> LLTable::getTerminalCombinations(
        const std::set<char>& CFGTerminals,
        const unsigned int dimension
        ) {
    std::vector<SymbolString> terminals;
    for (auto it = CFGTerminals.begin(); it != CFGTerminals.end(); it++)
    {
        std::stringstream ss;
        ss << *it;
        terminals.push_back(ss.str());
    }
    terminals.push_back("\t"); // empty char

    std::vector<SymbolString> result;
    enumerate(result, terminals, dimension);

                                                                        // TODO: filter result, \t only as last element

    return result;
}

void LLTable::enumerate(std::vector<SymbolString>& result, std::vector<SymbolString>& terminals, unsigned int length) { 
    if (result.empty()) {
        for (unsigned int i = 0; i < terminals.size(); i++) {
            result.push_back(terminals[i]);
        }
    }
    else {
        std::vector<SymbolString> temp;
        for (unsigned int j = result.size(); j != 0; j--) {
            SymbolString base = result.back();
            result.pop_back();

            for (unsigned int k = 0; k < terminals.size(); k++) {
                temp.push_back(base + terminals[k]);
            }
        }
        for (unsigned int l = 0; l < temp.size(); l++)
        {
            result.push_back(temp[l]);
        }
    }
    if (result.back().length() == length) { return; }

    enumerate(result, terminals, length);
}