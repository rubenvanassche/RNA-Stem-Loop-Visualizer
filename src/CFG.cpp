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
 * Last modified: 18 November 2013
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
        s.erase(s.begin());

        // get all the subset of the set without the first element
        std::set< std::set<int> > recursion = subset(s);

        // now, add the removed element in all those subset found
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

    // get the intersection of the set of variables and the set of terminals
    std::set<char> diff;
    std::set_intersection(
                    fTerminals.begin(), fTerminals.end(),
                    fVariables.begin(), fVariables.end(),
                    std::inserter(diff, diff.begin())
                    );

    // it must be empty, because they should be disjoints
    if (!diff.empty())
        throw std::invalid_argument("Set of terminals and variables are not disjoint.");

    // check the production rules
    for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
        // check whether the head is a variable
        if (fVariables.find(it->first) == fVariables.end())
            throw std::invalid_argument("Invalid production rules.");

        // check whether the body consists of symbols that is either in the
        // set of terminals or in the set of variables
        auto range = fProductions.equal_range(it->first);

        for (auto it1 = range.first; it1 != range.second; ++it1) {
            for (char s : it1->second) {
                if (fTerminals.find(s) != fTerminals.end()
                        || fVariables.find(s) != fVariables.end()) {
                    // because you are sure that the set of variables and
                    // terminals were disjoint (checked before) you know
                    // that the symbol is in one of the two (not both) so
                    // continue
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

CFG::CFG(const CFG& cfg)
    : fTerminals(cfg.fTerminals), fVariables(cfg.fVariables), 
        fProductions(cfg.fProductions), fStartSymbol(cfg.fStartSymbol) {
    // you don't have to check the preconditions, since you are sure that
    // the cfg passed has a valid contstruction.
}

CFG& CFG::operator=(const CFG& cfg) {
    // you don't have to check the preconditions, since you are sure that
    // the cfg passed has a valid contstruction.
    this->fTerminals = cfg.fTerminals;
    this->fVariables = cfg.fVariables;
    this->fProductions = cfg.fProductions;
    this->fStartSymbol = cfg.fStartSymbol;
    return *this;
}

CFG::~CFG() {
    // nothing to destroy
}

std::set<SymbolString> CFG::bodies(const char& v) const {
    // first check whether the variable is in the set of variables
    if (fVariables.find(v) == fVariables.end())
        throw std::invalid_argument("Symbol is not in the set of variables.");

    // the set of all bodies with the passed variable v as head
    std::set<SymbolString> bodies;

    auto range = fProductions.equal_range(v);
    for (auto it = range.first; it != range.second; ++it) {
        bodies.insert(it->second);
    } // end for

    return bodies;
}

std::set<char> CFG::nullable() const {
    std::set<char> nullable;

    // base case: all variables having an production A -> "" is surely nullable
    for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
        auto range = fProductions.equal_range(it->first);

        for (auto it1 = range.first; it1 != range.second; ++it1) {
            // check whether the body of the production has an empty string
            // if so, then the head is nullable
            if ((it1->second).empty())
                nullable.insert(it->first);
        } // end for
    } // end for


    // inductive part: all variables having a production whose body consists
    // only of nullable variables are also nullable
    bool newSymbols = true;

    // keep going until no new nullable symbols were found
    while (newSymbols) {
        unsigned int size_before = nullable.size();

        for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
            // if the key is already found to be nullable, skip them
            if (nullable.find(it->first) != nullable.end())
                continue;

            auto range = fProductions.equal_range(it->first);

            for (auto it1 = range.first; it1 != range.second; ++it1) {
                // iterate over each symbol in the body and check whether they
                // all are nullable
                bool isNullable = true;

                for (char s : it1->second) {
                    if (nullable.find(s) == nullable.end()) {
                        isNullable = false;
                        break;
                    } else {
                        continue;
                    } // end if-else
                } // end for

                // if all symbols in the body were nullable, then the head
                // is also nullable and so add them to the set of nullable 
                // symbols
                if (isNullable) 
                    nullable.insert(it->first);
            } // end for
        } // end for

        unsigned int size_after = nullable.size();

        // check whether new symbols were found
        newSymbols = (size_before == size_after) ? false : true;
    } // end while

    return nullable;
}

void CFG::eleminateEpsilonProductions() {
    // first, find all nullable symbols
    std::set<char> nullables = this->nullable();

    for (char v : nullables) { 
        // the new set of production rules for this variable
        std::multimap<char, SymbolString> newRules;

        // get the bodies of this production rule
        for (SymbolString body : this->bodies(v)) {
            // get the indexes of symbols that are nullable
            std::set<int> indexes;
            for (unsigned int i = 0; i < body.size(); ++i) {
                if (nullables.find(body.at(i)) != nullables.end())
                    indexes.insert(i);
            } // end for

            // now generate all the bodies that are a result from wheter or
            // not removing the symbols that were nullable, you'll need the
            // subset of the indexes (Binomial theorem)
            for (std::set<int> s : subset(indexes)) {
                // copy, because you don't want to touch onto the original body
                SymbolString b = body;

                // remove some nullable symbols
                int i = 0; // amount of erased symbols
                for (int k : s) {
                    b.erase(k-i, 1);
                    ++i;
                } // end for

                // skip if the body is already empty (we don't want that into
                // our new production
                if (b.empty())
                    continue;

                // add them to the new productions
                newRules.insert(std::pair<char, SymbolString>(v, b));
            } // end for
        } // end for

        // replace the old rules by the new
        fProductions.erase(v);
        fProductions.insert(newRules.begin(), newRules.end());
    } // end for
    return;
}

std::set< std::pair<char, char> > CFG::units() const {
    std::set< std::pair<char, char> > units;

    // first the base case, (A, A) for A is all variable is an unit pair
    for (char v : fVariables) {
        units.insert( std::pair<char, char>(v, v) );
    } // end for

    // now, inductive part
    for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
        for (SymbolString body : this->bodies(it->first)) {
            // if the body is of length 1 and it's a variable, then we have
            // an unit pair
            if (body.size() == 1 && fVariables.find(body.at(0)) != fVariables.end())
                units.insert(std::pair<char, char>(it->first, body.at(0)) );
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
        // the bodies from the second variable are also the bodies of first
        // variable in the unit pairs
        for (SymbolString body : this->bodies(u.second)) {
            // ignore if the body is of length 1 and is a variable (we
            // actually want to remove the unit pairs
            if (body.size() == 1 && fVariables.find(body.at(0)) != fVariables.end())
                continue;

            // add the new rule
            newProductions.insert( std::pair<char, SymbolString>(u.first, body) );
        } // end for
    } // end for

    // replace the old production rules by the new production rules
    fProductions = newProductions;
    return;
}

std::set<char> CFG::generating() const {
    // base case, all terminal symbols are generating
    std::set<char> generating = fTerminals;

    // inductive part: all variables having a production whose body consists
    // only of generating symbols are also generating
    bool newSymbols = true;

    // keep going until no new nullable symbols were found
    while (newSymbols) {
        unsigned int size_before = generating.size();

        for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
            // if the key is already generating, skip them
            if (generating.find(it->first) != generating.end())
                continue;

            for (SymbolString body : this->bodies(it->first)) {
                // iterate over each symbol in the body and check whether they
                // are generating
                bool isGenerating = true;

                for (char s : body) {
                    if (generating.find(s) == generating.end()) {
                        isGenerating = false;
                        break;
                    } else {
                        continue;
                    } // end if-else
                } // end for

                // if all symbols in the body were generating, then the head
                // is also generating and thus insert in the set of generating
                // symbols
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

    // inductive part: all symbols in the bodies from productions with as head
    // a variable that is reachable, are also reachable
    bool newSymbols = true;

    while (newSymbols) {
        unsigned int size_before = reachable.size();

        for (char s : reachable) {
            // ignore if it's a terminal, because they will never be in
            // the head of the production
            if (fTerminals.find(s) != fTerminals.end())
                continue;

            // all symbols of the bodies are also reachable
            for (SymbolString body : this->bodies(s)) {
                for (char c : body) {
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
    // first, get all the generating symbols
    std::set<char> generating = this->generating();

    // the new set of production rules
    std::multimap<char, SymbolString> newProductions;

    // first, eleminate all symbols that are not generating
    for (char v : generating) {
        // ignore if v is a terminal because rules have only a variable in the
        // head
        if (fTerminals.find(v) != fTerminals.end())
            continue;

        // only add rules whose body does not contain any non-generating symbols
        for (SymbolString body : this->bodies(v)) {
            // first check whether the body does not contain any non-generating
            // symbols, if so, skip it
            bool isGenerating = true;
            for (char c : body) {
                if (generating.find(c) == generating.end()) {
                    isGenerating = false;
                    break;
                } else {
                    continue;
                } // end if-else
            } // end for

            // add them if the body consists of generating symbols
            if (isGenerating)
                newProductions.insert( std::pair<char, SymbolString>(v, body) );
        } // end for
    } // end for

    fProductions = newProductions;
    // now we have a set of production rules only containing generating
    // symbols, our next step is to eleminate non-reachable symbols

    std::set<char> reachable = this->reachable();

    for (auto it = fProductions.begin(); it != fProductions.end(); ++it) {
        // if the key is not reachable, then remove the rules
        if (reachable.find(it->first) == reachable.end())
            fProductions.erase(it->first);
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
