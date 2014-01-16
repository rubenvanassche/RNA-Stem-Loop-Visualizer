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
#include <list>
#include <set>
#include <utility>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <queue>
#include <fstream>
#include "CFG.h"
#include "TinyXML/tinyxml.h"

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

        /**
		 * @brief Constructor
		 *
		 * @param name The name of the PDAState
		 * @param isFinal Bool which tells if this state is final or not
		 */
		PDAState(std::string name, bool isFinal);

		 /**
		 * @brief Check if a state is final
		 *
		 * @return A bool telling if it's true or not
		 */
		bool isFinal() const;

		 /**
		 * @brief Get the name of the state
		 *
		 * @return A string representing the name
		 */
		std::string getName() const;

		/**
		 * @brief == operator overloading
		 *
		 * @return Bool telling if the two states are equal
		 */
		bool operator==(const PDAState& other);

	    /**
		 * @brief << overloading
		 */
		friend std::ostream& operator<<(std::ostream& out, PDAState state);

        virtual ~PDAState();
private:
        std::string fName;
        bool fFinal;
};

enum PDAStackOperation{
    PUSH,
    POP,
    POPPUSH,
    STAY,
    EMPTY
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
	 * @param stackOperation Should the stack be popped, pushed or stay as it is
	 */
	PDATransition(
		PDAState* from,
		PDAState* to,
		const char& input,
		const char& stackTop,
		const PDAStackOperation& stackOperation
	);

     /**
     * @brief Constructor
     *
     * @param from A pointer to the PDAState where this transition is coming from
     * @param to A pointer to the PDAState where this transition is going to
     * @param input The input symbol for the transition
     * @param stackTop Define what should be on the top of the stack when processing this transition
     * @param stackOperation Should the stack be popped, pushed or stay as it is
     * @param stackPush The character that should be pushed to the stack during the transition
     */
    PDATransition(
        PDAState* from, 
        PDAState* to, 
        const char& input, 
        const char& stackTop, 
        const PDAStackOperation& stackOperation,
        const char& stackPush
    );

     /**
     * @brief Constructor
     *
     * @param from A pointer to the PDAState where this transition is coming from
     * @param to A pointer to the PDAState where this transition is going to
     * @param input The input symbol for the transition
     * @param stackTop Define what should be on the top of the stack when processing this transition
     * @param stackOperation Should the stack be popped, pushed or stay as it is
     * @param stackPush The character vector that should be pushed to the stack during the transition
     */
    PDATransition(
        PDAState* from, 
        PDAState* to, 
        const char& input, 
        const char& stackTop, 
        const PDAStackOperation& stackOperation,
        const std::vector<char>& stackPush
        );

    virtual ~PDATransition();

    /**
	 * @brief == operator overloading
	 *
	 * @return Bool telling if the two transitions are equal
	 */
	bool operator==(const PDATransition& other);

	/**
	 * @brief Change a stack based upon the data in the transition
	 *
	 * @param in A stack with chars representing the stack in the PDA. Be careful! The stack is given by reference
	 */
    void stackOperation(std::stack<char>& in);

    /**
	 * @brief << overloading
	 */
    friend std::ostream& operator<<(std::ostream& out, PDATransition transition);

    /**
	* @brief get the from state of the transition
	*
	* @return PDAState pointer
	*/
    PDAState* getFrom(){ return this->fFrom;};

    /**
	* @brief get the to state of the transition
	*
	* @return PDAState pointer
	*/
	PDAState* getTo(){ return this->fTo;};

	/**
	* @brief get the input symbol of the transition
	*
	* @return char
	*/
	char getInputSymbol(){ return this->fInputSymbol;};

	/**
	* @brief get the symbol on the top of the stack in this transition
	*
	* @return char
	*/
	char getTopStack(){ return this->fTopStack;};

	/**
	* @brief get the stack operation of the transition
	*
	* @return PDAStackOperation
	*/
	PDAStackOperation getStackOperation(){ return this->fStackOperation;};

	/**
	* @brief get the characters which are popped on the stack during a push operation
	*
	* @return vector with chars
	*/
	std::vector<char> getPushStack(){ return this->fPushStack;};

	/**
	* @brief change the from state in the transition
	*
	* @param from PDAState pointer to the state
	*/
	void setFrom(PDAState* from){ this->fFrom = from;};

	/**
	* @brief change the to state in the transition
	*
	* @param from PDAState pointer to the state
	*/
	void setTo(PDAState* to){ this->fTo = to;};
private:
    PDAState *fFrom;
    PDAState *fTo;
    char fInputSymbol;
    char fTopStack;
    std::vector<char> fPushStack;
    PDAStackOperation fStackOperation;
};

// Is the PDA final with empty stack or in final State
enum PDAFinal{
    STACK,
    STATE
};

/**
 * @brief Class representing a PDA Instantenious Description
 */
class PDAID{
public:
    /**
     * @brief Constructor
     *
     * @param input The input string
     * @param currentState The pointer to state where the ID starts
     * @param stack The stack at this moment
     */

	PDAID(const std::string& input, PDAState* currentState, const std::stack<char> stack);

    /**
     * @brief Process the ID according to one transition for one step
     *
     * @param to Pointer to next state
     * @param inputSymbol Character accompanied with this transition
     * @param topStack Character that should be at the top of the stack after the transition
     */
    void step(const std::string& input, PDAState* currentState, const std::stack<char> stack);

    /**
	 * @brief Check if this ID will be accepted by the PDA
	 *
	 * @param pdaType Type of PDA(State or Stack)
	 *
	 * @return bool telling if the ID is accepted
	 */
    bool isAccepted(PDAFinal pdaType);

    /**
	 * @brief << overloading
	 */
    friend std::ostream& operator<<(std::ostream& out, PDAID id);

    /**
     * @brief get the input of the ID
     *
     * @return string
     */
    std::string getInput(){ return this->fInput;};

    /**
	 * @brief get the state of the ID
	 *
	 * @return PDAState pointer
	 */
    PDAState* getState(){ return this->fState;};

    /**
	 * @brief get the stack of the ID
	 *
	 * @return stack with chars
	 */
    std::stack<char> getStack(){ return this->fStack;};

private:
    std::string fInput;
    PDAState* fState;
    std::stack<char> fStack;
};

/**
 * @brief Class representing a PDA
 */
class PDA {
public:
     /**
     * @brief Constructor
     *
     * @param alphabetPDA A set containing characters representing the alphabet of the PDA
     * @param alphabetStack A set containing characters representing the alphabet of the stack
     * @param PDAending The type of PDA(STACK: PDA is final with empty stack, STATE: PDA is final when it reaches an empty state)
     */
    PDA(
        const std::set<char>& alphabetPDA, 
        const std::set<char>& alphabetStack, 
        const PDAFinal& PDAending
        );


    /**
    * @brief Constructor
    *
    * @param cfg A Context Free Grammar to be transformed to a PDA
    */
    PDA(CFG cfg);

    /**
     * @brief Constructor
     *
     * @param fileName A XML file containing info about the PDA
     */
    PDA(const std::string& fileName);

     /**
     * @brief Add a new state to the PDA
     *
     * @param state An PDAState object representing an PDA state
     *
     * @return A bool telling if the state is added or not
     */
    bool addState(const PDAState& state);

     /**
     * @brief Add a new state to the PDA
     *
     * @param state A PDAState object representing an PDA state
     * @param isStarting A bool describing if the state is the start state
     *
     * @return A bool telling if the state is added or not
     */
    bool addState(const PDAState& state, const bool& isStarting);

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

    /**
     * @brief Store an PDA in a dot file
     *
     * @param fileName The file to write to
     *
     * @bool A bool telling if creating and writing the file was succesfull
     */
    bool toDotFile(std::string fileName);

    /**
	 * @brief << overloading
	 */
    friend std::ostream& operator<<(std::ostream& out, PDA pda);

    virtual ~PDA();

private:
    /**
     * @brief Get transitions based upon an input, stackTopsymbol and state
     *
     * @param input Which string should be processed by the state
     * @param stackTopSymbol The symbol on the top of the stack at the moment this function is called
     * @param from A PDAState from where the transitions should start
     *
     * @return A vector<PDATransition> giving you all the transitions starting from the specified state
     */
    std::vector<PDATransition> getTransitions(std::string input, char stackTopSymbol, PDAState* from);


    std::list<PDATransition> fTransitions;
    std::list<PDAState> fStates;

    PDAState* fStartState = nullptr;

    PDAFinal fPDAtype;

    std::set<char> fStackAlphabet;
    std::set<char> fAlphabet;

    std::stack<char> fStack;

    bool fBasedUponCFG = false; // So we do not spend computer time at running in loops
};


#endif /* PDA_H_ */
