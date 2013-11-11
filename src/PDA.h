/*
 * PDA.h
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
 * By: Ruben Van Assche
 */

#ifndef PDA_H_
#define PDA_H_

#include <stack>
#include <string>
#include <vector>

/**
 * @brief Class representing a state from a PDA
 */

class PDAState {
public:
	 /**
	 * @brief Constructor
	 *
	 * @param name The name of the PDAState
	 */
	PDAState(std::string name);

	virtual ~PDAState();
private:
	std::string name;
};

/**
 * @brief Class representing a transition from a PDA
 */

class PDATransition {
public:
	 /**
	 * @brief Constructor
	 *
	 * @param from A pointer to the PDAState where this transition is coming from
	 * @param to A pointer to the PDAState where this transition is going to
	 * @param input The input symbol for the transition
	 * @param stackTop Define what should be on the top of the stack when processing this transition
	 * @param stackPush The character that should be pushed to the stack during the transition
	 */
	PDATransition(PDAState* from, PDAState* to, char input, char stackTop, char stackPush);

	 /**
	 * @brief Constructor
	 *
	 * @param from A pointer to the PDAState where this transition is coming from
	 * @param to A pointer to the PDAState where this transition is going to
	 * @param input The input symbol for the transition
	 * @param stackTop Define what should be on the top of the stack when processing this transition
	 * @param stackPush The character vector that should be pushed to the stack during the transition
	 */
	PDATransition(PDAState* from, PDAState* to, char input, char stackTop, std::vector<char> stackPush);

	virtual ~PDATransition();
private:
	PDAState *from = nullptr;
	PDAState *to = nullptr;
	char inputSymbol;
	char topStack;
	std::vector<char> pushStack;
};

// Is the PDA final with empry stack or in final State
enum PDAFinal{
    STACK,
    STATE
};

/**
 * @brief Class representing a PDA
 */

class PDA {
public:
	 /**
	 * @brief Constructor
	 *
	 * @param alphabetPDA A vector containing characters representing the alphabet of the PDA
	 * @param alphabetStack A vector containing characters representing the alphabet of the stack
	 * @param stackStartSymbol The start symbol for the stack
	 * @param PDAending The type of PDA(STACK: PDA is final with empty stack, STATE: PDA is final when it reaches an empty state)
	 */
	PDA(std::vector<char> alphabetPDA, std::vector<char> alphabetStack, char stackStartSymbol, PDAFinal PDAending);

	 /**
	 * @brief Add a new state to the PDA
	 *
	 * @param state An PDAState object representing an PDA state
	 *
	 * @return A bool telling if the state is added or not
	 */
	bool addState(PDAState state);
	 /**
	 * @brief Add a new state to the PDA
	 *
	 * @param state An PDAState object representing an PDA state
	 * @param isEnding A bool describing if the state is a final state
	 *
	 * @return A bool telling if the state is added or not
	 */
	bool addState(PDAState state, bool isFinal);
	 /**
	 * @brief Add a new state to the PDA
	 *
	 * @param state A PDAState object representing an PDA state
	 * @param isEnding A bool describing if the state is a final state
	 * @param isStarting A bool describing if the state is the start state
	 *
	 * @return A bool telling if the state is added or not
	 */
	bool addState(PDAState state, bool isFinal, bool isStarting);

	 /**
	 * @brief Add a new transition to the PDA
	 *
	 * @param transition A PDATransition object representing an PDA transition
	 *
	 * @return A bool telling if the transition is added or not
	 */
	bool addTransition(PDATransition transition);

	/**
	 * @brief Process an input string through the PDA
	 *
	 * @param input The string to be processed by the PDA
	 *
	 * @return A bool telling if the PDA ended in a final state or empty stack
	 */
	bool process(std::string input);

	virtual ~PDA();
private:
	std::vector<PDATransition> transitions;
	std::vector<PDAState> states;
	std::vector<PDAState*> finalStates;

	PDAState* startState = nullptr;

	char startSymbol;

	PDAFinal PDAtype;

	std::vector<char> stackAlphabet;
	std::vector<char> Alphabet;

	std::stack<char> stack;

};

#endif /* PDA_H_ */
