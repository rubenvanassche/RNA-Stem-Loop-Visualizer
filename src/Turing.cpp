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

TuringState::TuringState(const std::string& name, const std::vector<char>& storage) : fName(name), fStorage(storage) {}


bool TuringState::isCalled(const std::string& name) const {
    if (fName == name)
        return true;
    return false;
}

bool TuringState::hasThisStorage(const std::vector<char> storage) const {
    if (storage.size() == fStorage.size())   {
        for (int i=0; i < storage.size(); i++)
            if (storage[i] != fStorage[i])
                return false;
        return true;
    }
    return false;
}


bool TuringState::operator<(const TuringState& that) const {
    if (this->fName < that.fName)
        return true;
    for (int i=0; i < this->fStorage.size(); i++) {
        if ((this->fStorage)[i] < (that.fStorage)[i])
            return true;
        if ((this->fStorage)[i] > (that.fStorage)[i])
            return false;
    }
    return false;
}

bool TuringState::operator>(const TuringState& that) const {
    if (this->fName > that.fName)
        return true;
    for (int i=0; i < this->fStorage.size(); i++) {
        if ((this->fStorage)[i] > (that.fStorage)[i])
            return true;
        if ((this->fStorage)[i] < (that.fStorage)[i])
            return false;
    }
    return false;
}

std::ostream& operator<<(std::ostream& output, const TuringState& TS) {
    output << TS.fName;
    output << "  Storage: ";
    for (auto i : TS.fStorage)
        output << i << " ";
    return output;
}


TuringState::~TuringState() {}

TuringTransition::TuringTransition(const TuringState* from, const TuringState* to, const std::vector<char>& read, const std::vector<char>& write, Direction dir) :
    fFrom(from), fTo(to), fRead(read), fWrite(write), fDirection(dir) {}


bool TuringTransition::match(const TuringState* state, std::vector<char> symbol) {
    if (fFrom == state && std::equal(symbol.begin(), symbol.end(), fRead.begin()))
        return true;
    return false;
}

std::tuple<const TuringState*, std::vector<char>, Direction> TuringTransition::getTransition() const {
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
    for (int i=0; i < this->fRead.size(); i++) {
        if ((this->fRead)[i] < (that.fRead)[i])
            return true;
        if ((this->fRead)[i] > (that.fRead)[i])
            return false;
    }
    //read symbols the same
    for (int i=0; i < this->fWrite.size(); i++) {
        if ((this->fWrite)[i] < (that.fWrite)[i])
            return true;
        if ((this->fWrite)[i] > (that.fWrite)[i])
            return false;
    }
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

bool TuringTransition::isThisTransition(const TuringState* from, const TuringState* to, const std::vector<char>& read, const std::vector<char>& write, Direction dir) const{
    if (fFrom == from && fTo == to && std::equal(fRead.begin(), fRead.end(), read.begin()) && std::equal(fWrite.begin(), fWrite.end(), write.begin()) && fDirection == dir)
        return true;
    return false;
}

std::ostream& operator<<(std::ostream& output, const TuringTransition& TT) {
    output << "From: " << (*TT.fFrom) << " To: " << (*TT.fTo);
    output << " Read: ";
    for (auto i : TT.fRead)
        output << i << " ";
    output << " Write: ";
    for (auto i : TT.fWrite)
        output << i << " ";
    if (TT.fDirection == L)
        output << " Direction: L";
    else
        output << " Direction: R";
    return output;
}


TuringTransition::~TuringTransition() {}


Tape::Tape(std::string& input, char blank, int trackCount) : fBlank(blank), fHead(0), fTrackCount(trackCount) {
    std::vector<char> cellVector;
    for (auto i : input) {
        cellVector.clear();
        cellVector.push_back(i);
        for (int j=1; j < fTrackCount; j++)
            cellVector.push_back(fBlank);
        fTape.push_back(cellVector);
    }
}

std::vector<char> Tape::getSymbolAtHead() const {
    if (fHead < fTape.size() && fHead >= 0)   //Not out of tape bounds
        return fTape[fHead];
    return std::vector<char>(fTrackCount, 'B');     //if out of bounds; must be blank
}

void Tape::replaceSymbolAtHead(std::vector<char> symbol) {
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

std::ostream& operator<<(std::ostream& output, const Tape& T) {
    output << "Tape: ";
    for (std::vector<char> i : T.fTape)
        for (auto j : i)
            std::cout << j;
    std::cout << " Head at position: " << T.fHead;
    return output;
}

TMID::TMID(std::string& input, const TuringState* startState, char blank, int trackCount) : fTape(input, blank, trackCount), fState(startState), fTrackCount(trackCount) {}


std::pair<const TuringState*, std::vector<char>> TMID::getStateAndSymbol() const {
    std::pair<const TuringState*, std::vector<char>> answer;
    answer.first = fState;
    answer.second = fTape.getSymbolAtHead();
    return answer;
}

void TMID::step(const TuringState* to, std::vector<char> write, Direction dir) {
    fState = to;
    fTape.replaceSymbolAtHead(write);
    fTape.moveHead(dir);
}

std::ostream& operator<<(std::ostream& output, const TMID& ID) {
    output << ID.fTape << " ";
    output << (*ID.fState);
    return output;
}



TuringMachine::TuringMachine(const std::set<char>& alphabetTuring, const std::set<std::vector<char>>& alphabetTape, const char& tapeBlank) :
    fAlphabet(alphabetTuring), fTapeAlphabet(alphabetTape), fTrackCount((*alphabetTape.begin()).size()) {
    try {                                                          //Check if blank symbol in tape alphabet before adding

        std::vector<char> blankVec;
        for (int i=0; i < fTrackCount; i++)
            blankVec.push_back(tapeBlank);
        bool blankFound = false;
        for (auto i : alphabetTape) {
            if (std::equal(i.begin(), i.end(), blankVec.begin()))
                blankFound = true;
            if (i.size() != fTrackCount)
                throw std::runtime_error("All tape alphabet vectors should have equal size!");
        }
        if (!blankFound) {
            std::vector<char> fBlank;
            for (int i=0; i < fTrackCount; i++)
                fBlank.push_back('B');
            blankFound = false;
            for (auto i : fTapeAlphabet)
                if (std::equal(i.begin(), i.end(), fBlank.begin()))
                    blankFound = true;
            if (!blankFound)
                fTapeAlphabet.insert(fBlank);
            throw std::runtime_error("Blank symbol not in tape alphabet. Blank symbol set to 'B' and added to tape alphabet if necessary. Correct behaviour of this TM no longer guaranteed.");
        }
    }
    catch (std::runtime_error& e) {
        std::cout << "Error in TuringMachine constructor: " << e.what() << std::endl;
        return;
    }
    fBlank = tapeBlank;

}

TuringMachine::TuringMachine(const std::set<char>& alphabetTuring, const std::set<char>& alphabetTape, const char& tapeBlank) :
    fAlphabet(alphabetTuring), fTrackCount(1) {
    std::set<std::vector<char>> alphabetVecTape;
    for(auto i : alphabetTape)
        alphabetVecTape.insert(std::vector<char> (1, i));
    fTapeAlphabet = alphabetVecTape;
    try {                                                          //Check if blank symbol in tape alphabet before adding
        std::vector<char> blankVec;
        for (int i=0; i < fTrackCount; i++)
            blankVec.push_back(tapeBlank);
        bool blankFound = false;
        for (auto i : alphabetVecTape) {
            if (std::equal(i.begin(), i.end(), blankVec.begin()))
                blankFound = true;
            if (i.size() != fTrackCount)
                throw std::runtime_error("All tape alphabet vectors should have equal size!");
        }
        if (!blankFound) {
            std::vector<char> fBlank;
            for (int i=0; i < fTrackCount; i++)
                fBlank.push_back('B');
            blankFound = false;
            for (auto i : fTapeAlphabet)
                if (std::equal(i.begin(), i.end(), fBlank.begin()))
                    blankFound = true;
            if (!blankFound)
                fTapeAlphabet.insert(fBlank);
            throw std::runtime_error("Blank symbol not in tape alphabet. Blank symbol set to 'B' and added to tape alphabet if necessary. Correct behaviour of this TM no longer guaranteed.");
        }
    }
    catch (std::runtime_error& e) {
        std::cout << "Error in TuringMachine constructor: " << e.what() << std::endl;
        return;
    }
    fBlank = tapeBlank;


}

/*TuringMachine::TuringMachine(const std::set<char>& alphabetTuring, const std::set<char>& alphabetTape, const char& tapeBlank,
                             const std::set<TuringState*>& states, const std::set<TuringTransition>& transitions,
                             const TuringState* startState, const std::set<const TuringState*>& acceptingStates) :
    fAlphabet(alphabetTuring), fTapeAlphabet(alphabetTape), fStates(states), fTransitions(transitions),
    fStartState(startState),  fAccepting(acceptingStates) {
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

}*/



bool TuringMachine::addState(const std::string& name, const bool isStarting, const bool isFinal, const std::vector<char>& storage) {
    try {
        for (auto i : fStates)     //Check if state of that name already a state
            if (i->isCalled(name))
                if (!fStateStorageSize)
                    throw std::runtime_error("Name is not unique!");
                else if (i->hasThisStorage(storage))
                    throw std::runtime_error("Name + storage is not unique!");
        if (isStarting && fStartState != nullptr)
            throw std::runtime_error("Trying to create second start state!");
        if (fStateStorageSize != -1 && fStateStorageSize != storage.size())
            throw std::runtime_error ("All storages should have same size!");
    }
    catch (std::runtime_error& e) {
        std::cout << "Error adding state: " << e.what() << std::endl;
        return 0;
    }
    std::pair<std::set<TuringState*>::iterator, bool> pos;   //Holds address of state in set and success boolean
    if (!storage.size())
        pos = fStates.insert(new TuringState(name));
    else
        pos = fStates.insert(new TuringState(name, storage));
    if (fStateStorageSize == -1)
        fStateStorageSize = storage.size();
    if (isStarting)
        fStartState = *pos.first;
    if (isFinal) {
        const TuringState* statePtr = *pos.first;
        fAccepting.insert(statePtr);
    }
    return pos.second;
}

bool TuringMachine::addTransition(const std::string& from, const std::string& to, const std::vector<char>& read, const std::vector<char>& write, Direction dir,
                                  const std::vector<char>& fromStorage, const std::vector<char>& toStorage) {
    bool storage = fromStorage.size();
    const TuringState* fromPtr = nullptr;
    const TuringState* toPtr = nullptr;
    try {
        if (read.size() != write.size())
            throw std::runtime_error("Read and write do not have same number of characters!");
        if (fTrackCount != -1 && fTrackCount != read.size())
            throw std::runtime_error("Read and write character count does not match track count!");
        if (fromStorage.size() != toStorage.size())
            throw std::runtime_error("Storages do not have same size!");
        std::set<TuringState*>::iterator it = fStates.begin();   //look for the names of the necessary states, get pointer to the state if found
        while(it != fStates.end()) {
            if ((*it)->isCalled(from) && (*it)->hasThisStorage(fromStorage))
                fromPtr = *it;
            if ((*it)->isCalled(to) && (*it)->hasThisStorage(toStorage))
                toPtr = *it;
            if (fromPtr && toPtr)
                break;
            it++;
        }
        if (!fromPtr)
            throw std::runtime_error("From state not in set of states!");
        if (!toPtr)
            throw std::runtime_error("To state not in set of states!");
        bool found = false;
        for (auto i : fTapeAlphabet)
            if (std::equal(read.begin(), read.end(), i.begin()))
                found = true;
        if (!found)
            throw std::runtime_error("Symbol to be read not in tape alphabet!");
        found = false;
        for (auto i : fTapeAlphabet)
            if (std::equal(write.begin(), write.end(), i.begin()))
                found = true;
        if (!found)
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
    if (fTrackCount == -1)
        fTrackCount = write.size();
    std::pair<std::set<TuringTransition>::iterator, bool> pos;
    pos = fTransitions.insert(TuringTransition(fromPtr, toPtr, read, write, dir));
    return pos.second;
}

bool TuringMachine::addTransition(const std::string& from, const std::string& to, const char& read, const char& write, Direction dir,
                                  const std::vector<char>& fromStorage, const std::vector<char>& toStorage) {
    std::vector<char> readVec;
    std::vector<char> writeVec;
    readVec.push_back(read);
    writeVec.push_back(write);
    addTransition(from, to, readVec, writeVec, dir, fromStorage, toStorage);
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
    fIDs.push(TMID(input, fStartState, fBlank, fTrackCount));  //Generate first ID
    while (fIDs.size()) {               //continue processing until no IDs left or accept state reached
        TMID& currentID = fIDs.front();
        for(auto i : fTransitions) {                //Multiple valid transitions possible!
            std::pair<const TuringState*, std::vector<char>> IDpair = currentID.getStateAndSymbol();    //Current state and read symbol on tape
            if (i.match(IDpair.first, IDpair.second)) {                                    //Transition for current state and symbol found
                std::tuple<const TuringState*, std::vector<char>, Direction> trans = i.getTransition();    //Fetch next state, symbol to write and direction to move tape head
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



int main () {
    std::set<char> alph;
    std::set<char> alphT;
    std::vector<char> storage;
    std::string input;
    alph.insert('0');
    alph.insert('1');
    alphT.insert('0');
    alphT.insert('1');
    alphT.insert('X');
    alphT.insert('Y');
    alphT.insert('B');
    /*TuringMachine TM(alph, alphT, 'B');;
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
    input = "00001111";
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
    alph.insert('0');
    alph.insert('1');
    alphT.insert('0');
    alphT.insert('1');
    alphT.insert('B');
    TuringMachine TM2(alph, alphT, 'B');
    storage.push_back('0');
    TM2.addState("Q0", 0, 0, storage);
    TM2.addState("Q1", 0, 0, storage);
    storage.clear();
    storage.push_back('1');
    TM2.addState("Q0", 0, 0, storage);
    TM2.addState("Q1", 0, 0, storage);
    storage.clear();
    storage.push_back('B');
    TM2.addState("Q0", 1, 0, storage);
    TM2.addState("Q1", 0, 1, storage);
    TM2.addTransition("Q0", "Q1", '0', '0', R, std::vector<char> (1, 'B'), std::vector<char> (1, '0'));
    TM2.addTransition("Q0", "Q1", '1', '1', R, std::vector<char> (1, 'B'), std::vector<char> (1, '1'));
    TM2.addTransition("Q1", "Q1", '1', '1', R, std::vector<char> (1, '0'), std::vector<char> (1, '0'));
    TM2.addTransition("Q1", "Q1", '0', '0', R, std::vector<char> (1, '1'), std::vector<char> (1, '1'));
    TM2.addTransition("Q1", "Q1", 'B', 'B', R, std::vector<char> (1, '0'), std::vector<char> (1, 'B'));
    TM2.addTransition("Q1", "Q1", 'B', 'B', R, std::vector<char> (1, '1'), std::vector<char> (1, 'B'));
    std::string input2 = "01111";
    std::cout << "01111: " << TM2.process(input2) << std::endl;
    input2 = "011110";
    std::cout << "011110: " << TM2.process(input2) << std::endl;
    input2 = "";
    std::cout << "empty: " << TM2.process(input2) << std::endl;
    input2 = "0";
    std::cout << "0: " << TM2.process(input2) << std::endl;
    input2 = "1";
    std::cout << "1: " << TM2.process(input2) << std::endl;
    input2 = "1000";
    std::cout << "1000: " << TM2.process(input2) << std::endl;
    input2 = "10001";
    std::cout << "10001: " << TM2.process(input2) << std::endl;
    input2 = "10";
    std::cout << "10: " << TM2.process(input2) << std::endl;
    input2 = "01";
    std::cout << "01: " << TM2.process(input2) << std::endl;*/
    alph.insert('c');
    std::set<std::vector<char>> alphTvec;
    std::vector<char> tempVec1;
    std::vector<char> tempVec2;
    tempVec1.push_back('0'); tempVec1.push_back('1'); tempVec1.push_back('c'); tempVec1.push_back('B');
    tempVec2.push_back('B'); tempVec2.push_back('*');
    for (auto i : tempVec1) {
        for (auto j : tempVec2) {
            std::vector<char> alphEntry;
            alphEntry.push_back(i); alphEntry.push_back(j);
            alphTvec.insert(alphEntry);
        }
    }
    TuringMachine TM3(alph, alphTvec, 'B');
    storage.clear();
    storage.push_back('0');
    TM3.addState("Q1", 0, 0, storage);
    TM3.addState("Q2", 0, 0, storage);
    TM3.addState("Q3", 0, 0, storage);
    TM3.addState("Q4", 0, 0, storage);
    TM3.addState("Q5", 0, 0, storage);
    TM3.addState("Q6", 0, 0, storage);
    TM3.addState("Q7", 0, 0, storage);
    TM3.addState("Q8", 0, 0, storage);
    TM3.addState("Q9", 0, 0, storage);
    storage.clear();
    storage.push_back('1');
    TM3.addState("Q1", 0, 0, storage);
    TM3.addState("Q2", 0, 0, storage);
    TM3.addState("Q3", 0, 0, storage);
    TM3.addState("Q4", 0, 0, storage);
    TM3.addState("Q5", 0, 0, storage);
    TM3.addState("Q6", 0, 0, storage);
    TM3.addState("Q7", 0, 0, storage);
    TM3.addState("Q8", 0, 0, storage);
    TM3.addState("Q9", 0, 0, storage);
    storage.clear();
    storage.push_back('B');
    TM3.addState("Q1", 1, 0, storage);
    TM3.addState("Q2", 0, 0, storage);
    TM3.addState("Q3", 0, 0, storage);
    TM3.addState("Q4", 0, 0, storage);
    TM3.addState("Q5", 0, 0, storage);
    TM3.addState("Q6", 0, 0, storage);
    TM3.addState("Q7", 0, 0, storage);
    TM3.addState("Q8", 0, 0, storage);
    TM3.addState("Q9", 0, 1, storage);

    std::vector<char> read;
    std::vector<char> write;
    //1.
    read.push_back('0'); read.push_back('B');
    write.push_back('0'); write.push_back('*');
    TM3.addTransition("Q1", "Q2", read, write, R, std::vector<char> (1, 'B'), std::vector<char> (1, '0'));
    read.clear(); write.clear();
    read.push_back('1'); read.push_back('B');
    write.push_back('1'); write.push_back('*');
    TM3.addTransition("Q1", "Q2", read, write, R, std::vector<char> (1, 'B'), std::vector<char> (1, '1'));
    //2.
    read.clear(); write.clear();
    read.push_back('0'); read.push_back('B');
    write.push_back('0'); write.push_back('B');
    TM3.addTransition("Q2", "Q2", read, write, R, std::vector<char> (1, '0'), std::vector<char> (1, '0'));
    read.clear(); write.clear();
    read.push_back('1'); read.push_back('B');
    write.push_back('1'); write.push_back('B');
    TM3.addTransition("Q2", "Q2", read, write, R, std::vector<char> (1, '0'), std::vector<char> (1, '0'));
    read.clear(); write.clear();
    read.push_back('0'); read.push_back('B');
    write.push_back('0'); write.push_back('B');
    TM3.addTransition("Q2", "Q2", read, write, R, std::vector<char> (1, '1'), std::vector<char> (1, '1'));
    read.clear(); write.clear();
    read.push_back('1'); read.push_back('B');
    write.push_back('1'); write.push_back('B');
    TM3.addTransition("Q2", "Q2", read, write, R, std::vector<char> (1, '1'), std::vector<char> (1, '1'));
    //3.
    read.clear(); write.clear();
    read.push_back('c'); read.push_back('B');
    write.push_back('c'); write.push_back('B');
    TM3.addTransition("Q2", "Q3", read, write, R, std::vector<char> (1, '0'), std::vector<char> (1, '0'));
    read.clear(); write.clear();
    read.push_back('c'); read.push_back('B');
    write.push_back('c'); write.push_back('B');
    TM3.addTransition("Q2", "Q3", read, write, R, std::vector<char> (1, '1'), std::vector<char> (1, '1'));
    //4.
    read.clear(); write.clear();
    read.push_back('0'); read.push_back('*');
    write.push_back('0'); write.push_back('*');
    TM3.addTransition("Q3", "Q3", read, write, R, std::vector<char> (1, '0'), std::vector<char> (1, '0'));
    read.clear(); write.clear();
    read.push_back('1'); read.push_back('*');
    write.push_back('1'); write.push_back('*');
    TM3.addTransition("Q3", "Q3", read, write, R, std::vector<char> (1, '0'), std::vector<char> (1, '0'));
    read.clear(); write.clear();
    read.push_back('0'); read.push_back('*');
    write.push_back('0'); write.push_back('*');
    TM3.addTransition("Q3", "Q3", read, write, R, std::vector<char> (1, '1'), std::vector<char> (1, '1'));
    read.clear(); write.clear();
    read.push_back('1'); read.push_back('*');
    write.push_back('1'); write.push_back('*');
    TM3.addTransition("Q3", "Q3", read, write, R, std::vector<char> (1, '1'), std::vector<char> (1, '1'));
    //5.
    read.clear(); write.clear();
    read.push_back('0'); read.push_back('B');
    write.push_back('0'); write.push_back('*');
    TM3.addTransition("Q3", "Q4", read, write, L, std::vector<char> (1, '0'), std::vector<char> (1, 'B'));
    read.clear(); write.clear();
    read.push_back('1'); read.push_back('B');
    write.push_back('1'); write.push_back('*');
    TM3.addTransition("Q3", "Q4", read, write, L, std::vector<char> (1, '1'), std::vector<char> (1, 'B'));
    //6.
    read.clear(); write.clear();
    read.push_back('0'); read.push_back('*');
    write.push_back('0'); write.push_back('*');
    TM3.addTransition("Q4", "Q4", read, write, L, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    read.clear(); write.clear();
    read.push_back('1'); read.push_back('*');
    write.push_back('1'); write.push_back('*');
    TM3.addTransition("Q4", "Q4", read, write, L, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    //7.
    read.clear(); write.clear();
    read.push_back('c'); read.push_back('B');
    write.push_back('c'); write.push_back('B');
    TM3.addTransition("Q4", "Q5", read, write, L, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    //8.
    read.clear(); write.clear();
    read.push_back('0'); read.push_back('B');
    write.push_back('0'); write.push_back('B');
    TM3.addTransition("Q5", "Q6", read, write, L, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    read.clear(); write.clear();
    read.push_back('1'); read.push_back('B');
    write.push_back('1'); write.push_back('B');
    TM3.addTransition("Q5", "Q6", read, write, L, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    //9.
    read.clear(); write.clear();
    read.push_back('0'); read.push_back('B');
    write.push_back('0'); write.push_back('B');
    TM3.addTransition("Q6", "Q6", read, write, L, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    read.clear(); write.clear();
    read.push_back('1'); read.push_back('B');
    write.push_back('1'); write.push_back('B');
    TM3.addTransition("Q6", "Q6", read, write, L, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    //10.
    read.clear(); write.clear();
    read.push_back('0'); read.push_back('*');
    write.push_back('0'); write.push_back('*');
    TM3.addTransition("Q6", "Q1", read, write, R, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    read.clear(); write.clear();
    read.push_back('1'); read.push_back('*');
    write.push_back('1'); write.push_back('*');
    TM3.addTransition("Q6", "Q1", read, write, R, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    //11.
    read.clear(); write.clear();
    read.push_back('0'); read.push_back('*');
    write.push_back('0'); write.push_back('*');
    TM3.addTransition("Q5", "Q7", read, write, R, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    read.clear(); write.clear();
    read.push_back('1'); read.push_back('*');
    write.push_back('1'); write.push_back('*');
    TM3.addTransition("Q5", "Q7", read, write, R, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    //12.
    read.clear(); write.clear();
    read.push_back('c'); read.push_back('B');
    write.push_back('c'); write.push_back('B');
    TM3.addTransition("Q7", "Q8", read, write, R, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    //13.
    read.clear(); write.clear();
    read.push_back('0'); read.push_back('*');
    write.push_back('0'); write.push_back('*');
    TM3.addTransition("Q8", "Q8", read, write, R, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    read.clear(); write.clear();
    read.push_back('1'); read.push_back('*');
    write.push_back('1'); write.push_back('*');
    TM3.addTransition("Q8", "Q8", read, write, R, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    //14.
    read.clear(); write.clear();
    read.push_back('B'); read.push_back('B');
    write.push_back('B'); write.push_back('B');
    TM3.addTransition("Q8", "Q9", read, write, R, std::vector<char> (1, 'B'), std::vector<char> (1, 'B'));
    input = "010c010";
    std::cout << "010c010: " << TM3.process(input) << std::endl;



    return 0;
}
