/*
 * LLParser.h
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
 * Last modified: 15 November 2013
 * By: Pieter Lauwers
 */

#ifndef LLPARSER_H
#define LLPARSER_H

#include <stack>
#include "GFG.h"   

/**
 * @brief Class representing an LL Parse Table
 */

class LLTable {
public:
     /**
     * @brief Constructor, constructs an LL Parse Table from the given elements of a context free grammar
     *
     * @param CFGTerminals A set containing the terminals of the CFG
     * @param CFGVariables A set containing the variables of the CFG
     * @param CFGProductions A multimap that maps a symbol to an symbolString
     * @param dimension The dimension of the table, thus the size of the lookahead (k)
     *
     * @exception invalid_argument Throws this exception when the Table can't be contructed
     */
    LLTable(
        const std::set<char>& CFGTerminals,            
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions, 
        unsigned int dimension
        );

    /**
     * @brief Constructor, constructs an LL Parse Table for the given context free grammar
     *
     * @param grammar A context free grammar as a base for this LL Parser
     * @param dimension The dimension of the table, thus the size of the lookahead (k)
     *
     * @exception invalid_argument Throws this exception when the Table can't be contructed
     */
    LLTable(
        const CFG& grammar, 
        unsigned int dimension
        );

    /**
     * @brief Process one input symbol of the remaining input string.
     *
     * @param topStack The variable at the top of the stack
     * @param remainingInput The remaining part of the input string
     *
     * @pre topStack is an element of CFGVariables
     *
     * @exception runtime_error Throws this exception when the remaining input string results in an error
     *
     * @return The right side of the used production rule.
     */
    SymbolString process(const char& topStack, const SymbolString& remainingInput) const;

    /**
     * @brief Destructor
     */
    virtual ~LLTable();

private:
    /**
     * @brief Generates the parse table.
     *
     * @param CFGTerminals A set containing the terminals of the CFG
     * @param CFGVariables A set containing the variables of the CFG
     * @param CFGProductions A multimap that maps a symbol to an symbolString
     * @param dimension The dimension of the table, thus the size of the lookahead (k)
     *
     * @return A bool telling if the Parser accepted
     */
    static std::map<SymbolString, SymbolString> generateTable(
        const std::set<char>& CFGTerminals,            
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions, 
        unsigned int dimension
        );

    const unsigned int dimension;

    /**
    * Representation of the Parse Table by a map.
    * Key:   left side of the production rule (variable) + lookahead symbols (terminals)
    * Value: right side of the production rule
    *
    * If the wanted key doesn't occur this means 'error'. 
    */
    std::map<SymbolString, SymbolString> table;
};

/**
 * @brief Class representing an LL Parser
 */

class LLParser {
public:
     /**
     * @brief Constructor, constructs an LL Parser from the given elements of a context free grammar
     *
     * @param CFGTerminals A set containing the terminals of the CFG
     * @param CFGVariables A set containing the variables of the CFG
     * @param CFGProductions A multimap that maps a symbol to an symbolString
     * @param CFGStartsymbol The startsymbol for the CFG
     * @param lookahead The size of the lookahead (k)
     *
     * @exception invalid_argument Throws this exception when the Parser can't be contructed
     */
    LLParser(
        const std::set<char>& CFGTerminals,             // need getters and setters, inheritance of CFG stores redundant information (productions, variables, terminals)
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions, 
        const char& CFGStartsymbol,
        unsigned int lookahead
        );

    /**
     * @brief Constructor, constructs an LL Parser for the given context free grammar
     *
     * @param grammar A context free grammar as a base for this LL Parser
     * @param lookahead The size of the lookahead (k)
     *
     * @exception invalid_argument Throws this exception when the Parser can't be contructed
     */
    LLParser(
        const CFG& grammar, 
        unsigned int lookahead
        );

    /**
     * @brief Process an input string through the LL Parser
     *
     * @param input The string to be processed by the LL Parser
     *
     * @return A bool telling if the Parser accepted
     */
    bool process(std::string& input) const;

    /**
     * @brief Destructor
     */
    virtual ~LLParser();

private:
    const char startsymbol;
    const LLTable parseTable;
};

#endif /*LLPARSER_H*/
