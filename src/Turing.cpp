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

bool TuringState::isCalled(const std::string& name) const {
    if (fName == name)
        return true;
    return false;
}


bool TuringState::operator<(const TuringState& that) const {
    if (this->fName < that.fName)
        return true;
    return false;
}

bool TuringState::operator>(const TuringState& that) const {
    if (this->fName > that.fName)
        return true;
    return false;
}

std::ostream& operator<<(std::ostream& output, const TuringState& TS) {
    output << TS.fName;
    return output;
}


TuringState::~TuringState() {}

TuringTransition::TuringTransition(const TuringState* from, const TuringState* to, const char& read, const char& write, Direction dir) :
    fFrom(from), fTo(to), fRead(read), fWrite(write), fDirection(dir) {}


bool TuringTransition::match(const TuringState* state, char symbol) {
    if (fFrom == state && fRead == symbol)
        return true;
    return false;
}

std::tuple<const TuringState*, char, Direction> TuringTransition::getTransition() const {
    return std::make_tuple(fTo, fWrite, fDirection);
}


bool TuringTransition::operator<(const TuringTransition& that) const {
    if (*(this->fFrom) < *(that.fFrom))
        return true;
    if (*(this->fFrom) > *(that.fFrom))
        return false;
    //from states are the same
    if (*(this->fTo) < *(that.fTo))
        return true;
    if (*(this->fTo) > *(that.fTo))
        return false;
    //to states are the same
    if (this->fRead < that.fRead)
        return true;
    if (this->fRead > that.fRead)
        return false;
    //read symbols the same
    if (this->fWrite < that.fWrite)
        return true;
    if (this->fWrite > that.fWrite)
        return false;
    //write symbols the same
    switch (this->fDirection) {
    case L:
        return true;
        break;
    default:
        return false;
        break;
    }
    return false;

}

bool TuringTransition::isThisTransition(const TuringState* from, const TuringState* to, const char& read, const char& write, Direction dir) const{
    if (fFrom == from && fTo == to && fRead == read && fWrite == write && fDirection == dir)
        return true;
    return false;
}


TuringTransition::~TuringTransition() {}


Tape::Tape(std::string& input, char blank) : fBlank(blank), fHead(0) {
    for (auto i : input)
        fTape.push_back(i);
}

char Tape::getSymbolAtHead() const {
    if (fHead < fTape.size() && fHead >= 0)   //Not out of tape bounds
        return fTape[fHead];
    return fBlank;     //if out of bounds; must be blank
}

void Tape::replaceSymbolAtHead(char symbol) {
    if (fHead == fTape.size())   //One spot to the right of the tape saved in fTape; add one symbol to the back of tape ("overwrites a blank")
        fTape.push_back(symbol);
    else if (fHead == -1) {   //One spot to the left of the tape saved in fTape; add one symbol to the front of tape ("overwrites a blank")
        fTape.push_front(symbol);
        fHead++;              //Head now at first spot of fTape instead of one to its left, increment head
    }
    else                       //Head in fTape bounds
        fTape[fHead] = symbol;

}

void Tape::moveHead(Direction dir) {
    switch(dir) {
    case L:
        fHead--;
        break;
    case R:
        fHead++;
        break;
    }
}

TMID::TMID(std::string& input, const TuringState* startState, char blank) : fTape(input, blank), fState(startState) {}


std::pair<const TuringState*, char> TMID::getStateAndSymbol() const {
    std::pair<const TuringState*, char> answer;
    answer.first = fState;
    answer.second = fTape.getSymbolAtHead();
    return answer;
}

void TMID::step(const TuringState* to, char write, Direction dir) {
    fState = to;
    fTape.replaceSymbolAtHead(write);
    fTape.moveHead(dir);
}




TuringMachine::TuringMachine(const std::set<char>& alphabetTuring, const std::set<char>& alphabetTape, const char& tapeBlank) :
    fAlphabet(alphabetTuring), fTapeAlphabet(alphabetTape) {
    try {                                                          //Check if blank symbol in tape alphabet before adding
        if (alphabetTape.find(tapeBlank) == alphabetTape.end()) {
            fBlank = 'B';
            if (fTapeAlphabet.find('B') == fTapeAlphabet.end())
                fTapeAlphabet.insert('B');
            throw std::runtime_error("Blank symbol not in tape alphabet. Blank symbol set to 'B' and added to tape alphabet if necessary. Correct behaviour of this TM no longer guaranteed.");
        }
    }
    catch (std::runtime_error& e) {
        std::cout << "Error in TuringMachine constructor: " << e.what() << std::endl;
        return;
    }
    fBlank = tapeBlank;

}

bool TuringMachine::addState(const std::string& name, const bool isStarting, const bool isFinal) {
    try {
        for (auto i : fStates)     //Check if state of that name already a state
            if (i->isCalled(name))
                throw std::runtime_error("Name is not unique!");
        if (isStarting && fStartState != nullptr)
            throw std::runtime_error("Trying to create second start state");
    }
    catch (std::runtime_error& e) {
        std::cout << "Error adding state: " << e.what() << std::endl;
        return 0;
    }
    std::pair<std::set<TuringState*>::iterator, bool> pos;   //Holds address of state in set and success boolean
    pos = fStates.insert(new TuringState(name));
    if (isStarting)
        fStartState = *pos.first;
    if (isFinal) {
        const TuringState* statePtr = *pos.first;
        fAccepting.insert(statePtr);
    }
    return pos.second;
}

bool TuringMachine::addTransition(const std::string& from, const std::string& to, const char& read, const char& write, Direction dir) {
    const TuringState* fromPtr = nullptr;
    const TuringState* toPtr = nullptr;
    try {
        std::set<TuringState*>::iterator it = fStates.begin();   //look for the names of the necessary states, get pointer to the state if found
        while(it != fStates.end()) {
            if ((*it)->isCalled(from))
                fromPtr = *it;
            if ((*it)->isCalled(to))
                toPtr = *it;
            if (fromPtr && toPtr)
                break;
            it++;
        }
        if (!fromPtr)
            throw std::runtime_error("From state not in set of states!");
        if (!toPtr)
            throw std::runtime_error("To state not in set of states!");
        if (fTapeAlphabet.find(read) == fTapeAlphabet.end())
            throw std::runtime_error("Symbol to be read not in tape alphabet!");
        if (fTapeAlphabet.find(write) == fTapeAlphabet.end())
            throw std::runtime_error("Symbol to be written not in tape alphabet!");
        for (auto i : fTransitions) {
            if (i.isThisTransition(fromPtr, toPtr, read, write, dir))
                throw std::runtime_error("Transition not unique!");
        }

    }
    catch (std::runtime_error& e) {
        std::cout << "Error adding transition: " << e.what() << std::endl;
        return 0;
    }
    std::pair<std::set<TuringTransition>::iterator, bool> pos;
    pos = fTransitions.insert(TuringTransition(fromPtr, toPtr, read, write, dir));
    return pos.second;
}

const TuringState* TuringMachine::getStatePtr(std::string& name) const{
    std::set<TuringState*>::iterator it = fStates.begin();
    while (it != fStates.end()) {
        if ((*it)->isCalled(name))
            return *it;
        it++;
    }
    return nullptr;
}

bool TuringMachine::process(std::string& input) {
    try {
        for (auto i : input) {
            if (fAlphabet.find(i) == fAlphabet.end()) {
                throw std::runtime_error("Character in input but not in input alphabet!");
            }
        }
    }
    catch (std::runtime_error& e) {
        std::cout << "Error while processing input string: " << e.what() << std::endl;
        return 0;
    }
    std::queue<TMID> fIDs;   //Queue ensures all IDs for i-th character in input are processed before moving on to IDs for (i+1)th character
    fIDs.push(TMID(input, fStartState, fBlank));  //Generate first ID
    while (fIDs.size()) {               //continue processing until no IDs left or accept state reached
        TMID& currentID = fIDs.front();
        for(auto i : fTransitions) {                //Multiple valid transitions possible!
            std::pair<const TuringState*, char> IDpair = currentID.getStateAndSymbol();    //Current state and read symbol on tape
            if (i.match(IDpair.first, IDpair.second)) {                                    //Transition for current state and symbol found
                std::tuple<const TuringState*, char, Direction> trans = i.getTransition();    //Fetch next state, symbol to write and direction to move tape head
                if (fAccepting.find(std::get<0>(trans)) != fAccepting.end())              //Next state accepting --> immediately accept input
                    return 1;
                TMID newID = currentID;                                                   //copy current ID (through default copy constructor, which does the job in this case)
                newID.step(std::get<0>(trans), std::get<1>(trans), std::get<2>(trans));   //Apply transition to copied ID
                fIDs.push(newID);                                                         //And finally add to the queue
            }
        }
        fIDs.pop();
    }
    return 0;
}

TuringMachine::~TuringMachine() {}


/*
int main () {
    std::set<char> alph;
    std::set<char> alphT;
    alph.insert('0');
    alph.insert('1');
    alphT.insert('0');
    alphT.insert('1');
    alphT.insert('X');
    alphT.insert('Y');
    alphT.insert('B');
    TuringMachine TM(alph, alphT, 'B');
    TM.addState("Q0", 1, 0);
    TM.addState("Q1", 0, 0);
    TM.addState("Q2", 0, 0);
    TM.addState("Q3", 0, 0);
    TM.addState("Q4", 0, 1);
    TM.addTransition("Q0", "Q1", '0', 'X', R);
    TM.addTransition("Q0", "Q3", 'Y', 'Y', R);
    TM.addTransition("Q1", "Q1", '0', '0', R);
    TM.addTransition("Q1", "Q2", '1', 'Y', L);
    TM.addTransition("Q1", "Q1", 'Y', 'Y', R);
    TM.addTransition("Q2", "Q2", '0', '0', L);
    TM.addTransition("Q2", "Q0", 'X', 'X', R);
    TM.addTransition("Q2", "Q2", 'Y', 'Y', L);
    TM.addTransition("Q3", "Q3", 'Y', 'Y', R);
    TM.addTransition("Q3", "Q4", 'B', 'B', R);
    std::string input = "00001111";
    std::cout << "00001111: " << TM.process(input) << std::endl;
    input = "000011111";
    std::cout << "000011111: " << TM.process(input) << std::endl;
    input = "0000111";
    std::cout << "0000111: " << TM.process(input) << std::endl;
    input = "0001111";
    std::cout << "0001111: " << TM.process(input) << std::endl;
    input = "000001111";
    std::cout << "000001111: " << TM.process(input) << std::endl;
    input = "00000";
    std::cout << "00000: " << TM.process(input) << std::endl;
    input = "1";
    std::cout << "1: " << TM.process(input) << std::endl;
    input = "10";
    std::cout << "10: " << TM.process(input) << std::endl;
    input = "";
    std::cout << "empty: " << TM.process(input) << std::endl;
    input = "01";
    std::cout << "01: " << TM.process(input) << std::endl;
    input = "0101";
    std::cout << "0101: " << TM.process(input) << std::endl;
    input = "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
    std::cout << "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111: " << TM.process(input) << std::endl;
    return 0; }*/
