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
 * Last modified: 2 December 2013
 * By: Pieter Lauwers
 */

#ifndef LLPARSER_H
#define LLPARSER_H

#include "CFG.h"
#include <stack>
#include <vector>
#include <iostream>   

#define DEBUG false

namespace LLP {

/**
 * @brief Symbol for representing the end of string.
 */
const std::string EOS = " ";

/**
 * @brief Symbol for representing epsilon.
 */
const std::string EPSILON = "e";

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
     * @param CFGProductions A multimap that maps a variable to an symbolString
     * @param dimension The dimension of the table, thus the size of the lookahead (k)
     *
     * @exception invalid_argument Throws this exception when the Table can't be contructed
     */
    LLTable(
        const std::set<char>& CFGTerminals,            
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions, 
        const unsigned int dimension
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
        const unsigned int dimension
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

    /**
     * @brief Returns a string representation of the parse table.
     *
     * @param CFGTerminals A set containing the terminals of the CFG
     * @param CFGVariables A set containing the variables of the CFG
     *
     * @return String representation.
     */
    std::string toString(const std::set<char>& CFGTerminals, const std::set<char>& CFGVariables) const;

private:
    /**
     * @brief Generates the parse table.
     *
     * @param CFGTerminals A set containing the terminals of the CFG
     * @param CFGVariables A set containing the variables of the CFG
     * @param CFGProductions A multimap that maps a variable to an symbolString
     * @param dimension The dimension of the table, thus the size of the lookahead (k)
     *
     * @return The generated parse table.
     */
    static std::map<char, std::map<SymbolString, SymbolString> > generateTable(
        const std::set<char>& CFGTerminals,            
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions, 
        const unsigned int dimension
        );

    /**
     * @brief Generates a row for the parse table with the given variable as head of the row.
     *
     * @param variable The variable for this row
     * @param terminalCombinations A vector with all combinations of the terminals of the CFG
     * @param CFGVariables A set containing the variables of the CFG
     * @param CFGProductions A multimap that maps a variable to an symbolString
     *
     * @return The generated row.
     */
    static std::map<SymbolString, SymbolString> generateRow(
        const char variable,
        const std::vector<SymbolString>& terminalCombinations, 
        const std::set<char>& CFGVariables,          
        const std::multimap<char, SymbolString>& CFGProductions 
        );

    /**
     * @brief Returns the matching production rule to fill the cell
     *        described by the head of the row ('variable') 
     *        and the head of the collumn ('terminalCombination').
     *
     * @param CFGVariables A set containing the variables of the CFG
     * @param CFGProductions A multimap that maps a variable to an symbolString
     * @param variable Head of the row
     * @param terminalCombination Head of the collumn
     *
     * @return The generated row.
     */
    static SymbolString findRule(
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions,
        const char variable,
        const SymbolString& terminalCombination
        );

    /**
     * @brief Searches for all variables that can be reached with one or more direct transitions (of the form: X -> Y).
     *
     * @param variable Start variable
     * @param CFGVariables A set containing the variables of the CFG
     * @param CFGProductions A multimap that maps a variable to an symbolString
     * @param directVariables A set of already found direct variables.
     *
     * @return The generated row.
     */
    static void getDirectVariables(
        const char variable,
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions,
        std::set<char>& directVariables
        );

    /**
     * @brief Enumerates all possible combinations of the given terminals 
     *
     * @param result Container for all combinations
     * @param terminals All terminals that can be used in a combination
     * @param length The desired length of the combinations
     */
    static void enumerate(std::vector<SymbolString>& result, std::vector<SymbolString>& terminals, unsigned int length);

    /**
     * @brief Generates a vector with all combinations of the given terminals.
     *        The string length is determined by the given dimension.
     *
     * @param CFGTerminals A set containing the terminals of the CFG
     * @param dimension The dimension of the table, thus the size of the lookahead (k)
     *
     * @return A vector with the combinations of terminals.
     */
    static std::vector<SymbolString> getTerminalCombinations(
        const std::set<char>& CFGTerminals,
        const unsigned int dimension
        );

    /**
     * @brief Returns the right side of the transition that has to be used.
     *
     * @param variable The variable at the left side of the transition
     * @param lookahead The lookahead symbols
     *
     * @return A vector with the combinations of terminals.
     */
    SymbolString get_transition(const char& variable, const SymbolString& lookahead) const;

    const unsigned int dimension;

    /**
    * Representation of the Parse Table by a map.
    * Key 1: left side of the production rule (variable)
    * Key 2: lookahead symbols (terminals)
    * Value: right side of the production rule
    *
    * If the wanted key doesn't occur this means 'error'. 
    */
    const std::map<char, std::map<SymbolString, SymbolString> > table;
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
     * @param CFGProductions A multimap that maps a variable to an symbolString
     * @param CFGStartsymbol The startsymbol for the CFG
     * @param lookahead The size of the lookahead (k)
     *
     * @exception invalid_argument Throws this exception when the Parser can't be contructed
     */
    LLParser(
        const std::set<char>& CFGTerminals,
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions, 
        const char& CFGStartsymbol,
        const unsigned int lookahead
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
        const unsigned int lookahead
        );

    /**
     * @brief Process an input string through the LL Parser
     *
     * @param input The string to be processed by the LL Parser
     *
     * @return A bool telling if the Parser accepted
     */
    bool process(const std::string& input) const;

    /**
     * @brief Destructor
     */
    virtual ~LLParser();

    /**
     * @brief Prints the parse table to the given output stream.
     *
     * @param stream The output stream
     * @param obj The parser
     *
     * @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream& stream, const LLParser& obj);

private:
    /**
     * @brief Checks if the given character is a variable.
     *
     * @param e Character to check
     *
     * @return True if 'e' is a variable, else False
     */
    bool isVariable(char e) const;

    const LLTable parseTable;
    const char startsymbol;
    const std::set<char> CFGTerminals;
    const std::set<char> CFGVariables;
};


/**
 * @brief Class representing an RNA Parser based on an LLParser
 */
class RNAParser {
public:
    /**
     * @brief Parses the given string.
     *
     * @param input RNA string
     * @param stemSize The size of the stem 
     *
     * @return True if 'input' is a vallid RNA string with stem of size 'stemSize'
     */ 
    static bool parse(std::string input, unsigned int stemSize);

    /**
     * @brief Parses the given string.
     *
     * @param input RNA string
     *
     * @return If vallid the stemsize of the RNA-string, else 0
     */ 
    static unsigned int parse(const std::string& input);

    /**
     * @brief Indicates whether the given character is a vallid RNA-element.
     *
     * @param c Possible RNA-element
     *
     * @return True if 'c' is a vallid RNA-element
     */ 
    static bool isElement(char c);

private:
    static LLParser createParser();
    static const LLParser parser;
};
}

#endif /*LLPARSER_H*/
