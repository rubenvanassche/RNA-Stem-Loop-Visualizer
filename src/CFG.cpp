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
 * Last modified: 15 November 2013
 * By: Stijn Wouters
 */
#include "CFG.h"

CFG::CFG(
    const std::set<char>& terminals,
    const std::set<char>& variables,
    const std::multimap<char, SymbolString>& productions,
    const char& start
    ) : fTerminals(terminals), fVariables(variables), fProductions(productions), fStartSymbol(start) {
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
