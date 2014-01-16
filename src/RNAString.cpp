/*
 * RNAString.cpp
 *
 * Copyright (C) 2014 Jakob Struye
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
 * Last modified: 2 January 2014
 * By: Jakob Struye
 */

#include <string>
#include <vector>
#include "Turing.h"
#include "RNAString.h"

RNAString::RNAString() {}
RNAString::RNAString(Tape tape) {
    tape.resetHead();
    char nucleotide = (tape.getSymbolsAtHead())[0];
    while (nucleotide != 'B') {   //until blank is found
        fString.push_back(nucleotide);
        if (tape.getSymbolsAtHead()[1] == 'P')
            fLoops.push_back('0');
        else if (tape.getSymbolsAtHead()[1] == 'B' || tape.getSymbolsAtHead()[1] == 'T')
            fLoops.push_back('X');
        tape.moveHead(R);
        nucleotide = (tape.getSymbolsAtHead())[0];

    }
}

char RNAString::getLetterAt(int i) const {
    if (i < 0 || i > getSize())
        throw std::runtime_error("This RNA String has no such character");
    return fString[i];
}

char RNAString::getLoopSignAt(int i) const {
    if (i < 0 || i > getSize())
        throw std::runtime_error("This RNA String has no such character");
    return fLoops[i];
}

int RNAString::getSize() const {
    return fString.size();
}

std::ostream& operator<<(std::ostream& os, RNAString str) {
    os << str.fString << std::endl;
    os << str.fLoops;
    return os;
}


