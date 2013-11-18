/*
 * Turing.h
 *
 * Copyright (C) 2013 Jakob Struye
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

#ifndef TURING_H
#define TURING_H

#include <set>
#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <utility>
#include <deque>
#include <queue>
#include <tuple>

enum Direction {L, R};


/**
 * @brief Class representing a state of a Turing Machine
 */
class TuringState {
public:
     /**
     * @brief Constructor
     *
     * @param name The name of the TuringState
     */
    TuringState(const std::string& name);

    /**
     * @brief Checks if name of state is given name
     *
     * @param name Name to check
     *
     * @return True if name is given name
     */

    bool isCalled(const std::string& name) const;


    /**
     * @brief overload for boolean < (set support)
     */

    bool operator<(const TuringState&) const;

    /**
     * @brief overload for boolean > (set support)
     */

    bool operator>(const TuringState&) const;

    /**
     * @brief output overload
     */

    friend std::ostream& operator<<(std::ostream& output, const TuringState& TS);

    virtual ~TuringState();

private:

    std::string fName;
};



/**
 * @brief Class representing a transition of a Turing Machine
 */
class TuringTransition {
public:
     /**
     * @brief Constructor
     *
     * @param from A pointer to the TuringState where this transition is coming from
     * @param to A pointer to the TuringState where this transition is going to
     * @param read The read symbol on the tape
     * @param write The symbol to be written to the tape
     * @param dir Direction in which to move head
     */
    TuringTransition(
        const TuringState* from,
        const TuringState* to,
        const char& read,
        const char& write,
        Direction dir
    );

    /**
     * @brief Checks if transition is for given state and tape symbol
     *
     * @param state State from which transition should start
     * @param symbol Symbol that should be under head on tape
     *
     * @return True if matching transition
     */

    bool match(const TuringState* state, char symbol);

    /**
     * @brief Gets next state, symbol to write and direction
     *
     * @return Tuple of next state, symbol to write and direction to move in
     */

    std::tuple<const TuringState*, char, Direction> getTransition() const;

    /**
     * @brief overload for boolean <, needed to insert TuringTransitions into a set
     */

    bool operator<(const TuringTransition&) const;

    /**
     * @brief all of the given arguments are the transition's
     *
     * @param from From pointer to check
     * @param to To pointer to check
     * @param read Read symbol to check
     * @param write Write symbol to check
     * @param dir Direction to check
     *
     *
     * @return True if the same
     */

    bool isThisTransition(const TuringState* from,
            const TuringState* to,
            const char& read,
            const char& write,
            Direction dir) const;


    virtual ~TuringTransition();

private:
    const TuringState *fFrom = nullptr;
    const TuringState *fTo = nullptr;
    char fRead;
    char fWrite;
    Direction fDirection;
};

/**
 * @brief Class representing the tape for a Turing Machine
 */

class Tape {
public:

    /**
     * @brief Constructor
     *
     * @param input String to write to tape
     * @param blank Blank symbol
     */

    Tape(std::string& input, char blank);

    /*
     * @brief fetches the symbol at a given position
     *
     * @return the symbol
     */
    char getSymbolAtHead() const;

    /**
     * @brief Replaces symbol at given position by given symbol
     *
     * @param symbol The symbol to be written to tape
     */
    void replaceSymbolAtHead(char symbol);

    /**
     * @brief Move head one stop
     *
     * @param dir Left or right
     */

    void moveHead(Direction dir);

private:
    std::deque<char> fTape;
    char fBlank;
    int fHead;

};

/**
 * @brief Class representing Turing Machine Instantaneous Description
 */

class TMID {
public:

    /**
     * @brief Constructor
     *
     * @param input The input string
     * @param state The start state
     * @param blank The blank symbol for the tape
     */

    TMID(std::string& input, const TuringState* startState, char blank);

    /**
     * @brief Get the symbol at the current head position and the current state of the ID
     *
     * @return Pair of the symbol and the state
     */
    std::pair<const TuringState*, char> getStateAndSymbol() const;

    /**
     * @brief Process the ID according to one transition for one step
     *
     * @param to Pointer to next state
     * @param write Symbol to be written to tape
     * @param dir Direction to move head in
     */
    void step(const TuringState* to, char write, Direction dir);

private:
    Tape fTape;
    const TuringState* fState;

};

/**
 * @brief Class representing a Turing Machine
 */

class TuringMachine {
public:
     /**
     * @brief Constructor
     *
     * @param alphabetTuring A set containing characters representing the alphabet of the Turing Machine
     * @param alphabetTape A set containing characters representing the alphabet of the tape
     * @param tapeBlank The blank symbol for the stack
     */
    TuringMachine(
        const std::set<char>& alphabetTuring,
        const std::set<char>& alphabetTape,
        const char& tapeBlank
        );


     /**
     * @brief Add a new state to the Turing Machine
     *
     * @param state The name of the state to be added
     * @param isEnding A bool describing if the state is a final state
     * @param isStarting A bool describing if the state is the start state
     *
     * @return A bool telling if the state is added or not
     */
    bool addState(
            const std::string& state,
            const bool isStarting,
            const bool isFinal
            );

     /**
     * @brief Add a new transition to the Turing Machine
     *
     * @param from The name of the state where the transition starts
     * @param to The name of the state where the transition leads to
     * @param read The symbol read from the tape
     * @param write The symbol to write to the tape
     * @param dir The direction to move tape head in
     *
     * @return A bool telling if the transition is added or not
     */
    bool addTransition(const std::string& from,
            const std::string& to,
            const char& read,
            const char& write,
            Direction dir);



    /**
     * @brief Process an input string through the Turing Machine
     *
     * @param input The string to be processed by the Turing Machine
     *
     * @return A bool telling if the Turing Machine accepted
     */
    bool process(std::string& input);

    virtual ~TuringMachine();

private:
    const TuringState* getStatePtr(std::string& name) const;


    std::set<TuringState*> fStates;
    std::set<char> fAlphabet;
    std::set<char> fTapeAlphabet;
    std::set<TuringTransition> fTransitions;
    const TuringState* fStartState = nullptr;
    char fBlank;
    std::set<const TuringState*> fAccepting;
};
#endif /*TURING_H*/
