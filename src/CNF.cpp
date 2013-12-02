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

    unsigned int size_before = fVariables.size();

    // new variables, keep incrementing until the variable is not already
    // in the set of variables
    char c = 1;
    while (fVariables.find(c) != fVariables.end()) { ++c; }

    // first, elemeninate terminal symbols in bodies (of size > 1)
    std::map<char, char> terms; // which terminals belong to which variable

    // generate productions of the form A --> a for a is each terminal
    for (char t : fTerminals) {
        // introduce a new variable
        fVariables.insert(c);

        fProductions.insert(std::pair<char, SymbolString>(c, SymbolString(1, t)));

        terms.insert(std::pair<char, char>(t, c));

        // don't forget to update the variable!
        ++c;
        while (fVariables.find(c) != fVariables.end()) { ++c; }
    } // end for

    // now we can actually remove the terminals in the bodies of size > 1
    std::multimap<char, SymbolString> newProductions;

    for (char v : fVariables) {
        for (SymbolString body : this->bodies(v)) {
            if (body.size() > 1) {
                // replace all terminals by the corresponding variables
                SymbolString newBody;

                for (char s : body) {
                    if (fTerminals.find(s) == fTerminals.end()) {
                        // you may append variables
                        newBody += s;
                    } else {
                        // replace terminal by the corresponding terminal
                        newBody += terms.find(s)->second;
                    } // end if-else
                } // end for

                newProductions.insert(std::pair<char, SymbolString>(v, newBody));
            } else {
                newProductions.insert(std::pair<char, SymbolString>(v, body));
            } // end if-else
        } // end for
    } // end for

    fProductions = newProductions;

    unsigned int size_after = fVariables.size();

    // keep going untill no new variables were introduced (indicating
    // everything is now in Chomsky Normal Form)
    while (size_before != size_after) {
        size_before = fVariables.size();

        // the new set of production rules
        std::multimap<char, SymbolString> newProductions;

        for (char v : fVariables) {
            for (SymbolString body : this->bodies(v)) {
                if (body.size() <= 2) {
                    // this rule is already in Chomsky Normal Form
                    newProductions.insert(std::pair<char, SymbolString>(v, body));
                } else {
                    // e.g. ABCDE becomes AB and VCDE bodies

                    // introduce new variable
                    fVariables.insert(c);

                    // add the rule V --> AB
                    newProductions.insert(std::pair<char, SymbolString>(c, SymbolString(body.begin(), body.begin()+2)));

                    SymbolString newBody1(1, c);
                    // append the rest of the original body (ABCDE) --> (VCDE)
                    newBody1.append(body.begin()+2, body.end());
                    newProductions.insert(std::pair<char, SymbolString>(v, newBody1));

                    // don't forget to update the variable!
                    ++c;
                    while (fVariables.find(c) != fVariables.end()) { ++c; }
                } // end if-else
            } // end for
        } // end for

        fProductions = newProductions;

        size_after = fVariables.size();
    } // end while
}

bool CNF::CYK(const std::string& terminalstring) const {
    // first, check whether the terminalstring is valid
    for (char t : terminalstring) {
        if (fTerminals.find(t) == fTerminals.end())
            throw std::invalid_argument("Invalid terminal string.");
    } // end for

    // first generate inverse production rules
    std::multimap<char, char> baseProductions;
    std::multimap<SymbolString, char> inductiveProductions;

    for (char v : fVariables) {
        for (SymbolString body : this->bodies(v)) {
            if (body.size() > 1) {
                // inductive production rule
                inductiveProductions.insert(std::pair<SymbolString, char>(body, v));
            } else {
                baseProductions.insert(std::pair<char, char>(body.at(0), v));
            } // end if-else
        } // end for
    } // end for

    // the CYK table
    std::map<std::pair<int, int>, std::set<char>> table;

    // base case
    for (unsigned int i = 1; i <= terminalstring.size(); ++i) {
        // the set of variables producting this terminal
        std::set<char> vset;

        auto range = baseProductions.equal_range(terminalstring.at(i-1));

        for (auto it = range.first; it != range.second; ++it) {
            vset.insert(it->second);
        } // end for

        table.insert(std::pair<std::pair<int, int>, std::set<char>>(std::pair<int, int>(i, i), vset));
    } // end for

    unsigned int i = 1;
    unsigned int j_init = 2;
    unsigned int j = j_init;

    // keep going until you reached row 1 column == size of the terminalstring
    while( !(i==1 && j == terminalstring.size()+1) ) {
        // the set of variables producing this X(ij)
        std::set<char> vset;

        for (unsigned int k = i; k < j; ++k) {
            for (char v0 : table.find(std::pair<int, int>(i, k))->second) {
                for (char v1 : table.find(std::pair<int, int>(k+1, j))->second) {
                    SymbolString body;
                    body += std::string(1, v0);
                    body += std::string(1, v1);

                    // try to find variables producing sentential form v(0)v(1)
                    auto range = inductiveProductions.equal_range(body);
                    for (auto it = range.first; it != range.second; ++it) {
                        vset.insert(it->second);
                    } // end for
                } // end for
            } // end for
        } // end for

        table.insert(std::pair<std::pair<int, int>, std::set<char>>(std::pair<int, int>(i, j), vset));

        // now iterate further, depending where you are now
        if (j == terminalstring.size()) {
            // move row up
            j = ++j_init;
            i = 1;
        } else {
            // move to the auxillary cell
            ++i; ++j;
        }  // end if-else
    } // end while

    auto it = table.find(std::pair<int, int>(1, terminalstring.size()));
    auto setit = (it->second).find(fStartSymbol);
    return (setit == (it->second).end()) ? false : true;
}
