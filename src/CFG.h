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
 * Last modified: 17 November 2013
 * By: Stijn Wouters
 */

#ifndef CFG_H_
#define CFG_H_

#include <set>
#include <map>
#include <string>

/**
 * @brief Class representing a sequence of terminals and variables
 */
typedef std::string SymbolString;

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
     *
     * @pre
     * - The set of variables and the set of terminales are disjoints.
     * - The production rule is valid: The head consist of exactly one symbol
     *   that is in the set of the variables and the body must be empty or
     *   consisting of symbols that is either in the set of variables or in
     *   the set of terminals.
     * - The starting symbol must be a member of the set of variables.
     *
     * @throw std::invalid_argument One of the preconditions were not met.
     */
    CFG(
        const std::set<char>& terminals, 
        const std::set<char>& variables,
        const std::multimap<char, SymbolString>& productions, 
        const char& startsymbol
        );

    /**
     * @brief Copy constructor.
     *
     * @param cfg The CFG you want to copy.
     */
    CFG(const CFG& cfg);

    /**
     * @brief Copy assignment operator.
     *
     * @param cfg The CFG you want to assign to it.
     */
    CFG& operator=(const CFG& cfg);

    /**
     * @brief Destructor.
     */
    virtual ~CFG();

    /**
     * @brief Get the set of symbolstrings.
     *
     * @param v The variable representing the head of the production rules.
     *
     * @return The set of symbolstrings representing the body of the
     * production rules.
     *
     * @pre
     * - The variable must be in the set of the variables.
     *
     * @throw std::invalid_argument The precondition were not satisfied.
     */
    std::set<SymbolString> productions(const char& v) const;

    /**
     * @brief Get all the nullable variables.
     *
     * @return The set of all nullable variables.
     */
    std::set<char> nullable() const;

    /**
     * @brief Eleminate epsilon productions. That is, eleminate productions
     * of the form A -> ε
     *
     * @post The production rules doesn't contain any epsilon productions.
     */
    void eleminateEpsilonProductions();

    /**
     * @brief Eleminate unit productions. That is, eleminate productions of
     * the form A -> B. But doing so that it does not affect the language of
     * this CFG.
     *
     * @post The production rules doesn't contain any unit productions.
     * @post The CFG still accepts the same language.
     */
    //void eleminateUnitProductions();

    /**
     * @brief Eleminate useless symbols. But doing so that is does not affect
     * the language of this CFG.
     *
     * @post The production rules doesn't contain any unit productions.
     * @post The CFG still accepts the same language.
     */
    //void eleminateUselessSymbols();

/* 
 * for converting CFG's to CNF, I need access to the following data members.
 * Of course, you could write getters and setters, but that's pointless here
 * since you'll probably never use that getter and setters again.
 */
protected:
    /**
     * @brief The set of terminal symbols.
     */
    std::set<char> fTerminals;

    /**
     * @brief The set of non-terminal symbols.
     */
    std::set<char> fVariables;

    /**
     * @brief The set of production rules where each terminal symbol is 
     * (multi)mapped to a SymbolString
     */
    std::multimap<char, SymbolString> fProductions;

    /**
     * @brief The start symbol
     */
    char fStartSymbol;
};

#endif /* CFG_H_ */
