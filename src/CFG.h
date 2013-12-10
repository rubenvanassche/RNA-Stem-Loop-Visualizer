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
 * Last modified: 27 November 2013
 * By: Stijn Wouters
 */

#ifndef CFG_H_
#define CFG_H_

#include <set>
#include <map>
#include <string>

/**
 * @brief A consisting of symbols from either the set of variables or the 
 * set of terminals.
 */
typedef std::string SymbolString;

/**
 * @brief Class representing a context free grammar.
 */
class CFG {
public:
     /**
     * @brief Constructor
     *
     * @param terminals  A set containing the terminals of the CFG.
     * @param variables A set containing the variables of the CFG.
     * @param productions A multimap that maps any symbol from the set of
     * variables to a (possibly empty) SymbolString (which contains symbols
     * from either the set of terminals or either the set of variables.
     * @param startsymbol The startsymbol for the CFG.
     *
     * @pre
     * - The set of variables and the set of terminals are disjoints.
     * - The production rule is valid: the head consist of exactly one symbol
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
     * @brief Destructor.
     */
    virtual ~CFG();

    /**
     * @brief Get the set of bodies with the passed variable as head. For
     * example if there are productions of the form A -> "a" and A -> "aA",
     * then this will returns {"a", "aA"}.
     *
     * @param v The variable representing the head of the production rules.
     *
     * @return The set of SymbolString representing the body of the production
     * rules whose head is the passed variable.
     *
     * @pre
     * - The passed variable must be in the set of the variables.
     *
     * @throw std::invalid_argument The precondition were not satisfied.
     */
    std::set<SymbolString> bodies(const char& v) const;

    /**
     * @brief Get all the nullable variables.
     *
     * @return The set of all nullable variables.
     */
    std::set<char> nullable() const;

    /**
     * @brief Eleminate epsilon productions. That is, eleminate productions
     * of the form A -> ε, but doing so that the CFG still accepts the same
     * language with epsilon (empty string excluded).
     *
     * @post The production rules doesn't contain any nullable symbols.
     */
    void eleminateEpsilonProductions();

    /**
     * @brief Get all the unit pairs of this CFG.
     *
     * @return The set of all unit pairs.
     */
    std::set< std::pair<char, char> > units() const;

    /**
     * @brief Eleminate unit productions. That is, eleminate productions of
     * the form A -> B. But doing so that it does not affect the language of
     * this CFG.
     *
     * @note The algorithm only works if there is no cycle of unit productions.
     * That is, unit pairs of the form A -> B, B -> C and C -> A. If that's the
     * case, an exception will be thrown.
     *
     * @throw std::logic_error This CFG has a cycle of unit pairs. TODO
     *
     * @post The CFG has only unit pairs of the form (A, A) for each A is a
     * variable.
     */
    void eleminateUnitProductions();

    /**
     * @brief Get all the generating symbols.
     *
     * @return The set of generating symbols.
     */
    std::set<char> generating() const;

    /**
     * @brief Get all the reachable symbols.
     *
     * @return The set of all reachable symbols.
     */
    std::set<char> reachable() const;

    /**
     * @brief Eleminate useless symbols. But doing so that is does not affect
     * the language of this CFG.
     *
     * @post The production rules doesn't contain any useless symbols.
     * @post The CFG still accepts the same language.
     */
    void eleminateUselessSymbols();

    /**
     * @brief Clean up CFG, that is, eleminate epsilon productions, useless
     * symbols and unit productions IN SAFE ORDER. This comes in handy for 
     * converting to CNF (Chomsky Normal Form). Also: removes all variables
     * which don't have any production rules at all.
     *
     * @post The production rules doesn't contain any nullable symbols.
     * @post The production rules doesn't contain any useless symbols.
     * @post The CFG has only unit pairs of the form (A, A) for each A is a
     * variable.
     */
    void cleanUp();

    /*
     * @brief Get the terminals of the CFG.
     */
    std::set<char> getTerminals(){
    	return this->fTerminals;
    }

    /*
	* @brief Get the variables of the CFG.
	*/
	std::set<char> getVariables(){
		return this->fVariables;
	}

	 /*
	 * @brief Get the productions of the CFG.
	 */
	std::multimap<char, SymbolString> getProductions(){
		return this->fProductions;
	}

	 /*
	 * @brief Get the start symbol of the CFG.
	 */
	char getStartsymbol(){
		return this->fStartSymbol;
	}

protected:
    /**
     * @brief The set of terminal symbols.
     */
    std::set<char> fTerminals;

    /**
     * @brief The set of variables.
     */
    std::set<char> fVariables;

    /**
     * @brief The set of production rules.
     */
    std::multimap<char, SymbolString> fProductions;

    // the starting symbol
    char fStartSymbol;
};

#endif /* CFG_H_ */
