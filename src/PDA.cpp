/*
 * PDA.cpp
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
 * Last modified: 8 November 2013
 * By: Ruben Van Assche*
 */

#include "PDA.h"

PDAState::PDAState(std::string name){
	this->fName = name;
	this->fFinal = false;
}

PDAState::PDAState(std::string name, bool isFinal){
	this->fName = name;
	this->fFinal = isFinal;
}

bool PDAState::isFinal() const{
	return this->fFinal;
}

bool PDAState::operator==(const PDAState& other){
	if(other.getName() == this->fName and other.isFinal() == this->isFinal()){
		return true;
	}else{
		return false;
	}
}

std::string PDAState::getName() const{
	return this->fName;
}

std::ostream& operator<<(std::ostream& out, PDAState state){
	out << "State " << state.getName();
	if(state.isFinal() == true){
		out << " (final)";
	}
	out << std::endl;
	return out;
}

PDAState::~PDAState(){

}

PDATransition::PDATransition(PDAState* from, PDAState* to, const char& input, const char& stackTop, const PDAStackOperation& stackOperation) {
	this->fFrom = from;
	this->fTo = to;
	this->fInputSymbol = input;
	this->fTopStack = stackTop;
	this->fStackOperation = stackOperation;
}

PDATransition::PDATransition(PDAState* from, PDAState* to, const char& input, const char& stackTop, const PDAStackOperation& stackOperation, const char& stackPush) {
	this->fFrom = from;
	this->fTo = to;
	this->fInputSymbol = input;
	this->fTopStack = stackTop;
	this->fPushStack.push_back(stackPush);
	this->fStackOperation = stackOperation;
}

PDATransition::PDATransition(PDAState* from, PDAState* to, const char& input, const char& stackTop, const PDAStackOperation& stackOperation, const std::vector<char>& stackPush){
	this->fFrom = from;
	this->fTo = to;
	this->fInputSymbol = input;
	this->fTopStack = stackTop;
	this->fPushStack.insert(this->fPushStack.end(), stackPush.begin(), stackPush.end());
	this->fStackOperation = stackOperation;
}

void PDATransition::stackOperation(std::stack<char>& in){
	if(this->fStackOperation == PUSH){
		for(auto pushStackIt = this->fPushStack.begin(); pushStackIt != this->fPushStack.end();pushStackIt++){
			in.push(*pushStackIt);
		}
	}else if(this->fStackOperation == POP){
		if(in.size() == 0){
			throw std::runtime_error("Try to pop something from an empty stack");
			return;
		}

		if(in.top() == this->fTopStack){
			in.pop();
		}else{
			throw std::runtime_error("Try to pop an element which is not at the top of the stack according to the transition info");
		}
	}else if(this->fStackOperation == STAY){
		// do nothing
	}else if(this->fStackOperation == EMPTY){
		if(in.size() == 0){
			//throw std::runtime_error("Try to pop something from an empty stack");
			return;
		}

		in.empty();
		in.push(9);
	}
}

bool PDATransition::operator==(const PDATransition& other){
	if(other.fFrom == this->fFrom and other.fTo == this->fTo and other.fInputSymbol == this->fInputSymbol and other.fPushStack == this->fPushStack and other.fTopStack == this->fTopStack){
		return true;
	}else{
		return false;
	}
}

std::ostream& operator<<(std::ostream& out, PDATransition transition){
	out << "Transition " << transition.fFrom->getName() << " -> " << transition.fTo->getName() << " (";
	if(transition.fInputSymbol != 0){
		out << transition.fInputSymbol << ")" << std::endl;
	}else{
		out << "[e]" << ")" << std::endl;
	}
	if(transition.fPushStack.size() == 0){
		// nothing should happen
		out << "when top stack is: " << transition.fTopStack << " nothing happens" << std::endl;
	}else{
		if(transition.fPushStack.size() == 1 and transition.fPushStack.at(0) == 9){
			// Epsilon so pop value from stack
			out << "When top stack is: " << transition.fTopStack << " this value will be popped" << std::endl;
		}else if(transition.fPushStack.size() == 1 and transition.fPushStack.at(0) == transition.fTopStack){
			// a/a nothing to do the stack stays the same
			out << "when top stack is: " << transition.fTopStack << " it stays the same" << std::endl;
		}else{
			// just push everything on the stack
			out << "when top stack is: " << transition.fTopStack << " push: ";
			for(auto pushStackIt = transition.fPushStack.begin(); pushStackIt != transition.fPushStack.end();pushStackIt++){
				out << *pushStackIt << ",";
			}
			out << std::endl;
		}
	}
	return out;
}

PDATransition::~PDATransition() {
    // TODO Auto-generated destructor stub
}

PDAID::PDAID(const std::string& input, PDAState* currentState, const std::stack<char> stack){
	this->fInput = input;
	this->fState = currentState;
	this->fStack = stack;
}

void PDAID::step(const std::string& input, PDAState* to, const std::stack<char> stack){
	this->fInput = input;
	this->fState = to;
	this->fStack = stack;
}

bool PDAID::isAccepted(PDAFinal pdaType){
	bool accepted = false;
	if(this->fInput.size() == 0){
		if(pdaType == STATE and this->fState->isFinal()){
			// PDA is state ending
			accepted = true;
		}else if(pdaType == STACK and this->fStack.size() == 0){
			// PDA is stack ending
			accepted = true;
		}else if(pdaType == STACK and this->fStack.size() == 1 and this->fStack.top() == 9){
			// PDA is stack ending
						accepted = true;
		}
	}

	return accepted;
}


std::ostream& operator<<(std::ostream& out, PDAID id){
	std::stack<char> temp = id.fStack;

	out << "PDA ID(" << id.fInput << ", " << id.fState->getName() << ", ";
	std::cout << temp.size() << ": ";
	for(unsigned int i = 0; i <= temp.size(); i++){
		if(temp.top() == 9){
			out << "[Z0]";
			temp.pop();
		}else if(temp.top() == 0){
			out << "[e]";
			temp.pop();
		}else{
			out << temp.top();
			temp.pop();
		}
	}
	out << ")";
	return out;
}


PDA::PDA(const std::set<char>& alphabetPDA, const std::set<char>& alphabetStack, const PDAFinal& PDAending){
	this->fAlphabet = alphabetPDA;
	this->fStackAlphabet = alphabetStack;
	this->fPDAtype = PDAending;

	// Now let's initiate the stack with the Z0 symbol which is char = 9;
	this->fStack.push(9);
}

PDA::PDA(CFG cfg){
	std::set<char> terminals = cfg.getTerminals();
	std::set<char> variables = cfg.getVariables();
	std::multimap<char, SymbolString> productions = cfg.getProductions();
	char startSymbol = cfg.getStartsymbol();

	// First add a state q;
	PDAState q("q");
	this->addState(q, true);

	// Add the alphabet
	this->fAlphabet = terminals;

	// Add the stack alphabet
	this->fStackAlphabet = terminals;
	this->fStackAlphabet.insert(variables.begin(), variables.end());

	// Set the type of PDA
	this->fPDAtype = STACK;

	// Add initial z0 to the stack
	this->fStack.push(9);

	// Add the productions based upon variables
	for(auto it = productions.begin();it != productions.end();it++){
		PDAState* from = &q;
		PDAState* to = &q;
		char input = 0;
		char stacktop;
		if(it->first != startSymbol){
			stacktop = it->first;
		}else{
			stacktop = 9;
		}
		PDAStackOperation operation = PUSH;
		std::vector<char> push;
		for(auto it2 = it->second.begin();it2 != it->second.end();it2++){
			if(*it2 != startSymbol){
				push.push_back(*it2);
			}else{
				push.push_back(9);
			}
		}

		PDATransition transition(from, to, input, stacktop, operation, push);
		std::cout << transition << std::endl;
		this->addTransition(transition);
	}

	// add the productions based upon terminals
	for(auto it = terminals.begin();it != terminals.end();it++){
		PDAState* from = &q;
		PDAState* to = &q;
		char input = *it;
		char stacktop = *it;
		PDAStackOperation operation = POP;

		PDATransition transition(from, to, input, stacktop, operation);
		this->addTransition(transition);
	}

}


PDA::PDA(const std::string& fileName){
	// Load from XML: time for some fancy stuff woop woop woop

    TiXmlDocument doc;
    std::string path = fileName;
    if(!doc.LoadFile(path.c_str())){
        throw std::runtime_error("Error generating PDA from XML: File not found!");
    }

    TiXmlElement* root = doc.FirstChildElement();
    std::string rootName;

    if(root == NULL){
        throw std::runtime_error("Error generating PDA from XML: Failed to load file: No root element.");
        doc.Clear();
    }

    rootName = root->Value();
    if (rootName != "PDA") {
        throw std::runtime_error("Error generating PDA from XML: Not a Turing Machine XML file!");
    }

    std::set<char> alphabet;
    std::set<char> stackAlphabet;
    bool allFound = false;
    PDAFinal type;
    for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {  //find  alphabets and blank symbol
        std::string elemName = elem->Value();
        if (elemName == "Alphabet") {
            for(TiXmlElement* elemOfSigma = elem->FirstChildElement(); elemOfSigma != NULL; elemOfSigma = elemOfSigma->NextSiblingElement()) {
                std::string elemOfSigmaName = elemOfSigma->Value();
                if (elemOfSigmaName == "symbol") {
                    TiXmlNode* e = elemOfSigma->FirstChild();
                    TiXmlText* text = e->ToText();
                    if(text == NULL)
                        continue;
                    std::string t = text->Value();
                    if (t.size() != 1)
                        throw std::runtime_error("Error generating PDA from XML: One input symbol per node please");
                    alphabet.insert(t.front());
                }
            }
        }
        if (elemName == "StackAlphabet") {
            for(TiXmlElement* elemOfGamma = elem->FirstChildElement(); elemOfGamma != NULL; elemOfGamma = elemOfGamma->NextSiblingElement()) {
                std::string elemOfGammaName = elemOfGamma->Value();
                if (elemOfGammaName == "symbol") {
                    TiXmlNode* e = elemOfGamma->FirstChild();
                    TiXmlText* text = e->ToText();
                    if(text == NULL)
                        continue;
                    std::string t = text->Value();
                    if (t.size() != 1) {
                        throw std::runtime_error("Error generating PDA from XML: One input symbol per node please");
                    }
                    stackAlphabet.insert(t.front());
                }
            }
        }
        if (elemName == "Type") {
            TiXmlNode* e = elem->FirstChild();
            TiXmlText* text = e->ToText();
            if(text == NULL)
                continue;
            std::string t = text->Value();
            if (t == "STACK") {
                 type = STACK;
            }else if(t == "STATE"){
            	type = STATE;
            }else{
            	throw std::runtime_error("Error generating PDA from XML: the type should be STATE or STACK not something else");
            }
        }
        if (stackAlphabet.size() && alphabet.size() && (type == STACK || type==STATE)) { //All arguments necessary to construct PDA found
            allFound = true;
            break;
        }
    }
    if (!allFound) {
         throw std::runtime_error("Error generating PDA from XML: Alphabet, Stack Alphabet or Type symbol missing!");
    }

    this->fAlphabet = alphabet;
    this->fStackAlphabet = stackAlphabet;
    this->fPDAtype = type;

    std::list<PDAState> states;

    for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {  //find  alphabets and blank symbol
        std::string elemName = elem->Value();
        if (elemName == "States") {
            for(TiXmlElement* elemOfDelta = elem->FirstChildElement(); elemOfDelta != NULL; elemOfDelta = elemOfDelta->NextSiblingElement()) {
                std::string elemOfDeltaName = elemOfDelta->Value();
                if (elemOfDeltaName == "state") {
                    std::string name = "";
                    bool accepting = false;
                    bool starting = false;
                    for(TiXmlElement* elemOfTransition = elemOfDelta->FirstChildElement(); elemOfTransition != NULL; elemOfTransition = elemOfTransition->NextSiblingElement()) {
                        std::string elemOfTransitionName = elemOfTransition->Value();
                        if (elemOfTransitionName == "name") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            name = t;
                        }
                        if (elemOfTransitionName == "accepting") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            if(t == "yes"){
                            	accepting = true;
                            }else if(t == "no"){
                            	accepting = false;
                            }else{
                            	throw std::runtime_error("Error generating PDA from XML: a state accepting can only be yes or no!");
                            }
                        }
                        if (elemOfTransitionName == "starting") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            if(t == "yes"){
                            	starting = true;
                            }else if(t == "no"){
                            	starting = false;
                            }else{
                            	throw std::runtime_error("Error generating PDA from XML: a state starting can only be yes or no!");
                            }
                        }
                    }

                    if (name.size()){
                    	PDAState state(name, accepting);
                    	if(this->addState(state, starting) == true){
                    		states.push_back(state);
                    		if(starting == true){
                    			this->fStartState = &(this->fStates.back());
                    		}
                    	}
                    }else{
                         throw std::runtime_error("Error generating PDA from XML: Incomplete state definition");
                    }
                }
            }

        }

    }

    for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {  //find  alphabets and blank symbol
        std::string elemName = elem->Value();
        if (elemName == "Transitions") {
            for(TiXmlElement* elemOfDelta = elem->FirstChildElement(); elemOfDelta != NULL; elemOfDelta = elemOfDelta->NextSiblingElement()) {
                std::string elemOfDeltaName = elemOfDelta->Value();
                if (elemOfDeltaName == "transition") {
                    std::string from = "";
                    std::string to = "";
                    char input = '\0';
                    char stackTop = '\0';
                    PDAStackOperation stackOperation;
                    std::vector<char> stackPush;
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
                        if (elemOfTransitionName == "input") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            if(t.size() != 1){
                            	throw std::runtime_error("Error generating PDA from XML: input must be only one character by transition");
                            }

                            input = t.at(0);
                        }
                        if (elemOfTransitionName == "stacktop") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            if(t.size() != 1){
								throw std::runtime_error("Error generating PDA from XML: input must be only one character by transition");
							}
                            stackTop = t.at(0);
                        }
                        if (elemOfTransitionName == "push") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            for (auto i : t)
                            	stackPush.push_back(i);
                        }
                        if (elemOfTransitionName == "operation") {
                            if (elemOfTransition->FirstChild() == NULL) {
                                continue;
                            }
                            TiXmlNode* e = elemOfTransition->FirstChild();
                            TiXmlText* text = e->ToText();
                            if(text == NULL)
                                continue;
                            std::string t = text->Value();
                            if(t == "PUSH"){
                            	stackOperation = PUSH;
                            }if(t == "POP"){
                            	stackOperation = POP;
                            }if(t == "STAY"){
                            	stackOperation = STAY;
                            }if(t == "EMPTY"){
                            	stackOperation = EMPTY;
                            }else{
                            	throw std::runtime_error("Error generating PDA from XML: operation should in transition should be POP, PUSH, EMPTY or STAY");
                            }
                        }

                    }

                    if (from.size() && to.size() && input != '\0' && stackTop != '\0' && stackOperation){
                    	// Let's find those states
                    	PDAState *fromptr = nullptr;
                    	PDAState *toptr = nullptr;
                    	for(auto it = states.begin();it != states.end();it++){
                    		if(it->getName() == from){
                    			fromptr = &(*it);
                    		}
                    		if(it->getName() == to){
								toptr = &(*it);
							}
                    	}

                    	if(fromptr == nullptr or toptr == nullptr){
                    		throw std::runtime_error("Error generating PDA from XML: State in transition doesn't exists");
                    	}
                    	PDATransition transition(fromptr, toptr, input, stackTop, stackOperation, stackPush);
                        this->addTransition(transition);
                    }else{
                         throw std::runtime_error("Error generating PDA from XML: Incomplete transition");
                    }
                }
            }

        }

    }
}

bool PDA::addState(const PDAState& state, const bool& isStarting){
	if(std::find(this->fStates.begin(), this->fStates.end(), state) == this->fStates.end()){
		// State is not yet in fStates list

		// now let's check if the state is legal
		for(auto statesIt = this->fStates.begin();statesIt != this->fStates.end();statesIt++){
			if(statesIt->getName() == state.getName()){
				throw std::runtime_error("There is already a state with this name in the PDA");
				return false;
			}
		}

		// check wheter we need to do some extra work for a start state
		if(isStarting == true){
			// this is a start state
			if(this->fStartState == nullptr){
				// OK, there is no startstate present so set one
				this->fStates.push_back(state);
				this->fStartState = &(this->fStates.back());
				return true;
			}else{
				throw std::runtime_error("There is already a start state");
				return false;
			}
		}else{
			this->fStates.push_back(state);
			return true;
		}
	}else{
		throw std::runtime_error("State is already in PDA");
		return false;
	}
}

bool PDA::addState(const PDAState& state){
	return this->addState(state, false);
}

bool PDA::addTransition(PDATransition transition){
	// now let's check if the transition is legal
	if(std::find(this->fAlphabet.begin(), this->fAlphabet.end(), transition.fInputSymbol) == this->fAlphabet.end()){
		if(transition.fInputSymbol != 0 or transition.fInputSymbol != 5){
			throw std::runtime_error("The transition symbol isn't in the alphabet");
			return false;
		}
	}

	for(auto pushStackIt = transition.fPushStack.begin();pushStackIt != transition.fPushStack.end();pushStackIt++){
		if(std::find(this->fStackAlphabet.begin(), this->fStackAlphabet.end(), *pushStackIt) == this->fStackAlphabet.end()){
			if(*pushStackIt != 0){
				throw std::runtime_error("The transition symbol from the push on stack vector isn't in the alphabet");
				return false;
			}
		}
	}

	if(std::find(this->fStackAlphabet.begin(), this->fStackAlphabet.end(), transition.fTopStack) == this->fStackAlphabet.end()){
		if(transition.fTopStack != 9){
			throw std::runtime_error("The top of the stack symbol isn't in the stack alphabet");
			return false;
		}
	}

	// now lets redirect the pointers to the states in the PDA
	PDAState* newFrom = nullptr;
	PDAState* newTo = nullptr;
	for(auto statesIt = this->fStates.begin();statesIt != this->fStates.end();statesIt++){
		if(transition.fFrom->getName() == statesIt->getName()){
			newFrom = &(*statesIt);
		}
		if(transition.fTo->getName() == statesIt->getName()){
			newTo = &(*statesIt);
		}
	}

	if(newFrom == nullptr){
		throw std::runtime_error("The state from where this transition is coming doesn't exists");
		return false;
	}

	if(newTo == nullptr){
		throw std::runtime_error("The state to where this transition is going doesn't exists");
		return false;
	}

	transition.fFrom = newFrom;
	transition.fTo = newTo;

	if(std::find(this->fTransitions.begin(), this->fTransitions.end(), transition) == this->fTransitions.end()){
		// Transition is not yet in fTransitions list
		this->fTransitions.push_back(transition);
		return true;
	}else{
		throw std::runtime_error("Transition is already in PDA");
		return false;
	}
}

std::vector<PDATransition> PDA::getTransitions(std::string input, char stackTopSymbol, PDAState* from){
	std::vector<PDATransition> selectedTransitions;


	for(auto transitionIt = fTransitions.begin(); transitionIt != fTransitions.end();transitionIt++){
		if(input.size() != 0){
			// only if there are characters, take the first character
			if(transitionIt->fInputSymbol == input.at(0)){
				// This transition corresponds to the symbol from input we're reading now
				if(transitionIt->fFrom == from){
					// This transition has the same from as given in the parameter list
					if(transitionIt->fTopStack == stackTopSymbol){
						// Top of the PDA stack is equal to what the top of the stack should be in this transition
						selectedTransitions.push_back(*transitionIt);
					}
				}
			}
		}

		// Now add the epsilon transitions
		if(transitionIt->fInputSymbol == 0){
			if(transitionIt->fTopStack == stackTopSymbol and transitionIt->fFrom == from){
				selectedTransitions.push_back(*transitionIt);
			}
		}
	}

	return selectedTransitions;
}

bool PDA::process(std::string input){
	if(this->fStartState == nullptr){
		throw std::runtime_error("Please define a  start state before processing a string");
		return false;
	}
	std::queue<PDAID> ids;

	// the first thing we do is adding all the ID's we get with the first input symbol from the start state
	std::vector<PDATransition> selectedTransitions = this->getTransitions(input, 9, this->fStartState);
	std::cout << "Selected Transitions size " << selectedTransitions.size() << std::endl;
	for(auto transitionsIt = selectedTransitions.begin();transitionsIt != selectedTransitions.end();transitionsIt++){
		// add the initial ID's
		std::string newInput = input;
		if(transitionsIt->fInputSymbol != 0){
			// If the input symbol is not zero we need to remove a character from the input string
			newInput.erase(0, 1);
		}

		std::stack<char> tempStack = this->fStack;
		transitionsIt->stackOperation(tempStack);

		PDAID newID(newInput, transitionsIt->fTo, tempStack);

		if(newID.isAccepted(this->fPDAtype) == true){
			std::cout << "Final in first stage with " << newID << std::endl;
			// YES, we're final!
			return true;
		}

		ids.push(newID);
	}

	selectedTransitions.clear();

	if(selectedTransitions.size() == 0){
		// There are no transitions found starting from the start state but maybe the start state is already accepting
		if(input.size() == 0){
			if(this->fPDAtype == STACK){
				// No states to go to anymore so it's final!
				return true;
			}else{
				// No input so this one is also accepting if:
				if(this->fStartState->isFinal()){
					std::cout << "Final due to a empty string and no transitions from the start state" <<std::endl;
					// Start state is final
					return true;
				}
			}
		}
	}

	std::cout << "ids size " << ids.size() <<std::endl;

	// That's done now start using these id's to find a succesfull path
	while(ids.size() > 0){
		// Now let's check if we are already dead, final or something else
		if(ids.front().isAccepted(this->fPDAtype) == true){
			// YES, we're final!
			return true;
		}


		// get the transitions corresponding with the character and the top of the stack
		char stackTopSymbol = 9;
		if(ids.front().fStack.size() != 0){
			stackTopSymbol = ids.front().fStack.top();
		}

		selectedTransitions = this->getTransitions(ids.front().fInput, stackTopSymbol, ids.front().fState);
		std::cout << selectedTransitions.size() <<  "Transitions found for " << ids.front() <<std::endl;

		if(selectedTransitions.size() == 0){
			// there are no transition possible anymore so remove this ID
			std::cout << "Remove ID:" << ids.front() << std::endl;
			ids.pop();
		}else{
			// there are multiple transitions possible so also multiple ID's
			// Get the essential data from the current ID
			std::stack<char> tempStack(ids.front().fStack);

			std::cout << "Change ID:" << ids.front() << std::endl;

			// Now we're gonna add new ID's for each transition
			for(auto transitionsIt = selectedTransitions.begin();transitionsIt != selectedTransitions.end();transitionsIt++){
				std::string newInput = ids.front().fInput;
				if(transitionsIt->fInputSymbol != 0){
					// If the input symbol is not zero we need to remove a character from the input string
					newInput.erase(0, 1);
				}
				std::stack<char> newStack(tempStack);
				transitionsIt->stackOperation(newStack);

				PDAID newID(newInput, transitionsIt->fTo, newStack);

				// check whether we are final or death
				if(newID.isAccepted(this->fPDAtype) == true){
					// YES, we're final!
					return true;
				}

				// We're not death of final yet so add the ID
				ids.push(newID);
				std::cout << "   for: " << newID << std::endl;
			}

			// Remove the current ID
			ids.pop();
		}

		selectedTransitions.clear();
	}

	// No path ended in a final state or empty stack
	return false;
}

std::ostream& operator<<(std::ostream& out, PDA pda){
	out << "PDA" << std::endl << "---" <<std::endl;

	out << "Alphabet:" <<std::endl;
	for(auto it = pda.fAlphabet.begin();it != pda.fAlphabet.end();it++){
		std::cout << *it <<std::endl;
	}

	out << "Stack Alphabet:" <<std::endl;
	for(auto it = pda.fStackAlphabet.begin();it != pda.fStackAlphabet.end();it++){
		std::cout << *it <<std::endl;
	}

	out << "States:" <<std::endl;
	for(auto it = pda.fStates.begin();it != pda.fStates.end();it++){
		std::cout << *it <<std::endl;
	}

	if(pda.fStartState != nullptr){
		out << "Start State:" << *(pda.fStartState) << std::endl;
	}

	out << "Transitions:" <<std::endl;
	for(auto it = pda.fTransitions.begin();it != pda.fTransitions.end();it++){
		std::cout << *it <<std::endl;
	}

	return out;
}

PDA::~PDA() {
    // TODO Auto-generated destructor stub
}

