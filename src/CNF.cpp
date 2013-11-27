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
 * Last modified: 27 November 2013
 * By: Stijn Wouters
 */
#include "CNF.h"
#include <map>
#include <stdexcept>

#include <iostream>

CNF::CNF(
    const std::set<char>& terminals,
    const std::set<char>& variables,
    const std::multimap<char, SymbolString>& productions,
    const char& start
    ) : CFG(terminals, variables, productions, start) {
    // first thing to do is clean up grammar
    this->cleanUp();

    // new variables, keep incrementing until the variable is not already
    // in the set of variables
    char c = 0;
    while (fVariables.find(c) != fVariables.end()) { ++c; }

    // first, elemeninate terminal symbols in bodies (of size > 1)
    std::map<char, char> terms;

    for (char t : fTerminals) {
        std::pair<char, SymbolString> rule(c, SymbolString(1, t));
        fProductions.insert(rule);

        terms.insert(std::pair<char, char>(t, c));

        ++c;
        while (fVariables.find(c) != fVariables.end()) { ++c; }
    } // end for

    
    std::multimap<char, SymbolString> productions_no_terminals;

    for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
        auto range = fProductions.equal_range(it->first);

        for (auto it1 = range.first; it1 != range.second; ++it1) {
            if ((it1->second).size() > 1) {
                SymbolString body;
                for (char t : it1->second) {
                    if (fTerminals.find(t) == fTerminals.end()) {
                        body += t;
                    } else {
                        body += terms.find(t)->second;
                    } // end if-else
                } // end for
                productions_no_terminals.insert(std::pair<char, SymbolString>(it->first, body));
            } else {
                productions_no_terminals.insert(std::pair<char, SymbolString>(it->first, it1->second));
            } // end if-else
        } // end for
    } // end for

    fProductions = productions_no_terminals;

    // check whether this set of productions is already in CNF
    bool notChomsky = true;

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
                        ++c;
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
    // first, check whether the terminalstring is valid
    for (char t : terminalstring) {
        auto it = fTerminals.find(t);

        if (it == fTerminals.end())
            throw std::invalid_argument("Invalid terminal string.");
    } // end for

    // inverse production rules
    std::multimap<SymbolString, char> inductiveProductions;
    std::multimap<char, char> baseProductions;
    for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
        auto range = fProductions.equal_range(it->first);

        for (auto it1 = range.first; it1 != range.second; ++it1) {
            if ((it1->second).size() > 1) {
                std::pair<SymbolString, int> args(it1->second, it->first);
                inductiveProductions.insert(args);
            } else {
                std::pair<int, int> args((it1->second).at(0), it->first);
                baseProductions.insert(args);
            } // end if-else
        } // end for
    } // end for

    std::map<std::pair<int, int>, std::set<char>> table;

    // base case
    std::vector<std::set<char>> row;
    for (unsigned int i = 1; i <= terminalstring.size(); ++i) {
        auto range = baseProductions.equal_range(terminalstring.at(i-1));
        std::set<char> xset;
        for (auto it = range.first; it != range.second; ++it) {
            xset.insert(it->second);
        } // end for
        table.insert(std::pair<std::pair<int, int>, std::set<char>>(std::pair<int, int>(i, i), xset));
    } // end for

    unsigned int i = 1;
    unsigned int j_init = 2;
    unsigned int j = j_init;
    while( !(i==1 && j == terminalstring.size()+1)) {
            std::set<char> xset;
        for (int k = i; k < j; ++k) {
            for (char B : table.find(std::pair<int, int>(i, k))->second) {
                for (char C : table.find(std::pair<int, int>(k+1, j))->second) {
                    SymbolString Body;
                    Body += std::string(1, B);
                    Body += std::string(1, C);
                    auto range = inductiveProductions.equal_range(Body);
                    for (auto it = range.first; it != range.second; ++it) {
                        xset.insert(it->second);
                    } // end for
                } // end for
            } // end for
        } // end for
                    table.insert(std::pair<std::pair<int, int>, std::set<char>>(std::pair<int, int>(i, j), xset));

        if (j == terminalstring.size()) {
            j = ++j_init;
            i = 1;
        } else {
            ++i; ++j;
        }  // end if-else
    } // end while

    auto it = table.find(std::pair<int, int>(1, terminalstring.size()));
    auto setit = (it->second).find(fStartSymbol);
    return (setit == (it->second).end()) ? false : true;
}
