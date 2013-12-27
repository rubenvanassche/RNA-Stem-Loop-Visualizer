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
 #include <algorithm> 
 #include <stack>

////////////////
#include <queue>
#include <ostream>

template <class Container, class Stream>
Stream& printOneValueContainer
    (Stream& outputstream, const Container& container)
{
    typename Container::const_iterator beg = container.begin();

    outputstream << "[";

    while(beg != container.end())
    {
        outputstream << " " << *beg++;
    }

    outputstream << " ]";

    return outputstream;
}

template < class Type, class Container >
const Container& container
    (const std::stack<Type, Container>& stack)
{
    struct HackedStack : private std::stack<Type, Container>
    {
        static const Container& container
            (const std::stack<Type, Container>& stack)
        {
            return stack.*&HackedStack::c;
        }
    };

    return HackedStack::container(stack);
}

template < class Type, class Container >
const Container& container
    (const std::queue<Type, Container>& queue)
{
    struct HackedQueue : private std::queue<Type, Container>
    {
        static const Container& container
            (const std::queue<Type, Container>& queue)
        {
            return queue.*&HackedQueue::c;
        }
    };

    return HackedQueue::container(queue);
}

template
    < class Type
    , template <class Typ, class Container = std::deque<Type> > class Adapter
    , class Stream
    >
Stream& operator<<
    (Stream& outputstream, const Adapter<Type>& adapter)
{
    return printOneValueContainer(outputstream, container(adapter));
}
//////////////////////////

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
    std::stack<char> stack;
    stack.emplace(startsymbol);
    SymbolString remainingInput(input);

    while (not stack.empty()) {
        if(DEBUG) std::cout << "remaining input: " << remainingInput << '\t';
        if(DEBUG) std::cout << "stack: " << stack << " with top = " << stack.top() << std::endl;

        if (stack.top() == EPSILON[0]) stack.pop();
        else if (not isVariable(stack.top())) {
            if (not remainingInput.empty() and stack.top() == remainingInput[0]) {
                stack.pop();
                remainingInput.erase(0, 1);
            }
            else return false;
        }
        else {
            if (not isVariable(stack.top())) return false;
            SymbolString rule;
            try {
                rule = parseTable.process(stack.top(), remainingInput);
            }
            catch (const std::out_of_range& oor) {
                return false; // hit 'error' in the table
            }
            stack.pop();

            // push the rule backwards down to the stack
            for (int i = rule.length() - 1; i >= 0; i--) stack.push(rule[i]);
        }
    }
    return remainingInput.empty();
}

LLParser::~LLParser() {
    // nothing to destroy
}

std::ostream& operator<<(std::ostream& stream, const LLParser& obj) {
    stream << "Start symbol: " << obj.startsymbol << std::endl;
    stream << "Parse table: " << std::endl << obj.parseTable.toString(obj.CFGTerminals, obj.CFGVariables) << std::endl;

    return stream;
}

bool LLParser::isVariable(char e) const {
    for (auto it = CFGVariables.begin(); it != CFGVariables.end(); it++) {
        if (e == *it) return true;
    }

    return false;
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

SymbolString LLTable::process(const char& topStack, const SymbolString& remainingInput) const {
    // calculate the length of the lookahead = min(k, remainingInput.length())
    unsigned int length = remainingInput.length() < dimension ? remainingInput.length() : dimension;
    SymbolString lookahead = remainingInput.substr(0, length);

    // add space if lookahead is smaller than k
    while (lookahead.length() < dimension) lookahead += " ";

    return get_transition(topStack, lookahead);
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
        if(DEBUG) std::cout << *it << std::endl;
        table.insert(std::pair<char, std::map<SymbolString, SymbolString> > (*it, generateRow(*it, terminalCombinations, CFGVariables, CFGProductions)));
    }

    return table;
}

std::map<SymbolString, SymbolString> LLTable::generateRow(
        const char variable,
        const std::vector<SymbolString>& terminalCombinations,
        const std::set<char>& CFGVariables,           
        const std::multimap<char, SymbolString>& CFGProductions
        ) {
    // find all variables that can be reached with one or more direct transitions (of the form: X -> Y)
    std::set<char> directVariables;
    directVariables.insert(variable);
    getDirectVariables(variable, CFGVariables, CFGProductions, directVariables);

    // generate the row for the given variable
    std::map<SymbolString, SymbolString> result;

    // iterate over all combinations of k terminals (the collumns) and then find the matching production rule to fill the cell with
    for (auto terminals_it = terminalCombinations.begin(); terminals_it != terminalCombinations.end(); terminals_it++) {
        if(DEBUG) std::cout << "\t" << *terminals_it << " (" << (*terminals_it).length() << ")" << std::endl;

        SymbolString cell = findRule(CFGVariables, CFGProductions, variable, *terminals_it);
        for (auto directVar_it = directVariables.begin(); cell == "" and directVar_it != directVariables.end(); directVar_it++) {
            //std::cout << *directVar_it << std::endl;
            if (findRule(CFGVariables, CFGProductions, *directVar_it, *terminals_it) != ""){ 
                cell = *directVar_it;
                if(DEBUG) std::cout << "\t\t" << cell << " (direct)" << std::endl;
                break;
            }
        }
        
        if (cell != "") result.emplace(*terminals_it, cell);
        else if(DEBUG) std::cout << "\t\t" << "error" << std::endl;  
    }

    return result;
}

SymbolString LLTable::findRule(
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions,
        const char variable,
        const SymbolString& terminalCombination
        ) {
    // determines the range of all productions that have 'variable' as left side of the productionrule
    auto productionsStart = CFGProductions.lower_bound(variable);
    auto productionsEnd = CFGProductions.upper_bound(variable);

    // indicates whether their is an epsilon transition for 'variable'
    bool epsilon_transition = false;

    for (auto production_it = productionsStart; production_it != productionsEnd and !epsilon_transition; production_it++) {
        epsilon_transition = std::get<1>(*production_it).compare(EPSILON) == 0; // 0 means they compare equal
    }

    SymbolString result;

    for (unsigned int i = 0; i != terminalCombination.length(); i++) {
        for (auto production_it = productionsStart; production_it != productionsEnd; production_it++) {
            SymbolString rule = std::get<1>(*production_it);
            if(DEBUG) std::cout << "\t\t" << rule << std::endl;
            if (i > 0) {
                // check if productionrule contains a variable
                bool var = false;
                for (unsigned int j = 0; !var and j != rule.length(); j++) {
                    var = (CFGVariables.find(rule[j]) != CFGVariables.end());
                }
                if (!var) break;
            }
            if (std::get<1>(*production_it).compare(0, terminalCombination.length() - i, terminalCombination.substr(0, terminalCombination.length() - i)) == 0)
            {
                // The current production rule matches the lookahead symbols
                return std::get<1>(*production_it);   // TODO: wat if match occures multiple time? Increase dimension of the table?
            }
        } 
    }
    if (epsilon_transition) return EPSILON;
    return "";                                       
}

void LLTable::getDirectVariables(
        const char variable,
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions,
        std::set<char>& directVariables
        ) {
    // find all variables that can be reached with one or more direct transitions (of the form: X -> Y)
    for (auto production_it = CFGProductions.lower_bound(variable); production_it != CFGProductions.upper_bound(variable); production_it++) {
        // check if right side contains only one element
        if (std::get<1>(*production_it).length() == 1) {
            char e = std::get<1>(*production_it)[0]; 
            if (
                // check if element is a variable
                CFGVariables.find(e) != CFGVariables.end() and
                // check if the element doesn't already occur in 'directVariables'
                directVariables.find(e) == directVariables.end()
            ) {
                directVariables.insert(e);
                getDirectVariables(e, CFGVariables, CFGProductions, directVariables);
            }
        }
    }
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

SymbolString LLTable::get_transition(const char& variable, const SymbolString& lookahead) const {
    return (table.at(variable)).at(lookahead);
}
};