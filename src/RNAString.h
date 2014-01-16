/*
 * RNAString.h
 *
 * Copyright (C) 2014 Jakob Struye
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
 * Last modified: 2 January 2014
 * By: Jakob Struye
 */

#include <string>
#include <iostream>
#include "Turing.h"

class RNAString {
public:
    /**
     * @brief default constructor
     */
    RNAString();
    /**
     * @brief Constructor using the information stored in a Tape
     *
     * @param tape The tape
     */
    RNAString(Tape tape);

    /**
     * @brief Gets the i'th letter (nucleotide) in the string
     *
     * @brief i Index of the nucleotide
     *
     * @return The nucleotide (as a char)
     */
    char getLetterAt(int i) const;

    /**
     * @brief Gets the i'th loop sign (part of stem or loop?) in the string
     *
     * @brief i Index of the nucleotide
     *
     * @return 0 if loop, X if stem, ? if neither
     */
    char getLoopSignAt(int i) const;

    /**
     * @brief add character in front of loop
     *
     * @param nucl Nucleotide type
     */

    void push_front(char nucl);

    /**
     * @brief add character in back of loop
     *
     * @param nucl Nucleotide type
     */
    void push_back(char nucl);

    /**
     * @brief gets the size of the string
     *
     * @return size of the string
     */
    int getSize() const;

    friend std::ostream& operator<<(std::ostream& os, RNAString);
private:
    std::string fString;
    std::string fLoops;
};

std::ostream& operator<<(std::ostream& os, RNAString);
