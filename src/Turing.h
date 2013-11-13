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
        TuringState* from,
        TuringState* to,
        const char& read,
        const char& write,
        Direction dir
    );


    virtual ~TuringTransition();

private:
    TuringState *fFrom = nullptr;
    TuringState *fTo = nullptr;
    char fRead;
    char fWrite;
    Direction fDirection;
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
     * @param state A TuringState object representing an Turing Machine state
     * @param isEnding A bool describing if the state is a final state
     * @param isStarting A bool describing if the state is the start state
     *
     * @return A bool telling if the state is added or not
     */
    bool addState(
            const TuringState& state,
            const bool& isFinal,
            const bool& isStarting
            );

     /**
     * @brief Add a new transition to the Turing Machine
     *
     * @param transition A TuringTransition object representing an Turing Machine transition
     *
     * @return A bool telling if the transition is added or not
     */
    bool addTransition(const TuringTransition& transition);

    /**
     * @brief Process an input string through the Turing Machine
     *
     * @param input The string to be processed by the Turing Machine
     *
     * @return A bool telling if the Turing Machine accepted
     */
    bool process(std::string input);

    virtual ~TuringMachine();

private:
    std::set<TuringState> fStates;
    std::set<char> fAlphabet;
    std::set<char> fTapeAlphabet;
    std::set<TuringTransition> fTransitions;
    TuringTransition* fStartState;
    char fBlank;
    std::set<TuringState*> fAccepting;

};


#endif /*TURING_H*/
