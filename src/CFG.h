/*
 * CFG.h
 *
 * Copyright (C) 2013 Ruben Van Assche
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
 * Last modified: 8 November 2013
 * By: Ruben Van Assche
 */

#ifndef CFG_H_
#define CFG_H_

#include <vector>
#include <map>
#include <string>

/**
 * @brief Class representing a sequence of terminals and variables
 */
class SymbolString{
public:
     /**
     * @brief Constructor
     *
     * @param string A string containing variables and symbols
     */
    SymbolString(const std::string& string);

     /**
     * @brief Get the string
     *
     * @param string A string containing variables and symbols
     *
     * @return A string representing the symbolString
     */
    std::string get() const;

     /**
     * @brief Get a symbol in the string at a certain point
     *
     * @param index The index of the symbol you want
     *
     * @return The symbol you wanted
     */
    char get(const int& index) const;

     /**
     * @brief Remove a symbol in the string at a certain point
     *
     * @param index The index of the symbol you want removed
     *
     * @return A bool telling if the operation was successfull
     */
    bool remove(const int& index);

     /**
     * @brief Append two symbolStrings
     *
     * @param second The second symbolString
     *
     * @return This symbolString object
     */
    SymbolString& operator+(const SymbolString second);

     /**
     * @brief Get the length of the symbolString
     *
     * @return An integer
     */
    int size() const;

private:
    std::string fSymbols;
};

/**
 * @brief Class representing a CFG
 */
class CFG {
public:
     /**
     * @brief Constructor
     *
     * @param CFGTerminals A vector containing the terminals of the CFG
     * @param CFGVariables A vector containing the variables of the CFG
     * @param CFGTransitions A multimap that maps a symbol to an symbolString
     * @param CFGStartsymbol The startsymbol for the CFG
     */
    CFG(
        const std::vector<char>& CFGTerminals, 
        const std::vector<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGTransitions, 
        const char& CFGStartsymbol
        );

    virtual ~CFG();

private:
    std::multimap<char, SymbolString> fTransitions;
    std::vector<char> fTerminals;
    std::vector<char> fVariables;

    char fStartSymbol;
};

#endif /* CFG_H_ */
