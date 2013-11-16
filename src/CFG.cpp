/*
 * CFG.cpp
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
 * Last modified: 16 November 2013
 * By: Stijn Wouters
 */
#include "CFG.h"
#include <algorithm>
#include <iterator>

CFG::CFG(
    const std::set<char>& terminals,
    const std::set<char>& variables,
    const std::multimap<char, SymbolString>& productions,
    const char& start
    ) : fTerminals(terminals), fVariables(variables), 
        fProductions(productions), fStartSymbol(start) {
    // nothing else to construct
}

CFG::CFG(const CFG& cfg) : CFG(cfg) {
    // nothing else to copy
}

CFG& CFG::operator=(const CFG& cfg) {
    this->fTerminals = cfg.fTerminals;
    this->fVariables = cfg.fVariables;
    this->fProductions = cfg.fProductions;
    this->fStartSymbol = cfg.fStartSymbol;
    return *this;
}

CFG::~CFG() {
    // nothing to destroy
}

void CFG::eleminateEpsilonProductions() {
    for (char c : fVariables) {
        for (auto it = fProductions.equal_range(c).first; 
                it != fProductions.equal_range(c).second; ++it) {
            // check whether this is an epsilon production
            if (it->second == "") {
                fProductions.erase(it);
            } else {
                // no it isn't
                continue;
            } // end if-else
        } // end for
    } // end for
    return;
}

void CFG::eleminateUnitProductions() {
    bool modified = true;

    // keep going untile the production rules aren't modified
    while (modified) {
        modified = false;

        for (char c : fVariables) {
            for (auto it = fProductions.equal_range(c).first; 
                it != fProductions.equal_range(c).second; ++it) {
                // first check whether this is an unit production
                if ((*it).second.size() == 1 
                        && fVariables.find(it->second[0]) != fVariables.end()) {
                    // it is, then replace this production rule
                    fProductions.erase(it);
                    for (auto i = fProductions.equal_range(it->second[0]).first;
                            i != fProductions.equal_range(it->second[0]).second;
                            ++i) {
                        std::pair<char, SymbolString> args(c, i->second);
                        fProductions.insert(args);
                    } // end for
                    modified = true;
                } else {
                    continue;
                } // end if-else
            } // end for
        } // end for
    } // end while
    return;
}

void CFG::eleminateUselessSymbols() {
    // first, find all reachable symbols
    std::set<char> reachable = {fStartSymbol};  // base case

    bool newSymbols = true;

    // keep going untill no new symbols were found
    while (newSymbols) {
        unsigned int size_before = reachable.size();

        for (char c : reachable) {
            for (auto it = fProductions.equal_range(c).first;
                    it != fProductions.equal_range(c).second; ++it) {
                // the symbols in the body are surely reachable
                for (char s : it->second) {
                    reachable.insert(s);
                } // end for
            } // end for
        } // end for

        unsigned int size_after = reachable.size();

        // if new symbols were found, then keep going
        newSymbols = (size_before == size_after) ? false : true;
    } // end while

    // then, find all generating symbols
    std::set<char> generating;
    generating.insert(fTerminals.begin(), fTerminals.end());    // base case

    newSymbols = true;
    while (newSymbols) {
        unsigned int size_before = generating.size();

        for (char c : fVariables) {
            for (auto it = fProductions.equal_range(c).first;
                    it != fProductions.equal_range(c).second; ++it) {
                // check if all symbols in the body are generating
                bool gen = true;
                for (char s : it->second) {
                    if (generating.find(s) == generating.end()) {
                        gen = false;
                        break;
                    } else {
                        continue;
                    } // end if-else
                } // end for

                if (gen)
                    generating.insert(it->first);
            } // end for
        } // end for

        unsigned int size_after = generating.size();

        // if new symbols were found, then keep going
        newSymbols = (size_before == size_after) ? false : true;
    } // end while

    // Now, all useful symbols will be both generating and reachable
    std::set<char> useful;

    std::set_intersection(
                    reachable.begin(), reachable.end(),
                    generating.begin(), generating.end(),
                    std::inserter(useful, useful.begin()));

    for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
        if (useful.find(it->first) == useful.end())
            fProductions.erase(it);
            continue;

        for (char c : it->second) {
            if (useful.find(c) == useful.end())
                fProductions.erase(it);
                break;
        } // end for
    } // end for
    return;
}
