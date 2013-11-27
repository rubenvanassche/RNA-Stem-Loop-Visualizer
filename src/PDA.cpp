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

		in.pop();
	}else if(this->fStackOperation == STAY){
		// do nothing
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
	out << "Transition " << transition.fFrom->getName() << " -> " << transition.fTo->getName() << " (" << transition.fInputSymbol << ")" << std::endl;
	if(transition.fPushStack.size() == 0){
		// nothing should happen
		out << "when top stack is: " << transition.fTopStack << " nothing happens" << std::endl;
	}else{
		if(transition.fPushStack.size() == 1 and transition.fPushStack.at(0) == 0){
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

std::ostream& operator<<(std::ostream& out, PDAID id){
	out << "PDA ID(" << id.fInput << ", " << id.fState->getName() << ", ";
	for(int i = 0; i < id.fStack.size(); i++){
		out << id.fStack.top();
		id.fStack.pop();
	}
	out << ")";
	return out;
}


PDA::PDA(const std::set<char>& alphabetPDA, const std::set<char>& alphabetStack, const PDAFinal& PDAending){
	this->fAlphabet = alphabetPDA;
	this->fStackAlphabet = alphabetStack;
	this->fPDAtype = PDAending;
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
		if(transition.fInputSymbol != 0){
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
		if(transition.fTopStack != 0){
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
				if(transitionIt->fTopStack == stackTopSymbol and transitionIt->fFrom == from){
					// Top of the PDA stack is equal to what the top of the stack should be in this transition
					// This transition has the same from as given in the parameter list
					selectedTransitions.push_back(*transitionIt);
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

std::pair<bool, bool> PDA::isFinalDead(const std::string& input, PDAState* to,const std::stack<char> stack){
	bool isDead = false;
	bool isFinal = false;

	if(this->fPDAtype == STATE){
		// PDA is state ending
		if(input.size() == 0){
			if(to->isFinal()){
				isFinal = true;
			}else{
				isDead = true;
			}
		}
	}else{
		// PDA is stack ending
		if(stack.size() == 0){
			isFinal = true;
		}
	}

	std::pair<bool, bool> result(isDead, isFinal);
	return result;
}

bool PDA::process(std::string input){
	if(this->fStartState == nullptr){
		throw std::runtime_error("Please define a  start state before processing a string");
		return false;
	}
	std::queue<PDAID> ids;

	// the first thing we do is adding all the ID's we get with the first input symbol from the start state
	std::vector<PDATransition> selectedTransitions = this->getTransitions(input, 0, this->fStartState);
	for(auto transitionsIt = selectedTransitions.begin();transitionsIt != selectedTransitions.end();transitionsIt++){
		// add the initial ID's
		std::string newInput = input;
		if(transitionsIt->fInputSymbol != 0){
			// If the input symbol is not zero we need to remove a character from the input string
			newInput.erase(0, 1);
		}

		std::stack<char> tempStack = this->fStack;
		transitionsIt->stackOperation(tempStack);

		PDAID newId(newInput, transitionsIt->fTo, tempStack);
		ids.push(newId);
	}
	selectedTransitions.clear();

	std::cout << "ids size " << ids.size() <<std::endl;

	// That's done now start using these id's to find a succesfull path
	while(ids.size() > 0){
		// Now let's check if we are already dead, final or something else
		std::pair<bool, bool> finalDeath = this->isFinalDead(ids.front().fInput, ids.front().fState, ids.front().fStack);
		if(finalDeath.second == true){
			// YES, we're final!
			return true;
		}
		if(finalDeath.first == true){
			// No! We're death
			ids.pop();
			continue;
		}

		// get the transitions corresponding with the character and the top of the stack
		char stackTopSymbol = 0;
		if(ids.front().fStack.size() != 0){
			stackTopSymbol = ids.front().fStack.top();
		}

		selectedTransitions = this->getTransitions(ids.front().fInput, stackTopSymbol, ids.front().fState);

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
				std::pair<bool, bool> finalDeath = this->isFinalDead(newInput, transitionsIt->fTo, newStack);

				if(finalDeath.second == true){
					// YES, we're final!
					return true;
				}

				if(finalDeath.first == true){
					// NO, we're death, so actually do not add this ID
				}else{
					// We're not death of final yet so add the ID
					ids.push(newID);
					std::cout << "   for: " << newID << std::endl;
				}
			}

			// Remove the current ID
			ids.pop();
		}

		selectedTransitions.clear();
	}

	// No path ended in a final state or empty stack
	return false;
}

PDA::~PDA() {
    // TODO Auto-generated destructor stub
}

