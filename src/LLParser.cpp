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
 * Last modified: 2 December 2013
 * By: Pieter Lauwers
 */

 #include "LLParser.h"
 #include <sstream>
 #include <iostream>

namespace LLP {
    
// Implementation of the LLParser methods:
LLParser::LLParser(
        const std::set<char>& CFGTerminals, 
        const std::set<char>& CFGVariables, 
        const std::multimap<char, SymbolString>& CFGProductions, 
        const char& CFGStartsymbol, 
        const unsigned int lookahead
        ) : parseTable(CFGTerminals, CFGVariables, CFGProductions, lookahead),      
            startsymbol(CFGStartsymbol),
            CFGTerminals(CFGTerminals),
            CFGVariables(CFGVariables) {     

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

std::ostream& operator<<(std::ostream& stream, const LLParser& obj) {
    stream << "Start symbol: " << obj.startsymbol << std::endl;
    stream << "Parse table: " << std::endl << obj.parseTable.toString(obj.CFGTerminals, obj.CFGVariables) << std::endl;

    return stream;
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
        if(DEBUG) std::cout << *terminals_it << " (" << (*terminals_it).length() << ")" << std::endl;
        // find the matching production rule
        bool epsilon_transition;
        for (auto production_it = CFGProductions.lower_bound(variable); production_it != CFGProductions.upper_bound(variable); production_it++) {
            if(DEBUG) std::cout << "\t" << std::get<1>(*production_it) << std::endl;
            epsilon_transition = false;
            if (std::get<1>(*production_it).compare(0, (*terminals_it).length(), *terminals_it) == 0)
            {
                // The current production rule matches the lookahead symbols
                result.emplace(*terminals_it, std::get<1>(*production_it));
                break;                                                  // TODO: wat if match occures multiple time? Increase dimension of the table?
            }
            if (std::get<1>(*production_it).compare(EPSILON) == 0)
            {
                epsilon_transition = true;
            }
            if (epsilon_transition and std::next(production_it) == CFGProductions.upper_bound(variable))
            {
                // The current production rule matches the lookahead symbols
                result.emplace(*terminals_it, EPSILON);
                break;                                                  
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
    terminals.push_back(EOS); // empty char

    std::vector<SymbolString> result;
    enumerate(result, terminals, dimension);

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

            if (base.back() == EOS.back()) {
                temp.push_back(base + EOS);
            }
            else {
                for (unsigned int k = 0; k < terminals.size(); k++) {
                    temp.push_back(base + terminals[k]);
                }
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

std::string LLTable::toString(const std::set<char>& CFGTerminals, const std::set<char>& CFGVariables) const {
    std::stringstream stream;
    const char H = '-';
    const char V = '|';
    const char C = '+';

    std::vector<SymbolString> terminalCombinations = getTerminalCombinations(CFGTerminals, dimension);

    // print heading
    for (auto head = terminalCombinations.begin(); head != terminalCombinations.end(); head++) {
        stream << '\t' << V << " " << *head;
    }
    unsigned int table_length = stream.str().length();

    stream << std::endl << H;
    for (unsigned int i = 0; i < table_length; i += 5) {
        stream << H << H << H << H << H << H << H << C;
    }
    stream << H << H << H << H << H << H << std::endl;

    // print rows
    for (auto variable = CFGVariables.begin(); variable != CFGVariables.end(); variable++) {
        stream << *variable;

        for (auto head = terminalCombinations.begin(); head != terminalCombinations.end(); head++) {
            stream << '\t' << V << ' ';
            try {
                stream << get_transition(*variable, *head);
            }
            catch (const std::out_of_range& oor) {
                stream << "error";
            }
        }
        stream << std::endl;
    }

    return stream.str();
}

SymbolString LLTable::get_transition(char variable, SymbolString lookahead) const {
    return (table.at(variable)).at(lookahead);
}
};