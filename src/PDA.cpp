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


PDATransition::PDATransition(PDAState* from, PDAState* to, const char& input, const char& stackTop, const char& stackPush) {
	this->fFrom = from;
	this->fTo = to;
	this->fInputSymbol = input;
	this->fTopStack = stackTop;
	this->fPushStack.push_back(stackPush);
}

PDATransition::PDATransition(PDAState* from, PDAState* to, const char& input, const char& stackTop, const std::vector<char>& stackPush){
	this->fFrom = from;
	this->fTo = to;
	this->fInputSymbol = input;
	this->fTopStack = stackTop;
	this->fPushStack.insert(this->fPushStack.end(), stackPush.begin(), stackPush.end());
}

void PDATransition::stackOperation(std::stack<char>& in){
	if(this->fPushStack.size() == 0){
		// nothing should happen
		return;
	}else{
		if(this->fPushStack.size() == 1 and this->fPushStack.at(0) == 0){
			// Epsilon so pop value from stack
			in.pop();
		}else if(this->fPushStack.size() == 1 and this->fPushStack.at(0) == this->fTopStack){
			// a/a nothing to do the stack stays the same
			return;
		}else{
			// just push everything on the stack
			for(auto pushStackIt = this->fPushStack.begin(); pushStackIt != this->fPushStack.end();pushStackIt++){
				in.push(*pushStackIt);
			}
		}
	}
}

bool PDATransition::operator==(const PDATransition& other){
	if(other.fFrom == this->fFrom and other.fTo == this->fTo and other.fInputSymbol == this->fInputSymbol and other.fPushStack == this->fPushStack and other.fTopStack == this->fTopStack){
		return true;
	}else{
		return false;
	}
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


PDA::PDA(const std::set<char>& alphabetPDA, const std::set<char>& alphabetStack, const char& stackStartSymbol, const PDAFinal& PDAending){
	this->fAlphabet = alphabetPDA;
	this->fStackAlphabet = alphabetStack;
	this->fStackStartSymbol = stackStartSymbol;
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
	if(std::find(this->fTransitions.begin(), this->fTransitions.end(), transition) == this->fTransitions.end()){
		// Transition is not yet in fTransitions list

		// now let's check if the transition is legal
		if(std::find(this->fAlphabet.begin(), this->fAlphabet.end(), transition.fInputSymbol) == this->fAlphabet.end()){
			throw std::runtime_error("The transition symbol isn't in the alphabet");
			return false;
		}

		for(auto pushStackIt = transition.fPushStack.begin();pushStackIt != transition.fPushStack.end();pushStackIt++){
			if(std::find(this->fAlphabet.begin(), this->fAlphabet.end(), *pushStackIt) == this->fAlphabet.end()){
				throw std::runtime_error("The transition symbol from the push on stack vector isn't in the alphabet");
				return false;
			}
		}

		if(std::find(this->fStackAlphabet.begin(), this->fStackAlphabet.end(), transition.fTopStack) == this->fStackAlphabet.end()){
			throw std::runtime_error("The top of the stack symbol isn't in the stack alphabet");
			return false;
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

		this->fTransitions.push_back(transition);
		return true;
	}else{
		throw std::runtime_error("Transition is already in PDA");
		return false;
	}
}

std::vector<PDATransition> PDA::getTransitions(char symbol, char stackTopSymbol, PDAState* from){
	std::vector<PDATransition> selectedTransitions;

	for(auto transitionIt = fTransitions.begin(); transitionIt != fTransitions.end();transitionIt++){
		if(transitionIt->fInputSymbol == symbol){
			// This transition corresponds to the symbol from input we're reading now
			if(transitionIt->fTopStack == stackTopSymbol){
				// Top of the PDA stack is equal to what the top of the stack should be in this transition
				if(transitionIt->fFrom == from){
					// This transition has the same from as given in the parameter list
					selectedTransitions.push_back(*transitionIt);
				}
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
		if(stack.size() == 0){
			isDead = true;
		}
	}else{
		// PDA is stack ending
		if(stack.size() == 0){
			isFinal = true;
		}
		if(input.size() == 0){
			isDead = true;
		}
	}

	std::pair<bool, bool> result(isDead, isFinal);
	return result;
}

bool PDA::process(std::string input){
	std::list<PDAID> ids;

	input.erase(0);
	std::vector<PDATransition> selectedTransitions = this->getTransitions(input[0], this->fStack.top(), this->fStartState);
	for(auto transitionsIt = selectedTransitions.begin();transitionsIt != selectedTransitions.end();transitionsIt++){
		// add the initial ID's
		std::stack<char> tempStack = this->fStack;
		transitionsIt->stackOperation(tempStack);
		PDAID newId(input, transitionsIt->fTo, tempStack);
		ids.push_back(newId);
	}

	selectedTransitions.clear();

	std::list<PDAID>::iterator idsIt = ids.begin();
	while(ids.size() != 0){
		// get the transitions corresponding with the character and the top of the stack
		char symbol = idsIt->fInput.at(0);
		char stackTopSymbol = idsIt->fStack.top();
		selectedTransitions = this->getTransitions(symbol, stackTopSymbol, idsIt->fState);

		if(selectedTransitions.size() == 0){
			// there are no transition possible anymore so remove this ID
			ids.erase(idsIt);
		}else if(selectedTransitions.size() == 1){
			// change the current ID to a new state with the one transition
			std::string newInput = idsIt->fInput.erase(0);
			std::stack<char> newStack = idsIt->fStack;
			PDAState* newTo = selectedTransitions.at(0).fTo;

			// change the stack
			selectedTransitions.at(0).stackOperation(newStack);

			// check whether we are final or death
			std::pair<bool, bool> finalDeath = this->isFinalDead(newInput, newTo, newStack);

			if(finalDeath.second == true){
				// YES, we're final!
				return true;
			}

			if(finalDeath.first == true){
				// NO, we're death!
				ids.erase(idsIt);
			}else{
				// We're not death of final yet so change the ID
				idsIt->step(newInput, newTo, newStack);
			}
		}else{
			// there are multiple transitions possible so also multiple ID's
			// Get the essential data from the current ID
			const std::string newInput = idsIt->fInput.erase(0);
			const std::stack<char> tempStack = idsIt->fStack;

			// Remove the current ID
			ids.erase(idsIt);

			// Now we're gonna add new ID's for each transition
			for(auto transitionsIt = selectedTransitions.begin();transitionsIt != selectedTransitions.end();transitionsIt++){
				// change the stack
				std::stack<char> newStack = tempStack;
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
					ids.push_back(newID);
				}
			}
		}
		selectedTransitions.clear();
		idsIt++;
	}

	return false;
}

PDA::~PDA() {
    // TODO Auto-generated destructor stub
}

