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
 * Last modified: 11 November 2013
 * By: Ruben Van Assche
 */

#ifndef CFG_H_
#define CFG_H_

#include <set>
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
     * @param CFGTerminals A set containing the terminals of the CFG
     * @param CFGVariables A set containing the variables of the CFG
     * @param CFGProductions A multimap that maps a symbol to an symbolString
     * @param CFGStartsymbol The startsymbol for the CFG
     */
    CFG(
        const std::set<char>& CFGTerminals, 
        const std::set<char>& CFGVariables,
        const std::multimap<char, SymbolString>& CFGProductions, 
        const char& CFGStartsymbol
        );

    virtual ~CFG();

/* for converting CFG's to CNF, I need access to the following data members.
 * Of course, you could write getters and setters, but that's pointless here
 * since you'll probably never use that getter and setters again.
 */
protected:
    /**
     * @brief The set of production rules where each terminal symbol is 
     * (multi)mapped to a SymbolString
     */
    std::multimap<char, SymbolString> fProductions;

    /**
     * @brief The set of terminal symbols.
     */
    std::set<char> fTerminals;

    /**
     * @brief The set of non-terminal symbols.
     */
    std::set<char> fVariables;

// ...but this one will be kept private.
private:
    char fStartSymbol;
};

#endif /* CFG_H_ */
