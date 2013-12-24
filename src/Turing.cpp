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
 * Last modified: 28 November 2013
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


bool TuringState::hasThisStorage(const std::vector<char>& storage) const {
    if (storage.size() == fStorage.size())   {
        for (unsigned i=0; i < storage.size(); i++)
            if (storage[i] != fStorage[i])
                return false;
        return true;
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


TuringTransition::TuringTransition(StatePtr from, StatePtr to, const std::vector<char>& read, const std::vector<char>& write, Direction dir) :
    fFrom(from), fTo(to), fRead(read), fWrite(write), fDirection(dir) {}


bool TuringTransition::match(StatePtr state, const std::vector<char>& symbols) const {
    if (fFrom == state && std::equal(symbols.begin(), symbols.end(), fRead.begin()))
        return true;
    return false;
}


std::tuple<StatePtr, std::vector<char>, Direction> TuringTransition::getTransition() const {
    return std::make_tuple(fTo, fWrite, fDirection);
}


bool TuringTransition::isThisTransition(StatePtr from, StatePtr to, const std::vector<char>& read, const std::vector<char>& write, Direction dir) const{
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


Tape::Tape(const std::string& input, char blank, int trackCount) : fBlank(blank), fHead(0), fTrackCount(trackCount) {
    std::vector<char> cellVector;
    for (auto i : input) {    //Put input string on tape (and blank symbols on additional tracks if multitrack)
        cellVector.clear();
        cellVector.push_back(i);
        for (int j=1; j < fTrackCount; j++)
            cellVector.push_back(fBlank);
        fTape.push_back(cellVector);
    }
}


std::vector<char> Tape::getSymbolsAtHead() const {
    if (fHead < (int) fTape.size() && fHead >= 0)   //Not out of tape bounds
        return fTape[fHead];
    return std::vector<char>(fTrackCount, fBlank);     //if out of bounds; must be blank
}


void Tape::replaceSymbolsAtHead(const std::vector<char>& symbols) {
    if (fHead == (int) fTape.size())   //One spot to the right of the tape saved in fTape; add one cell to the back of tape ("overwrites a blank")
        fTape.push_back(symbols);
    else if (fHead == -1) {   //One spot to the left of the tape saved in fTape; add one cell to the front of tape ("overwrites a blank")
        fTape.push_front(symbols);
        fHead++;              //Head now at first spot of fTape instead of one to its left, increment head
    }
    else                       //Head in fTape bounds
        fTape[fHead] = symbols;
}


void Tape::moveHead(Direction dir) {
    switch(dir) {
    case L:
        fHead--;
        break;
    case R:
        fHead++;
        break;
    case U:
        //should not happen
        break;
    }
}


std::ostream& operator<<(std::ostream& output, const Tape& T) {
    output << "Tape: ";
    for (int j=0; j < T.fTrackCount; j++) {
        for (std::vector<char> i : T.fTape)
            if (i[0] != T.fBlank) //todo: delete
                std::cout << i[j];
        std::cout << std::endl << "      ";
    }
    std::cout << "Head at position: " << T.fHead;
    return output;
}


TMID::TMID(const std::string& input, StatePtr startState, char blank, int trackCount) : fTape(input, blank, trackCount), fState(startState), fTrackCount(trackCount) {}


std::pair<StatePtr, std::vector<char>> TMID::getStateAndSymbols() const {
    std::pair<StatePtr, std::vector<char>> answer;
    answer.first = fState;
    answer.second = fTape.getSymbolsAtHead();
    return answer;
}


void TMID::step(StatePtr to, const std::vector<char>& write, Direction dir) {
    fState = to;
    fTape.replaceSymbolsAtHead(write);
    fTape.moveHead(dir);
}


std::ostream& operator<<(std::ostream& output, const TMID& ID) {
    output << ID.fTape << std::endl;
    output << (*ID.fState);
    return output;
}


TuringMachine::TuringMachine() {}


TuringMachine::TuringMachine(const std::set<char>& alphabetTuring, const std::set<char>& alphabetTape, char tapeBlank) :
    fAlphabet(alphabetTuring), fTapeAlphabet(alphabetTape) {
    bool blankFound = false;          //Check if blank symbol in tape alphabet before adding
    for (auto i : alphabetTape) {
        if (i == tapeBlank)
            blankFound = true;
    }
    if (!blankFound) {
        fTapeAlphabet.insert(fBlank);
        throw std::runtime_error("Error in TuringMachine constructor: Blank symbol not in tape alphabet. Blank symbol set to 'B' and added to tape alphabet if necessary. Correct behaviour of this TM no longer guaranteed.");
    }
    fBlank = tapeBlank;
}


bool TuringMachine::addState(const std::string& name, bool isStarting, bool isFinal, const std::vector<char>& storage) {
    for (auto i : fStates) {
        if (i->isCalled(name)) {
            if (fStateStorageSize == 0)   //won't get here on first added state, so fStateStorageSize will have been set
                throw std::runtime_error("Error adding state: Name is not unique!");
            else if (i->hasThisStorage(storage))
                throw std::runtime_error("Error adding state: Name + storage is not unique!");
        }
    }
    if (isStarting && fStartState != nullptr)
        throw std::runtime_error("Error adding state: Trying to create second start state!");
    if (fStateStorageSize != -1 && fStateStorageSize != (int) storage.size())
        throw std::runtime_error ("Error adding state: All storages should have same size!");
    if (!storage.size()) {
        StatePtr newState (new TuringState(name));
        fStates.push_back(newState);
    }
    else {
        StatePtr newState (new TuringState(name, storage));
        fStates.push_back(newState);
    }
    if (fStateStorageSize == -1)       //First state to be added, dictates mandatory storage size for all states of TM
        fStateStorageSize = storage.size();
    if (isStarting)
        fStartState = (fStates.back());
    if (isFinal) {
        fAccepting.insert(fStates.back());
    }
    return true;
}


bool TuringMachine::addTransition(const std::string& from, const std::string& to, const std::vector<char>& read, const std::vector<char>& write, Direction dir,
                                  const std::vector<char>& fromStorage, const std::vector<char>& toStorage) {
    StatePtr fromPtr = nullptr;
    StatePtr toPtr = nullptr;
    if (read.size() != write.size())
        throw std::runtime_error("Error adding transition: Read and write do not have same number of characters!");
    if (fTrackCount != -1 && fTrackCount != (int) read.size())
        throw std::runtime_error("Error adding transition: Read and write character count does not match track count!");
    if (fromStorage.size() != toStorage.size())
        throw std::runtime_error("Error adding transition: Storages do not have same size!");
    if (fStateStorageSize != 1 && (int) fromStorage.size() != fStateStorageSize)
        throw std::runtime_error("Error adding transition: Storages do not have right size!");
    for (auto i : fStates) {
        if (i->isCalled(from) && i->hasThisStorage(fromStorage))
            fromPtr = i;
        if (i->isCalled(to) && i->hasThisStorage(toStorage))
            toPtr = i;
        if (fromPtr && toPtr)
            break;
    }
    if (!fromPtr)
        throw std::runtime_error("Error adding transition: From state not in set of states!");
    if (!toPtr)
        throw std::runtime_error("Error adding transition: To state not in set of states!");
    bool found = true;
    for (auto i : read) {
        if (fTapeAlphabet.find(i) == fTapeAlphabet.end())
            found = false;
        if (!found)
            break;
    }
    if (!found)
        throw std::runtime_error("Error adding transition: Symbol to be read not in tape alphabet!");
    found = true;
    for (auto i : write) {
        if (fTapeAlphabet.find(i) == fTapeAlphabet.end())
            found = false;
        if (!found)
            break;
    }
    if (!found)
        throw std::runtime_error("Error adding transition: Symbol to be written not in tape alphabet!");
    for (auto i : fTransitions) {
        if (i.isThisTransition(fromPtr, toPtr, read, write, dir))
            throw std::runtime_error("Error adding transition: Transition not unique!");
    }
    if (fTrackCount == -1)
        fTrackCount = write.size();
    fTransitions.push_back(TuringTransition(fromPtr, toPtr, read, write, dir));
    return true;
}


bool TuringMachine::addTransition(const std::string& from, const std::string& to, char read, char write, Direction dir,
                                  const std::vector<char>& fromStorage, const std::vector<char>& toStorage) {
    std::vector<char> readVec;
    std::vector<char> writeVec;
    readVec.push_back(read);
    writeVec.push_back(write);
    return addTransition(from, to, readVec, writeVec, dir, fromStorage, toStorage);
}


bool TuringMachine::indicateStartState(const std::string& name, const std::vector<char>& storage) {
    if (fStartState != nullptr) {
        std::cout << "Start state already set!" << std::endl;
        return 0;
    }
    for (auto i : fStates) {
        if (i->isCalled(name)) {
            if (i->hasThisStorage(storage)) {
                fStartState =  i;
                return 1;
            }
        }
    }
    std::cout << "Start state not found" << std::endl;
    return 0;
}


bool TuringMachine::indicateAcceptingState(const std::string& name, const std::vector<char>& storage) {
    for (auto i : fStates) {
        if (i->isCalled(name)) {
            if (i->hasThisStorage(storage)) {
                fAccepting.insert(i);
                return 1;
            }
        }
    }
    std::cout << "Accepting state not found" << std::endl;
    return 0;
}


bool TuringMachine::process(const std::string& input) const {
    if (fStartState == nullptr)
        throw std::runtime_error("No start state specified!");
    for (auto i : input) {
        if (fAlphabet.find(i) == fAlphabet.end()) {
            throw std::runtime_error("Error while processing input string: Character in input but not in input alphabet!");
        }
    }

    std::queue<TMID> fIDs;   //Queue ensures all IDs for i-th character in input are processed before moving on to IDs for (i+1)th character
    fIDs.push(TMID(input, fStartState, fBlank, fTrackCount));  //Generate first ID
    while (fIDs.size()) {               //continue processing until no IDs left or accept state reached
        TMID& currentID = fIDs.front();
        for(auto i : fTransitions) {                //Multiple valid transitions possible!
            std::pair<StatePtr, std::vector<char>> IDpair = currentID.getStateAndSymbols();    //Current state and read symbol on tape
            if (i.match(IDpair.first, IDpair.second)) {                                    //Transition for current state and symbol found
                std::tuple<StatePtr, std::vector<char>, Direction> trans = i.getTransition();    //Fetch next state, symbol to write and direction to move tape head
                if (fAccepting.find(std::get<0>(trans)) != fAccepting.end()) {             //Next state accepting --> immediately accept input
                    std::cout << currentID << std::endl; //delete
                    return 1;
                }
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


TuringPtr generateTM(const std::string& fileName) {
    TiXmlDocument doc;
    std::string path = DATADIR + fileName;
    if(!doc.LoadFile(path.c_str()))
    {
        throw std::runtime_error("Error generating TM from XML: File not found!");
    }
    TiXmlElement* root = doc.FirstChildElement();
    std::string rootName;
    if(root == NULL)
    {
        throw std::runtime_error("Error generating TM from XML: Failed to load file: No root element.");
        doc.Clear();
    }
    rootName = root->Value();
    if (rootName != "TM") {
        throw std::runtime_error("Error generating TM from XML: Not a Turing Machine XML file!");
    }

    std::set<char> alphabet;
    std::set<char> tapeAlphabet;
    bool allFound = false;
    char blank = 0;
    for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {  //find  alphabets and blank symbol
        std::string elemName = elem->Value();
        if (elemName == "InputAlphabet") {
            for(TiXmlElement* elemOfSigma = elem->FirstChildElement(); elemOfSigma != NULL; elemOfSigma = elemOfSigma->NextSiblingElement()) {
                std::string elemOfSigmaName = elemOfSigma->Value();
                if (elemOfSigmaName == "symbol") {
                    TiXmlNode* e = elemOfSigma->FirstChild();
                    TiXmlText* text = e->ToText();
                    if(text == NULL)
                        continue;
                    std::string t = text->Value();
                    if (t.size() != 1)
                        throw std::runtime_error("Error generating TM from XML: One input symbol per node please");
                    alphabet.insert(t.front());
                }
            }
        }
        if (elemName == "TapeAlphabet") {
            for(TiXmlElement* elemOfGamma = elem->FirstChildElement(); elemOfGamma != NULL; elemOfGamma = elemOfGamma->NextSiblingElement()) {
                std::string elemOfGammaName = elemOfGamma->Value();
                if (elemOfGammaName == "symbol") {
                    TiXmlNode* e = elemOfGamma->FirstChild();
                    TiXmlText* text = e->ToText();
                    if(text == NULL)
                        continue;
                    std::string t = text->Value();
                    if (t.size() != 1) {
                        throw std::runtime_error("Error generating TM from XML: One input symbol per node please");
                    }
                    tapeAlphabet.insert(t.front());
                }
            }
        }
        if (elemName == "Blank") {
            TiXmlNode* e = elem->FirstChild();
            TiXmlText* text = e->ToText();
            if(text == NULL)
                continue;
            std::string t = text->Value();
            if (t.size() != 1) {
                 throw std::runtime_error("Error generating TM from XML: One blank symbol please");
            }
            blank = t.front();
        }
        if (tapeAlphabet.size() && alphabet.size() && blank) { //All arguments necessary to construct TM found
            allFound = true;
            break;
        }
    }
    if (!allFound) {
         throw std::runtime_error("Error generating TM from XML: Alphabet, tape alphabet or blank symbol missing!");
        return nullptr;
    }
    TuringPtr TM(new TuringMachine(alphabet, tapeAlphabet, blank));

    for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {  //find  alphabets and blank symbol
        std::string elemName = elem->Value();
        if (elemName == "States") {
            const char* attr = elem->Attribute("storage");
            bool hasStorage = false;
            std::vector<std::vector<char>> storages;
            if (attr) {
                std::string statesAttr(attr);
                if (statesAttr == "true")
                    hasStorage = true;
            }
            if (hasStorage) {
                for(TiXmlElement* elemOfQ = elem->FirstChildElement(); elemOfQ != NULL; elemOfQ = elemOfQ->NextSiblingElement()) {
                    std::string elemOfQName = elemOfQ->Value();
                    if (elemOfQName == "storage") {
                        if (elemOfQ->FirstChild() == NULL) {
                            storages.push_back(std::vector<char> ());
                            continue;
                        }
                        TiXmlNode* e = elemOfQ->FirstChild();
                        TiXmlText* text = e->ToText();
                        if(text == NULL)
                            continue;
                        std::string t = text->Value();
                        std::vector<char> thisStorage;
                        for (auto i : t)
                            thisStorage.push_back(i);
                        storages.push_back(thisStorage);
                    }
                }
            }
            for(TiXmlElement* elemOfQ = elem->FirstChildElement(); elemOfQ != NULL; elemOfQ = elemOfQ->NextSiblingElement()) {
                bool isStarting = false;
                bool isAccepting = false;
                std::string elemOfQName = elemOfQ->Value();
                if (elemOfQName == "state") {
                    const char* attr = elemOfQ->Attribute("start");
                    if (attr) {
                        std::string stateAttr(attr);
                        if (stateAttr == "true")
                            isStarting = true;
                    }
                    attr = elemOfQ->Attribute("accept");
                    if (attr) {
                        std::string stateAttr(attr);
                        if (stateAttr == "true")
                            isAccepting = true;
                    }
                    if (elemOfQ->FirstChild() == NULL) {
                         throw std::runtime_error("Error generating TM from XML: State without name");
                    }
                    TiXmlNode* e = elemOfQ->FirstChild();
                    TiXmlText* text = e->ToText();
                    if(text == NULL)
                        continue;
                    std::string t = text->Value();
                    if (!hasStorage)
                        TM->addState(t, isStarting, isAccepting);
                    else
                        for (auto i : storages)
                            TM->addState(t, isStarting, isAccepting, i);
                }
            }
        }
        if (elemName == "Transitions") {
            for(TiXmlElement* elemOfDelta = elem->FirstChildElement(); elemOfDelta != NULL; elemOfDelta = elemOfDelta->NextSiblingElement()) {
                std::string elemOfDeltaName = elemOfDelta->Value();
                if (elemOfDeltaName == "transition") {
                    std::string from = "";
                    std::string to = "";
                    std::vector<char> fromStorage;
                    std::vector<char> toStorage;
                    std::vector<char> read;
                    std::vector<char> write;
                    Direction dir = U;
                    for(TiXmlElement* elemOfTransition = elemOfDelta->FirstChildElement(); elemOfTransition != NULL; elemOfTransition = elemOfTransition->NextSiblingElement()) {
                        std::string elemOfTransitionName = elemOfTransition->Value();
                        if (elemOfTransitionName == "from") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            from = t;
                        }
                        if (elemOfTransitionName == "to") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            to = t;
                        }
                        if (elemOfTransitionName == "fromStorage") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            for (auto i : t)
                                fromStorage.push_back(i);
                        }
                        if (elemOfTransitionName == "toStorage") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            for (auto i : t)
                                toStorage.push_back(i);
                        }
                        if (elemOfTransitionName == "read") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            for (auto i : t)
                                read.push_back(i);
                        }
                        if (elemOfTransitionName == "write") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            for (auto i : t)
                                write.push_back(i);
                        }
                        if (elemOfTransitionName == "dir") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            if (t == "L")
                                dir = L;
                            else if (t == "R")
                                dir = R;
                            else
                                throw std::runtime_error("Error generating TM from XML: invalid direction" );
                        }
                    }
                    if (from.size() && to.size() && read.size() && write.size() && (dir == L || dir == R))
                        TM->addTransition(from, to, read, write, dir, fromStorage, toStorage);
                    else
                         throw std::runtime_error("Error generating TM from XML: Incomplete transition");
                }
            }

        }

    }
    for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {  //find  alphabets and blank symbol
        std::string elemName = elem->Value();
        if (elemName == "StartState") {
            std::string stateName = "";
            std::vector<char> storage;
            for(TiXmlElement* elemOfSS = elem->FirstChildElement(); elemOfSS != NULL; elemOfSS = elemOfSS->NextSiblingElement()) {
                std::string elemOfSSName = elemOfSS->Value();
                if (elemOfSSName == "name") {
                    if (elemOfSS->FirstChild() == NULL) {
                        continue;
                    }
                    TiXmlNode* e = elemOfSS->FirstChild();
                    TiXmlText* text = e->ToText();
                    if(text == NULL)
                        continue;
                    stateName = text->Value();
                }
                if (elemOfSSName == "storage") {
                    if (elemOfSS->FirstChild() == NULL) {
                        continue;
                    }
                    TiXmlNode* e = elemOfSS->FirstChild();
                    TiXmlText* text = e->ToText();
                    if(text == NULL)
                        continue;
                    std::string t = text->Value();

                    for (auto i : t)
                        storage.push_back(i);
                }
            }
            if (stateName.size() != 0) {
                if (storage.size() == 0)
                    TM->indicateStartState(stateName);
                else
                    TM->indicateStartState(stateName, storage);
            }
            else
                throw std::runtime_error("Error generating TM from XML: No name for start state specified");
        }
        if (elemName == "AcceptingStates") {
            for(TiXmlElement* elemOfAccepting = elem->FirstChildElement(); elemOfAccepting != NULL; elemOfAccepting = elemOfAccepting->NextSiblingElement()) {
                std::string elemOfAcceptingName = elemOfAccepting->Value();
                if (elemOfAcceptingName == "state") {
                    std::string stateName = "";
                    std::vector<char> storage;
                    for(TiXmlElement* elemOfAccState = elemOfAccepting->FirstChildElement(); elemOfAccState != NULL; elemOfAccState = elemOfAccState->NextSiblingElement()) {
                        std::string elemOfAccStateName = elemOfAccState->Value();
                        if (elemOfAccStateName == "name") {
                            if (elemOfAccState->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfAccState->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            stateName = text->Value();
                        }
                        if (elemOfAccStateName == "storage") {
                            if (elemOfAccState->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfAccState->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();

                            for (auto i : t)
                                storage.push_back(i);
                        }
                    }
                    if (stateName.size() != 0) {
                        if (storage.size() == 0)
                            TM->indicateAcceptingState(stateName);
                        else
                            TM->indicateAcceptingState(stateName, storage);

                    }
                    else
                        throw std::runtime_error("Error generating TM from XML: No name for accepting state specified");
                }
            }
        }
    }
    return TM;
}
