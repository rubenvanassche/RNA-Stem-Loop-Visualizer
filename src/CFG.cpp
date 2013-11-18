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

                //  if all symbols in the body were nullable, then the head
                // is also nullable
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

std::set< std::pair<char, char> > CFG::units() const {
    std::set< std::pair<char, char> > units;

    // first the base case
    for (char v : fVariables) {
        units.insert( std::pair<char, char>(v, v) );
    } // end for

    // now, recursion
    for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
        auto range = fProductions.equal_range(it->first);

        for (auto it1 = range.first; it1 != range.second; ++it1) {
            // if the body is of length 1 and it's a variable, then we have
            // an unit pair
            if ((it1->second).size() == 1 
                    && fVariables.find((it1->second).at(0)) != fVariables.end())
                units.insert( std::pair<char, char>(it1->first, (it1->second).at(0)) );
        } // end for
    } // end for

    return units;
}

void CFG::eleminateUnitProductions() {
    // first, get all unit pairs
    std::set< std::pair<char, char> > units = this->units();

    // check whether there are no circular unit pairs
    // TODO

    // the new productions
    std::multimap<char, SymbolString> newProductions;

    for (std::pair<char, char> u : units) {
        // get all the bodies from the second variable in the unit pair
        std::set<SymbolString> bodies = this->productions(u.second);

        // those bodies are also the bodies of the rules from the first
        // variable in the unit pair
        for (SymbolString b : bodies) {
            // ignore if the body is of length 1 and is a variable
            if (b.size() == 1 && fVariables.find(b.at(0)) != fVariables.end())
                continue;

            // add the new rule
            std::pair<char, SymbolString> rule(u.first, b);

            newProductions.insert(rule);
        } // end for
    } // end for

    fProductions = newProductions;
    return;
}

std::set<char> CFG::generating() const {
    // base case
    std::set<char> generating = fTerminals;

    // recursive part: all variables having a production whose body consists
    // only of generating symbols are also generating
    bool newSymbols = true;

    // keep going until no new nullable symbols were found
    while (newSymbols) {
        unsigned int size_before = generating.size();

        for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
            // if the key is already generating, skip them
            if (generating.find(it->first) != generating.end())
                continue;

            auto range = fProductions.equal_range(it->first);

            for (auto it1 = range.first; it1 != range.second; ++it1) {
                // iterate over each symbol in the body and check whether they
                // are generating
                bool isGenerating = true;

                for (char s : it1->second) {
                    if (generating.find(s) == generating.end()) {
                        isGenerating = false;
                        break;
                    } else {
                        continue;
                    } // end if-else
                } // end for

                //  if all symbols in the body were generating, then the head
                // is also generating
                if (isGenerating) 
                    generating.insert(it->first);
            } // end for
        } // end for

        unsigned int size_after = generating.size();

        // check whether new symbols were found
        newSymbols = (size_before == size_after) ? false : true;
    } // end for

    return generating;
}

std::set<char> CFG::reachable() const {
    // base case: the start symbol is surely reachable
    std::set<char> reachable = {fStartSymbol};

    // recursive part: 
    bool newSymbols = true;

    while (newSymbols) {
        unsigned int size_before = reachable.size();

        for (char s : reachable) {
            // ignore if it's a terminal
            if (fTerminals.find(s) != fTerminals.end())
                continue;

            // get all the bodies with this symbol as head
            std::set<SymbolString> bodies = this->productions(s);

            // all symbols of the bodies are also reachable
            for (SymbolString b : bodies) {
                for (char c : b ) {
                    reachable.insert(c);
                } // end for
            } // end for
        } // end for

        unsigned int size_after = reachable.size();

        // check whether new symbols were found
        newSymbols = (size_before == size_after) ? false : true;
    } // end for

    return reachable;
}

void CFG::eleminateUselessSymbols() {
    // first, eleminate all symbols that are not generating
    std::set<char> generating = this->generating();

    // the new set of production rules
    std::multimap<char, SymbolString> newProductions;

    for (char v : generating) {
        // ignore if v is a terminal
        if (fTerminals.find(v) != fTerminals.end())
            continue;

        // get the bodies
        std::set<SymbolString> bodies = this->productions(v);

        // only add rules whose body does not contain any non-generating symbols
        for (SymbolString b : bodies) {
            // first check whether the body does not contain any non-generating
            // symbols, if so, skip it
            bool isGenerating = true;
            for (char c : b) {
                if (generating.find(c) == generating.end()) {
                    isGenerating = false;
                    break;
                } else {
                    continue;
                } // end if-else
            } // end for

            // add them if it's ok
            if (isGenerating) {
                std::pair<char, SymbolString> rule(v, b);
                newProductions.insert(rule);
            } else {
                continue;
            } // end if-else
        } // end for
    } // end for

    fProductions = newProductions;
    // now we have a set of production rules only containing generating
    // symbols, our next step is to eleminate non-reachable symbols

    std::set<char> reachable = this->reachable();

    for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
        // if the key is not reachable, then remove the rules
        if (reachable.find(it->first) == reachable.end()) {
            fProductions.erase(it->first);
        }
    } // end for

    return;
}

void CFG::cleanUp() {
    // this one is easy now, just clean up in this order
    this->eleminateEpsilonProductions();
    this->eleminateUnitProductions();
    this->eleminateUselessSymbols();
    // that's it
    return;
}
