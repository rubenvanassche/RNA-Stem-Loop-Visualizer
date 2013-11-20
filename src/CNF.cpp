/* CNF (Chomsky Normal Form)
 * Interface definition of CNF (Chomsky Normal Form)
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
#include "CNF.h"
#include <map>
#include <stdexcept>

CNF::CNF(
    const std::set<char>& terminals,
    const std::set<char>& variables,
    const std::multimap<char, SymbolString>& productions,
    const char& start
    ) : CFG(terminals, variables, productions, start) {
    // first thing to do is clean up grammar
    CFG::cleanUp();
}

CNF::CNF(const CNF& cnf) : CFG(cnf) {
    // nothing else to construct
}

CNF& CNF::operator=(const CNF& cnf) {
    // nothing else to copy
    return *this;
}

CNF::~CNF() {
    // nothing else to destroy
}
