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
 * Last modified: 11 November 2013
 * By: Stijn Wouters
 */

#ifndef H_CNF_H
#define H_CNF_H

#include "CFG.h"

/**
 * The class CNF (Chomsky Normal Form), this is actually a CFG (Context Free
 * Grammar) but with productions of the form
 *     - A --> BC (with A, B and C variables) or
 *     - A --> a (with A a variable and a a terminal)
 *
 * Since this is a CFG, CNF will be inherited from CFG so you can also perform
 * the same actions as declared in the CFG interface.
 */
class CNF : public CFG {
public:
    /**
     * Constructor, initialize all datamembers. You'll have to pass a set
     * of terminal symbols, a set of variables, a multimap where each
     * character from the set of variables is (multi)mapped to a symbolstring
     * presenting production rules and a startsymbol (from the set of
     * variables.
     * Additionally, this constructor will reconstruct the productionrules and
     * the set of variables and terminals so that it is actually a CNF.
     *
     * Throws the same exception as the CFG constructor.
     */
    CNF(
        const std::set<char>& terminals,
        const std::set<char>& variables,
        const std::multimap<char, SymbolString>& productions,
        const char& start
        );

    /**
     * Convert the CFG to a CNF by calling the constructor immediatelly.
     */
    CNF(const CFG& cfg);

    /**
     * Copy constructor.
     */
    CNF(const CNF& cnf);

    /**
     * Copy assignment operator overloading.
     */
    CNF& operator=(const CNF& cnf);

    /**
     * Destructor.
     */
    ~CNF();
};

#endif // H_CNF_H
