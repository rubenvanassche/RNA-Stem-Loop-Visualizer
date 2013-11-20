/* CNF (Chomsky Normal Form)
 * Interface declaration of CNF (Chomsky Normal Form).
 *
 * Copyright (C) 2013 Stijn Wouters
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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
 * Last modified: 20 November 2013
 * By: Stijn Wouters
 */

#ifndef H_CNF_H
#define H_CNF_H

#include "CFG.h"
#include <set>
#include <map>
#include <vector>

/**
 * @brief The class CNF (Chomsky Normal Form), this is actually a CFG (Context
 * Free Grammar) but with production rules of the form:
 * - A --> BC (with A, B and C variables) or
 * - A --> a (with A a variable and a a terminal)
 */
class CNF : public CFG {
public:
    /**
     * @brief Constructor, initialize all datamembers. This will construct
     * production rules based upon the rules of the CFG cleaned up variant and
     * satisfying the conditions imposed by the CNF. 
     *
     * @note You can still use the CFG methods as if it was only cleaned up
     * (thus you can use the same variables for getting the bodies and such).
     *
     * @param terminals The set of terminal symbols.
     * @param variables The set of non-terminal symbols.
     * @param productions The set of production rules (which is actually a
     * std::map where each non-terminal symbol maps to a string consisting of
     * terminals and variables.
     * @param start The start symbol.
     *
     * @post
     * - The CFG methods will produce the same result as if it was already
     *   cleaned up without unwanted side-effects.
     */
    CNF(
        const std::set<char>& terminals,
        const std::set<char>& variables,
        const std::multimap<char, SymbolString>& productions,
        const char& start
        );

    /**
     * @brief Copy constructor.
     *
     * @param cnf The CNF to be copied.
     */
    CNF(const CNF& cnf);

    /**
     * @brief Copy assignment operator overloading.
     *
     * @param cnf The CNF to be assigned to.
     */
    CNF& operator=(const CNF& cnf);

    /**
     * @brief Destructor.
     */
    ~CNF();

private:
    /*
     * The raison that I use an apart production map is that the amount of
     * variables may be very big, even bigger than the provided chars (assuming
     * you don't want the special ASCII characters such as ~, @, #, ...
     * representing a variable). Besides, you can still keep using the methods
     * in the CFG's interface, because converting CFG's to CNF's will not
     * create any unwanted side effects (except that the CFG will be cleaned
     * up, which is necessary before converting to CNF's).
     */

    // All variables and terminals will be mapped to an integer, I'll call
    // this Chomsky Numbers (however, that doesn't make any sense at all).
    std::map<char, int> fChomskyNumbers;

    // The set of production rules where the head (a Chomsky Number will be
    // projected to an array of integers.
    std::multimap<int, std::vector<int> > fChomskyProductions;
};

#endif // H_CNF_H
