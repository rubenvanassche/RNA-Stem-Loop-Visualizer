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
template <class T>
std::set< std::set<T> > subset(std::set<T>& s) {
    if (s.empty()) {
        // base case
        return { std::set<T>{} };
    } else {
        // recursion: remove one element from the subset
        T i = *(s.begin());
        s.erase(s.begin());

        // get all the subset of the set without the first element
        std::set< std::set<T> > recursion = subset(s);

        // now, add the removed element in all those subset found
        std::set< std::set<T> > result;

        for (std::set<T> r : recursion) {
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

    // the intersection of the set of variables and the set of terminals
    // should be empty, since they should be disjoint
    std::set<char> intersection;
    std::set_intersection(
                    fTerminals.begin(), fTerminals.end(),
                    fVariables.begin(), fVariables.end(),
                    std::inserter(intersection, intersection.begin())
                    );

    if (!intersection.empty())
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
    std::set<char> nullables;

    unsigned int size_before = nullables.size();

    // base case: all variables having an production A -> "" is surely nullable
    for (char v : fVariables) {
        for (SymbolString body : this->bodies(v)) {
            if (body.empty()) {
                nullables.insert(v);
                break;
            } else {
                continue;
            } // end if-else
        } // end for
    } // end for

    unsigned int size_after = nullables.size();

    // inductive part: all variables having a production whose body consists
    // only of nullable variables are also nullable

    // keep going until no new symbols were found
    while (size_before != size_after) {
        size_before = nullables.size();

        for (char v : fVariables) {
            // if the key is already found to be nullable, skip them
            if (nullables.find(v) != nullables.end())
                continue;

            for (SymbolString body : this->bodies(v)) {
                // iterate over each symbol in the body and check whether they
                // all are nullable
                bool isNullable = true;

                for (char s : body) {
                    if (nullables.find(s) == nullables.end()) {
                        isNullable = false;
                        break;
                    } else {
                        continue;
                    } // end if-else
                } // end for

                // if all symbols in the body were nullable, then the head
                // is also nullable and so add them to the set of nullable 
                // symbols
                if (isNullable) {
                    nullables.insert(v);
                    break;
                } else {
                    continue;
                } // end if-else
            } // end for
        } // end for

        size_after = nullables.size();
    } // end while

    return nullables;
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
                // our new production)
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
    for (char v : fVariables) {
        for (SymbolString body : this->bodies(v)) {
            // if the body is of length 1 and it's a variable, then we have
            // an unit pair
            try {
                char c = body.at(0);

                if (body.size() == 1 && fVariables.find(c) != fVariables.end())
                    units.insert(std::pair<char, char>(v, body.at(0)) );
            } catch (const std::out_of_range& oor) {
                // ignore this error (occurs only in empty bodies
                // but that's not interesting for finding unit pairs
                // anyway...
                continue;
            } // end try-catch
        } // end for
    } // end for

    return units;
}

void CFG::eleminateUnitProductions() {
    // first, get all unit pairs
    std::set< std::pair<char, char> > units = this->units();

    // check whether there are no circular unit pairs
    // TODO

    // the new set of production rules
    std::multimap<char, SymbolString> newProductions;

    for (std::pair<char, char> u : units) {
        // the bodies from the second variable are also the bodies of first
        // variable in the unit pairs
        // note that this never throws an exception, since unit pairs consists
        // of variables only
        for (SymbolString body : this->bodies(u.second)) {
            // ignore if the body is of length 1 and is a variable (we
            // actually want to remove the unit productions and not creating
            // another new one)
            if (body.size() == 1 && fVariables.find(body.at(0)) != fVariables.end())
                continue;

            // add the new rule
            newProductions.insert(std::pair<char, SymbolString>(u.first, body));
        } // end for
    } // end for

    // replace the old production rules by the new production rules
    fProductions = newProductions;
    return;
}

std::set<char> CFG::generating() const {
    unsigned int size_before = 0;

    // base case, all terminal symbols are generating
    std::set<char> generating = fTerminals;

    unsigned int size_after = generating.size();

    // inductive part: all variables having a production whose body consists
    // only of generating symbols are also generating

    // keep going until no new nullable symbols were found
    while (size_before != size_after) {
        size_before = generating.size();

        for (char v : fVariables) {
            // if the variable is already generating, skip them
            if (generating.find(v) != generating.end())
                continue;

            for (SymbolString body : this->bodies(v)) {
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
                if (isGenerating) {
                    generating.insert(v);
                    break;
                } else {
                    continue;
                } // end if-else
            } // end for
        } // end for

        size_after = generating.size();
    } // end for

    return generating;
}

std::set<char> CFG::reachable() const {
    unsigned int size_before = 0;

    // base case: the start symbol is surely reachable
    std::set<char> reachable = {fStartSymbol};

    unsigned int size_after = reachable.size();

    // inductive part: all symbols in the bodies from productions with as head
    // a variable that is reachable, are also reachable
    while (size_before != size_after) {
        size_before = reachable.size();

        for (char s : reachable) {
            try {
                // all symbols of the bodies are also reachable
                for (SymbolString body : this->bodies(s)) {
                    for (char c : body) {
                        reachable.insert(c);
                    } // end for
                } // end for
            } catch (const std::invalid_argument& e) {
                // you only reach here if the symbol s was a terminal (and
                // thus cannot have a body in the production rules
                continue;
            } // end try-catch
        } // end for

        size_after = reachable.size();
    } // end for

    return reachable;
}

void CFG::eleminateUselessSymbols() {
    // first, get all the generating symbols
    std::set<char> generating = this->generating();

    // the new set of production rules
    std::multimap<char, SymbolString> newProductions;

    // first, eleminate all symbols that are not generating
    for (char s : generating) {
        try {
            // only add rules whose body does not contain any 
            // non-generating symbols
            for (SymbolString body : this->bodies(s)) {
                // first check whether the body does not contain
                // any non-generating symbols, if so, skip it
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
                    newProductions.insert(std::pair<char, SymbolString>(s, body));
            } // end for
        } catch (const std::invalid_argument& e) {
            // you only reach here if the symbol s was a terminal (and
            // thus cannot have a body in the production rules
            continue;
        } // end try-catch
    } // end for

    fProductions = newProductions;
    // now we have a set of production rules only containing generating
    // symbols, our next step is to eleminate non-reachable symbols

    // get the set of all reachable symbols
    std::set<char> reachable = this->reachable();

    for (char v : fVariables) {
        // if the variable is not reachable, then remove the rules
        // and also remove it from the set of variables, because it's
        // useless now
        if (reachable.find(v) == reachable.end())
            fProductions.erase(v);
    } // end for

    return;
}

void CFG::cleanUp() {
    // this one is easy now, just clean up in this order
    this->eleminateEpsilonProductions();
    this->eleminateUnitProductions();
    this->eleminateUselessSymbols();

    // now, also remove all symbols from the set of the variables which
    // don't have any production rule
    for (char v : fVariables) {
        if ((this->bodies(v)).empty())
            fVariables.erase(v);
    } // end for
    return;
}
