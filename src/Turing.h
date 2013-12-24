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
 * Last modified: 28 November 2013
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

/**
 * Please note that the TuringState, TuringTransition, TMID and Tape classes were designed specifically for use with the TuringMachine class.
 * Correct behaviour with invalid arguments is not guaranteed, though the TuringMachine class is guaranteed not to provide invalid arguments to the aforementioned classes.
 *
 * The user is only supposed to use generateTM and TuringMachine::process. This is currently not enforced, but calling other functions may cause bad stuff.
 * Both functions can (only) throw std::runtime_error. The user is responsible for catching them and printing the .what() if desired
 */

enum Direction {L, R, U}; //left, right, or undefined
class TuringState;
class TuringMachine;
typedef std::shared_ptr<const TuringState> StatePtr;
typedef std::shared_ptr<TuringMachine> TuringPtr;

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

    bool hasThisStorage(const std::vector<char>& storage) const;

    /**
     * @brief Output overload
     */

    friend std::ostream& operator<<(std::ostream& output, const TuringState& TS);

    /**
     * @brief Destructor
     */

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
     * @param read The read symbol(s) on the tape (in a vector)
     * @param write The symbol(s) to be written to the tape (in a vector)
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
     * @param symbols Symbol(s) that should be under head on tape
     *
     * @return True if matching transition
     */

    bool match(StatePtr state, const std::vector<char>& symbols) const;

    /**
     * @brief Gets next state, symbol(s) to write and direction
     *
     * @return Tuple of next state, symbol(s) to write and direction to move in
     */

    std::tuple<StatePtr, std::vector<char>, Direction> getTransition() const;

    /**
     * @brief Checks if all of the given arguments are also the transition's
     *
     * @param from From pointer to check
     * @param to To pointer to check
     * @param read Read symbol(s) to check
     * @param write Write symbol(s) to check
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

    /**
     * @Brief Destructor
     */

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

    Tape(const std::string& input, char blank, int trackCount);

    /*
     * @brief fetches the symbol(s) at a given position
     *
     * @return the symbol(s)
     */
    std::vector<char> getSymbolsAtHead() const;

    /**
     * @brief Replaces symbol(s) at given position by given symbol(s)
     *
     * @param symbol The symbol(s) to be written to tape
     */
    void replaceSymbolsAtHead(const std::vector<char>& symbols);

    /**
     * @brief Move head one spot
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
    int fHead;     //Position of head, 0 being the cell where the first character of the input was written to
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

    TMID(const std::string& input, StatePtr startState, char blank, int trackCount);

    /**
     * @brief Gets the symbol at the current head position and the current state of the ID
     *
     * @return Pair of the symbol and the state
     */
    std::pair<StatePtr, std::vector<char>> getStateAndSymbols() const;

    /**
     * @brief Processes the ID according to one transition for one step
     *
     * @param to Pointer to next state
     * @param write Symbol to be written to tape
     * @param dir Direction to move head in
     */
    void step(StatePtr to, const std::vector<char>& write, Direction dir);

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
     * @brief Constructor (will construct TM with empty alphabets, empty state, final state and transition sets and no blank symbol or start state)
     */

    TuringMachine();
     /**
     * @brief Constructor
     *
     * @param alphabetTuring A set containing characters representing the alphabet of the Turing Machine
     * @param alphabetTape A set containing characters representing the alphabet of the tape
     * @param tapeBlank The blank symbol for the tape
     */
    TuringMachine(
        const std::set<char>& alphabetTuring,
        const std::set<char>& alphabetTape,
        char tapeBlank
        );
     /**
     * @brief Adds a new state to the Turing Machine
     *
     * @param state The name of the state to be added
     * @param isStarting A bool indicating whether the state is the start state
     * @param isEnding A bool indicating whether the state is a final state
     *
     * @return True if state was added
     *
     * Note that start and final states may also be set to true afterwards.
     * Comes in handy when adding states through loops (when adding states with different storages!)
     */
    bool addState(
            const std::string& state,
            bool isStarting = 0,
            bool isFinal = 0,
            const std::vector<char>& storage = std::vector<char> ()
            );
     /**
     * @brief Add a new transition to the Turing Machine (single track only)
     *
     * @param from The name of the state where the transition starts
     * @param to The name of the state where the transition leads to
     * @param read The symbol read from the tape
     * @param write The symbol to write to the tape
     * @param dir The direction to move tape head in
     * @param fromStorage Storage for the start state
     * @param toStorage Storage for the start state
     *
     * @return True if transition was added
     */
    bool addTransition(const std::string& from,
            const std::string& to,
            char read,
            char write,
            Direction dir,
            const std::vector<char>& fromStorage = std::vector<char> (),
            const std::vector<char>& toStorage = std::vector<char> ());

    /**
    * @brief Add a new transition to the Turing Machine (multitrack supported)
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
    * @brief Indicates start state pointer after adding the states (so the state has to be in the TM first!). Useful for XML with state storage
    *
    * @param name Name of the start state
    * @param storage Storage of the start state
    *
    * @return True if start state pointer was added
    */
   bool indicateStartState(const std::string& name, const std::vector<char>& storage = std::vector<char> ());

   /**
    * @brief Indicates an accepting state after adding the states (so the state has to be in the TM first!). Useful for XML with state storage
    *
    * @param name Name of the accepting state
    * @param storage Storage of the accepting state
    *
    * @return True if start state pointer was added
    */
   bool indicateAcceptingState(const std::string& name, const std::vector<char>& storage = std::vector<char> ());


    /**
     * @brief Processes an input string through the Turing Machine
     *
     * @param input The string to be processed by the Turing Machine
     *
     * @return True if the input string is part of the language described by the TM
     */
    bool process(const std::string& input) const;

    /**
     * @brief Destructor
     */

    virtual ~TuringMachine();

private:
    std::vector<StatePtr> fStates;
    std::set<char> fAlphabet;
    std::set<char> fTapeAlphabet;
    std::vector<TuringTransition> fTransitions;
    StatePtr fStartState = nullptr;
    char fBlank;
    std::set<StatePtr> fAccepting;
    int fStateStorageSize = -1;   //-1 is temporary value, will be set when first transition is added
    int fTrackCount = -1;
};


/**
 * @brief Generates a Turing Machine from an XML file
 *
 * @param fileName Name of the XML file
 *
 * @return TM Pointer to the generated Turing Machine
 */
TuringPtr generateTM(const std::string& fileName);

#endif /*TURING_H*/
