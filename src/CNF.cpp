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
 * Last modified: 26 November 2013
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
    this->cleanUp();

    // check whether this set of productions is already in CNF
    bool notChomsky = true;

    // new variables, keep incrementing until the variable is not already
    // in the set of variables
    char c = 0;
    while (fVariables.find(c) != fVariables.end()) { ++c; }

    do {
        notChomsky = false;

        for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
            auto range = fProductions.equal_range(it->first);

            for (auto it1 = range.first; it1 != range.second; ++it1) {
                if ((it1->second).size() > 2) {
                    notChomsky = true;
                    break;
                } else {
                    continue;
                } // end if-else
            } // end for
        } // end for

        // apply reform if this is not in CNF
        if (notChomsky) {
            // the new set of production rules
            std::multimap<char, SymbolString> newProductions;

            for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
                auto range = fProductions.equal_range(it->first);

                for (auto it1 = range.first; it1 != range.second; ++it1) {
                    if ((it1->second).size() <= 2) {
                        // this rule is already in Chomsky Normal Form
                        std::pair<char, SymbolString> rule(it->first, it1->second);
                        newProductions.insert(rule);
                    } else {
                        // std::string constructor (converts char to string)
                        SymbolString chomskyBody(1, c);
                        // append the rest of the original body (ABCDE) --> (VCDE)
                        chomskyBody.append((it1->second).begin()+2, (it1->second).end());
                        // add the rule
                        std::pair<char, SymbolString> rule(it->first, chomskyBody);
                        newProductions.insert(rule);

                        // also add the other rule V --> AB
                        SymbolString chomskyBody1((it1->second).begin(), (it1->second).begin()+2);
                        // add the rule
                        std::pair<char, SymbolString> rule1(c, chomskyBody1);
                        newProductions.insert(rule1);

                        // don't forget to update the variable!
                        while (fVariables.find(c) != fVariables.end()) { ++c; }
                    } // end if-else
                } // end for
            } // end for

            fProductions = newProductions;
        } else {
            // do nothing
        } // end if-esle
    } while (notChomsky);
}

CNF::CNF(const CNF& cnf) : CFG(cnf) {
    // nothing to construct
}

CNF& CNF::operator=(const CNF& cnf) {
    // nothing to copy
    return *this;
}

CNF::~CNF() {
    // nothing else to destroy
}

bool CNF::CYK(const std::string& terminalstring) const {
    /*
    // first, check whether the terminalstring is valid while
    // translating the string into Chomsky Numbers
    std::vector<int> chomskyTerminal;

    for (char t : terminalstring) {
        auto it = fChomskyNumbers.find(t);

        if (it == fChomskyNumbers.end())
            throw std::invalid_argument("Invalid terminal string.");

        chomskyTerminal.push_back(it->second);
    } // end for

    std::multimap< std::vector<int>, int> inductiveProductions;
    std::multimap<int, int> baseProductions;
    for (auto it = fChomskyProductions.begin(); it != fChomskyProductions.end(); ++it) {
        auto range = fChomskyProductions.equal_range(it->first);

        for (auto it1 = range.first; it1 != range.second; ++it1) {
            if ((it1->second).size() == 2) {
                std::pair< std::vector<int>, int> args(it1->second, it->first);
                inductiveProductions.insert(args);
            } else {
                std::pair<int, int> args((it1->second).at(0), it->first);
                baseProductions.insert(args);
            } // end if-else
        } // end for
    } // end for

    std::vector<std::vector<std::set<int>>> table;

    for (int s = chomskyTerminal.size(); s > 0; --s) {
    } // end for
    */
    return true;
}
