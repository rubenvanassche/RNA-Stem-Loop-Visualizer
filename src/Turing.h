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
#include <algorithm>
#include <iostream>
#include <utility>
#include <deque>
#include <queue>
#include <tuple>
#include <memory>
#include "TinyXML/tinyxml.h"

enum Direction {L, R};
class TuringState;
typedef std::shared_ptr<const TuringState> StatePtr;


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
    * @brief Constructor
    *
    * @param name The name of the TuringState
    * @param storage The storage in the TuringState
    */
   TuringState(const std::string& name, const std::vector<char>& storage);

    /**
     * @brief Checks if name of state is given name
     *
     * @param name Name to check
     *
     * @return True if name is given name
     */

    bool isCalled(const std::string& name) const;

    /**
     * @brief Checks if the state has given storage
     *
     * @param storage The storage to check for
     *
     * @return True if storage is given storage
     */

    bool hasThisStorage(const std::vector<char> storage) const;


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
    std::vector<char> fStorage;
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
     * @param read The read symbol(s) on the tape
     * @param write The symbol(s) to be written to the tape
     * @param dir Direction in which to move head
     */
    TuringTransition(
        StatePtr from,
        StatePtr to,
        const std::vector<char>& read,
        const std::vector<char>& write,
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

    bool match(StatePtr state, std::vector<char> symbol);

    /**
     * @brief Gets next state, symbol to write and direction
     *
     * @return Tuple of next state, symbol to write and direction to move in
     */

    std::tuple<StatePtr, std::vector<char>, Direction> getTransition() const;

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

    bool isThisTransition(StatePtr from,
            StatePtr to,
            const std::vector<char>& read,
            const std::vector<char>& write,
            Direction dir) const;

    /**
     * @brief output overload
     */

    friend std::ostream& operator<<(std::ostream& output, const TuringTransition& TT);


    virtual ~TuringTransition();

private:
    StatePtr fFrom = nullptr;
    StatePtr fTo = nullptr;
    std::vector<char> fRead;
    std::vector<char> fWrite;
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
     * @param trackCount number of tracks on the tape
     */

    Tape(std::string& input, char blank, int trackCount);

    /*
     * @brief fetches the symbol at a given position
     *
     * @return the symbol
     */
    std::vector<char> getSymbolAtHead() const;

    /**
     * @brief Replaces symbol at given position by given symbol
     *
     * @param symbol The symbol to be written to tape
     */
    void replaceSymbolAtHead(std::vector<char> symbol);

    /**
     * @brief Move head one stop
     *
     * @param dir Left or right
     */

    void moveHead(Direction dir);

    /**
     * @brief output overload
     */

    friend std::ostream& operator<<(std::ostream& output, const Tape& T);

private:
    std::deque<std::vector<char>> fTape;
    char fBlank;
    int fHead;
    int fTrackCount;

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
     * @param trackCount Number of tracks on the tape
     */

    TMID(std::string& input, StatePtr startState, char blank, int trackCount);

    /**
     * @brief Get the symbol at the current head position and the current state of the ID
     *
     * @return Pair of the symbol and the state
     */
    std::pair<StatePtr, std::vector<char>> getStateAndSymbol() const;

    /**
     * @brief Process the ID according to one transition for one step
     *
     * @param to Pointer to next state
     * @param write Symbol to be written to tape
     * @param dir Direction to move head in
     */
    void step(StatePtr to, std::vector<char> write, Direction dir);

    /**
     * @brief output overload
     */

    friend std::ostream& operator<<(std::ostream& output, const TMID& ID);

private:
    Tape fTape;
    StatePtr fState;
    int fTrackCount;

};

/**
 * @brief Class representing a Turing Machine
 */

class TuringMachine {
public:

    /**
     * @brief Constructor
     *
     */

    TuringMachine();
     /**
     * @brief Constructor
     *
     * @param alphabetTuring A set containing characters representing the alphabet of the Turing Machine
     * @param alphabetTape A set containing characters representing the alphabet of the tape
     * @param tapeBlank The blank symbol for the stack
     */
    TuringMachine(
        const std::set<char>& alphabetTuring,
        const std::set<std::vector<char>>& alphabetTape,
        char tapeBlank
        );

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
       char tapeBlank
       );

    /**
    * @brief Constructor. Note that this constructor supposes the given states (and start/accepting) and transitions are valid; no further checks are made!
    *
    * @param alphabetTuring A set containing characters representing the alphabet of the Turing Machine
    * @param alphabetTape A set containing characters representing the alphabet of the tape
    * @param tapeBlank The blank symbol for the stack
    * @param states The set of states
    * @param transitions The set of transitions
    * @param startState Pointer to start state
    * @param acceptingStates The set of accepting states
    *
    */
  /* TuringMachine(
       const std::set<char>& alphabetTuring,
       const std::set<std::vector<char>>& alphabetTape,
       const char& tapeBlank,
       const std::set<StatePtr>& states,
       const std::set<TuringTransition>& transitions,
       StatePtr startState,
       const std::set<StatePtr>& acceptingStates
       );*/


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
            const bool isFinal,
            const std::vector<char>& storage = std::vector<char> ()
            );

     /**
     * @brief Add a new transition to the Turing Machine
     *
     * @param from The name of the state where the transition starts
     * @param to The name of the state where the transition leads to
     * @param read The symbol read from the tape
     * @param write The symbol to write to the tape
     * @param dir The direction to move tape head in
     * @param fromStorage Storage for the start state
     * @param toStorage Storage for the start state
     *
     * @return A bool telling if the transition is added or not
     */
    bool addTransition(const std::string& from,
            const std::string& to,
            const char& read,
            const char& write,
            Direction dir,
            const std::vector<char>& fromStorage = std::vector<char> (),
            const std::vector<char>& toStorage = std::vector<char> ());

    /**
    * @brief Add a new transition to the Turing Machine
    *
    * @param from The name of the state where the transition starts
    * @param to The name of the state where the transition leads to
    * @param read The vector of symbols read from the tape
    * @param write The vector of symbol to write to the tape
    * @param dir The direction to move tape head in
    * @param fromStorage Storage for the start state
    * @param toStorage Storage for the start state
    *
    * @return A bool telling if the transition is added or not
    */
   bool addTransition(const std::string& from,
           const std::string& to,
           const std::vector<char>& read,
           const std::vector<char>& write,
           Direction dir,
           const std::vector<char>& fromStorage = std::vector<char> (),
           const std::vector<char>& toStorage = std::vector<char> ());

   /**
    * @brief Adds start state pointer after adding the states. Useful for XML with state storage
    */
   bool addStartState(std::string name, std::vector<char> storage = std::vector<char> ());

   /**
    * @brief Adds an accepting state after adding the states. Useful for XML with state storage
    */
   bool addAcceptingState(std::string name, std::vector<char> storage = std::vector<char> ());


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
    StatePtr getStatePtr(std::string& name) const;


    std::vector<StatePtr> fStates;
    std::set<char> fAlphabet;
    std::set<std::vector<char>> fTapeAlphabet;
    std::vector<TuringTransition> fTransitions;
    StatePtr fStartState = nullptr;
    char fBlank;
    std::set<StatePtr> fAccepting;
    int fStateStorageSize = -1;
    int fTrackCount = -1;
};


/**
 * @brief Generates a Turing Machine from an XML file
 *
 * @param fileName Name of the XML file
 *
 * @return TM The generated Turing Machine
 */
TuringMachine generateTM(std::string fileName);

#endif /*TURING_H*/
