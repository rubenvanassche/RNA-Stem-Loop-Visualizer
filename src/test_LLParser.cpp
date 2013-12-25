/* 
 * test_LLParser.cpp
 *
 * Copyright (C) 2013 Pieter Lauwers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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
 * Last modified: 13 December 2013
 * By: Pieter Lauwers
 */

#include "Catch.h"
#include "LLParserInputOutput.h"

using namespace LLP;

TEST_CASE("Enumeration of terminals", "[LLParser]") {
    SECTION("2 terminals, length 2") {
        std::vector<SymbolString> result;
        std::vector<SymbolString> symbols;
        symbols.push_back("a");
        symbols.push_back("b");

        LLTable::enumerate(result, symbols, 2);

        std::vector<SymbolString> expected ({"ba", "bb", "aa", "ab"});
        REQUIRE(result.size() == expected.size());

        for (unsigned int i = 0; i != result.size(); i++) {
            CHECK(result[i] == expected[i]);
        }
    }

    SECTION("1 terminal + EOS, length 2") {
        std::vector<SymbolString> result;
        std::vector<SymbolString> symbols;
        symbols.push_back("a");
        symbols.push_back(" ");

        LLTable::enumerate(result, symbols, 2);

        std::vector<SymbolString> expected ({"  ", "aa", "a "});
        REQUIRE(result.size() == expected.size());

        for (unsigned int i = 0; i != result.size(); i++) {
            CHECK(result[i] == expected[i]);
        }
    }

    SECTION("2 terminals + EOS, length 3") {
        std::vector<SymbolString> result;
        std::vector<SymbolString> symbols;
        symbols.push_back("a");
        symbols.push_back("b");
        symbols.push_back(" ");

        LLTable::enumerate(result, symbols, 3);

        std::vector<SymbolString> expected ({"a  ", "aba", "abb", "ab ", "aaa", "aab", "aa ", 
                                             "b  ", "bba", "bbb", "bb ", "baa", "bab", "ba ", "   "});
        REQUIRE(result.size() == expected.size());

        for (unsigned int i = 0; i != result.size(); i++) {
            CHECK(result[i] == expected[i]);
        }
    }

    SECTION("5 terminals + EOS, length 5") {
        std::vector<SymbolString> result;
        std::vector<SymbolString> symbols;
        symbols.push_back("a");
        symbols.push_back("b");
        symbols.push_back("c");
        symbols.push_back("d");
        symbols.push_back("e");
        symbols.push_back(" ");

        LLTable::enumerate(result, symbols, 5);

        /*
         * Calculation of size:
         *
         * t = terminal
         * _ = EOS
         *
         * t t t t t : 5.5.5.5.5 = 3125
         * t t t t _ : 5.5.5.5.1 = 625
         * t t t _ _ : 5.5.5.1.1 = 125
         * t t _ _ _ : 5.5.1.1.1 = 25
         * t _ _ _ _ : 5.1.1.1.1 = 5
         * _ _ _ _ _ : 1.1.1.1.1 = 1
         *
         * TOTAL: 3906
         */
        CHECK(result.size() == 3906);

        // Each combination can only occur once:
        for (unsigned int i = 0; i != 1; i++) {
        //for (unsigned int i = 0; i != result.size(); i++) {
            for (unsigned int j = i+1; j != result.size(); j++) {
                CHECK(result[i] != result[j]);
            }
        }
    }
}

bool compare_files(std::string resultFile, std::string expectedFile) {
    std::ifstream result (resultFile, std::ifstream::in);
    std::ifstream expected (expectedFile, std::ifstream::in);
   
    if (!result) return false; 
    if (!expected) return false; 

    std::string resultLine;
    std::string expectedLine;
    while (!result.eof()) {
        if(expected.eof()) return false;
        std::getline(result, resultLine);
        std::getline(expected, expectedLine);
        if(resultLine != expectedLine) return false;
    }
    if(!expected.eof()) return false;
    return true;
}

TEST_CASE("Generating LLParsetables", "[LLParser]") {
    std::string path = "../data/";
    std::string extension = ".txt";
    for (unsigned int i = 1; i != 9; i++) {
        std::stringstream ss;
        ss << "LLP" << i;
        std::string prefix = ss.str();

        std::string inputFile = path + prefix + "in" + extension;
        std::string resultFile = path + prefix + "out" + extension;
        std::string expectedFile = path + prefix + "exp" + extension;

        if (i < 10) { SECTION("correct grammar: " + prefix) {
            processInput(inputFile, resultFile, false);
            CHECK(compare_files(resultFile, expectedFile));
        }}
    }
    compare_files("../data/LLP1in.txt", "../data/LLP1out");
}