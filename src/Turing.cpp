/*
 * Turing.cpp
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
 * Last modified: 13 November 2013
 * By: Jakob Struye
 */

#include "Turing.h"

TuringState::TuringState(const std::string& name) : fName(name) {}

bool TuringState::isCalled(const std::string& name) {
    if (fName == name)
        return true;
    return false;
}

TuringState::~TuringState() {}

TuringTransition::TuringTransition(TuringState* from, TuringState* to, const char& read, const char& write, Direction dir) :
    fFrom(from), fTo(to), fRead(read), fWrite(write), fDirection(dir) {}


bool TuringTransition::match(TuringState* state, char symbol) {
    if (fFrom == state && fRead == symbol)
        return true;
    return false;
}

TuringTransition::~TuringTransition() {}

TuringMachine::TuringMachine(const std::set<char>& alphabetTuring, const std::set<char>& alphabetTape, const char& tapeBlank) :
    fAlphabet(alphabetTuring), fTapeAlphabet(alphabetTape) {
    try {                                                          //Check if blank symbol in tape alphabet before adding
        if (alphabetTape.find(tapeBlank) == alphabetTape.end())
            throw std::runtime_error("Blank symbol not in tape alphabet");
    }
    catch (std::runtime_error& e) {
        std::cout << "Error in TuringMachine constructor: " << e.what() << std::endl;
    }
    fBlank = tapeBlank;

}

bool TuringMachine::addState(const std::string& name, const bool isFinal, const bool isStarting) {
    try {
        for (auto i : fStates)
            if (i.isCalled(name))
                throw std::runtime_error("Name is not unique!");
        if (isFinal && fStartState != nullptr)
            throw std::runtime_error("Trying to create second start state");
    }
    catch (std::runtime_error& e) {
        std::cout << "Error adding state: " << e.what() << std::endl;
    }
    /*std::pair<std::set<TuringState>::iterator, bool> pos = */
    TuringState state(name);
    fStates.insert(state);
}

TuringMachine::~TuringMachine() {}



int main () {
    std::set<char> alph;
    std::set<char> alphT;
    alph.insert('a');
    alphT.insert('a');
    alphT.insert    ('B');
    TuringMachine TM(alph, alphT, 'B');
    std::cout << "END" << std::endl;

    return 0; }
