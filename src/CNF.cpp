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

    // then maps each terminal and variable to the Chomsky numbers
    int i = 0;
    for (char t : terminals) {
        fChomskyNumbers.insert(std::pair<char, int>(t, i++));
    } // end for

    for (char v : variables) {
        fChomskyNumbers.insert(std::pair<char, int>(v, i++));
    } // end for

    for (char v : variables) {
        for (SymbolString body : CFG::bodies(v)) {
            // translate the body to ints
            int chomskyHead = fChomskyNumbers.find(v)->second;
            std::vector<int> chomskyBody;

            for (char s : body) {
                chomskyBody.push_back(fChomskyNumbers.find(s)->second);
            } // end for

            std::pair<int, std::vector<int>> rule(chomskyHead, chomskyBody);
            fChomskyProductions.insert(rule);
        } // end for
    } // end for

    // check whether this is already in CNF
    bool notChomsky = false;

    for (auto it = fChomskyProductions.begin(); it != fChomskyProductions.end(); ++it) {
        auto range = fChomskyProductions.equal_range(it->first);

        for (auto it1 = range.first; it1 != range.second; ++it1) {
            if ((it1->second).size() > 2) {
                notChomsky = true;
                break;
            } else {
                continue;
            } // end if-else
        } // end for
    } // end for

    while (notChomsky) {
        notChomsky = false;

        std::multimap<int, std::vector<int> > newProductions;

        for (auto it = fChomskyProductions.begin(); it != fChomskyProductions.end(); ++it) {
            auto range = fChomskyProductions.equal_range(it->first);

            for (auto it1 = range.first; it1 != range.second; ++it1) {
                if ((it1->second).size() <= 2) {
                    std::pair<int, std::vector<int>> rule(it->first, it1->second);
                    newProductions.insert(rule);
                } else {
                    std::vector<int> chomskyBody = {i};
                    chomskyBody.insert(chomskyBody.begin(), (it1->second).begin()+2, (it1->second).end());
                    newProductions.insert(std::pair<int, std::vector<int>>(it->first, chomskyBody));
                    std::vector<int> chomskyBody1((it1->second).begin(), (it1->second).begin()+2);
                    newProductions.insert(std::pair<int, std::vector<int>>(i++, chomskyBody1));
                } // end if-else
            } // end for
        } // end for

        fChomskyProductions = newProductions;

        for (auto it = fChomskyProductions.begin(); it != fChomskyProductions.end(); ++it) {
            auto range = fChomskyProductions.equal_range(it->first);

            for (auto it1 = range.first; it1 != range.second; ++it1) {
                if ((it1->second).size() > 2) {
                    notChomsky = true;
                    break;
                } else {
                    continue;
                } // end if-else
            } // end for
        } // end for
    } // end while
}

CNF::CNF(const CNF& cnf) : CFG(cnf) {
    this->fChomskyNumbers = cnf.fChomskyNumbers;
    this->fChomskyProductions = cnf.fChomskyProductions;
}

CNF& CNF::operator=(const CNF& cnf) {
    this->fChomskyNumbers = cnf.fChomskyNumbers;
    this->fChomskyProductions = cnf.fChomskyProductions;
    return *this;
}

CNF::~CNF() {
    // nothing else to destroy
}
