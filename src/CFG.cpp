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
 * Last modified: 17 November 2013
 * By: Stijn Wouters
 */
#include "CFG.h"
#include <stdexcept>
#include <algorithm>
#include <iterator>

#include <iostream>

/**
 * @brief Recursive implementation to get all the subsets of a given set.
 *
 * @param s The set whose subsets you want to know.
 *
 * @return The subset of the given set.
 */
std::set< std::set<int> > subset(std::set<int>& s) {
    if (s.empty()) {
        // base case
        return { {} };
    } else {
        // recursion: remove one element from the subset
        int i = *(s.begin());

        // get all the subset of the set without the last element
        s.erase(s.begin());

        std::set< std::set<int> > recursion = subset(s);

        // now, add all those subset with the last character
        std::set< std::set<int> > result;

        for (std::set<int> r : recursion) {
            r.insert(i);

            result.insert(r);
        } // end for

        // finally, return the result + recursion
        result.insert(recursion.begin(), recursion.end());
        return result;
    } // end if-else
}

CFG::CFG(
    const std::set<char>& terminals,
    const std::set<char>& variables,
    const std::multimap<char, SymbolString>& productions,
    const char& start
    ) : fTerminals(terminals), fVariables(variables), 
        fProductions(productions), fStartSymbol(start) {
    // check the preconditions

    // The set of variables and the set of terminals are disjoints?
    std::set<char> diff;
    std::set_intersection(
                    fTerminals.begin(), fTerminals.end(),
                    fVariables.begin(), fVariables.end(),
                    std::inserter(diff, diff.begin())
                    );

    if (!diff.empty())
        throw std::invalid_argument("Set of terminals and variables are not disjoint.");

    // valid production rules?
    for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
        // check whether the head is a variable
        if (fVariables.find(it->first) == fVariables.end())
            throw std::invalid_argument("Invalid production rules.");

        // check whether the body consists of symbols that is either in the
        // set of terminals or in 
        for (auto it1 = fProductions.equal_range(it->first).first;
                    it1 != fProductions.equal_range(it->first).second; ++it1) {
            for (char s : it->second) {
                if (fTerminals.find(s) != fTerminals.end()
                        || fVariables.find(s) != fVariables.end()) {
                    continue;
                } else {
                    throw std::invalid_argument("Invalid production rules.");
                } // end if-else
            } // end for
        } // end for
    } // end for

    // starting symbol is in the set of variables?
    if (fVariables.find(fStartSymbol) == fVariables.end())
        throw std::invalid_argument("Invalid start symbol.");
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

std::set<SymbolString> CFG::productions(const char& v) const {
    // first check whether the variable is in the set of variables
    if (fVariables.find(v) == fVariables.end())
        throw std::invalid_argument("Symbol is not in the set of variables.");

    std::set<SymbolString> productions;

    auto range = fProductions.equal_range(v);
    for (auto it = range.first; it != range.second; ++it) {
        productions.insert(it->second);
    } // end for

    return productions;
}

std::set<char> CFG::nullable() const {
    std::set<char> nullable;

    // base case: all variables having an production A -> "" is surely nullable
    for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
        auto range = fProductions.equal_range(it->first);

        for (auto it1 = range.first; it1 != range.second; ++it1) {
            // check whether the body of the production has an empty string
            if ((it1->second).empty())
                nullable.insert(it->first);
        } // end for
    } // end for


    // recursive part: all variables having a production whose body consists
    // only of nullable variables are also nullable
    bool newSymbols = true;

    // keep going until no new nullable symbols were found
    while (newSymbols) {
        unsigned int size_before = nullable.size();

        for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
            // if the key is already nullable, skip them
            if (nullable.find(it->first) != nullable.end())
                continue;

            auto range = fProductions.equal_range(it->first);

            for (auto it1 = range.first; it1 != range.second; ++it1) {
                // iterate over each symbol in the body and check whether they
                // are nullable
                bool isNullable = true;

                for (char s : it1->second) {
                    if (nullable.find(s) == nullable.end()) {
                        isNullable = false;
                        break;
                    } else {
                        continue;
                    } // end if-else
                } // end for


                if (isNullable) 
                    nullable.insert(it->first);
            } // end for
        } // end for

        unsigned int size_after = nullable.size();

        // check whether new symbols were found
        newSymbols = (size_before == size_after) ? false : true;
    } // end for

    return nullable;
}

void CFG::eleminateEpsilonProductions() {
    // first, find all nullable symbols
    std::set<char> nullables = this->nullable();

    for (char v : nullables) { 
        // the new set of production rules for this variable
        std::multimap<char, SymbolString> newRules;

        auto range = fProductions.equal_range(v);

        for (auto it = range.first; it != range.second; ++it) {
            // now, get the indexes of symbols that are nullable
            std::set<int> indexes;
            for (unsigned int i = 0; i < (it->second).size(); ++i) {
                if (nullables.find((it->second).at(i)) != nullables.end())
                    indexes.insert(i);
            } // end for

            // now generate all the bodies that are a result from wheter or
            // not removing the symbols that were nullable, you'll need the
            // subset of the indexes
            for (std::set<int> s : subset(indexes) ) {
                SymbolString body = it->second;

                // remove some nullable symbols
                int i = 0;
                for (int k : s) {
                    body.erase(k-i, 1);
                    ++i;
                } // end for

                // skip if the body is already empty
                if (body.empty())
                    continue;

                // add them to the new productions
                std::pair<char, SymbolString> rule(v, body);

                newRules.insert(rule);
            } // end for
            
        } // end for

        // replace the old productions by the new
        fProductions.erase(v);
        fProductions.insert(newRules.begin(), newRules.end());
    } // end for
    return;
}

/*
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
*/
