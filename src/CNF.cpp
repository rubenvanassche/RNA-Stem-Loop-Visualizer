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
 * Last modified: 18 November 2013
 * By: Stijn Wouters
 */
#include "CNF.h"

CNF::CNF(
    const std::set<char>& terminals,
    const std::set<char>& variables,
    const std::multimap<char, SymbolString>& productions,
    const char& start
    ) : CFG(terminals, variables, productions, start) {
    // first, clean up the grammar
    this->cleanUp();

    // then convert it to Chomsky Normal Form
    // TODO
}

CNF::CNF(const CFG& cfg) : CFG(cfg) {
    // first, clean up the grammar
    this->cleanUp();

    // then, convert it to Chomsky Normal Form
    // TODO
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

bool CNF::CYK(const std::string& terminalstring) const{
    // TODO
    return true;
}
