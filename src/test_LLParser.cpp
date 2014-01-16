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
 * Last modified: 16 January 2013
 * By: Pieter Lauwers
 */

#include "Catch.h"
#define private public
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

TEST_CASE("Direct variables", "[LLParser]") {
    std::set<char> CFGVariables ({'S', 'X', 'Y', 'Z'});

    std::multimap<char, SymbolString> CFGProductions;
    CFGProductions.insert(std::pair<char, SymbolString>('S', "X"));
    CFGProductions.insert(std::pair<char, SymbolString>('S', "Y"));
    CFGProductions.insert(std::pair<char, SymbolString>('S', EPSILON));
    CFGProductions.insert(std::pair<char, SymbolString>('X', "Z"));
    CFGProductions.insert(std::pair<char, SymbolString>('Y', "S"));

    std::set<char> directVariables1 ({'S'});
    LLTable::getDirectVariables('S', CFGVariables, CFGProductions, directVariables1);
    CHECK(directVariables1 == std::set<char> ({'S', 'X', 'Y', 'Z'}));

    std::set<char> directVariables2 ({'X'});
    LLTable::getDirectVariables('X', CFGVariables, CFGProductions, directVariables2);
    CHECK(directVariables2 == std::set<char> ({'X', 'Z'}));

    std::set<char> directVariables3 ({'Y'});
    LLTable::getDirectVariables('Y', CFGVariables, CFGProductions, directVariables3);
    CHECK(directVariables3 == std::set<char> ({'Y', 'S', 'X', 'Z'}));

    std::set<char> directVariables4 ({'Z'});
    LLTable::getDirectVariables('Z', CFGVariables, CFGProductions, directVariables4);
    CHECK(directVariables4 == std::set<char> ({'Z'}));
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
}

TEST_CASE("Parsing input", "[LLParser]") {
    std::vector<LLParser*> parsers;
    std::vector<std::vector<std::string>* > correctInput;
    std::vector<std::vector<std::string>* > wrongInput;

    // Parser 1
    std::set<char> CFGTerminals1 ({'x', 'y', 'z'});

    std::set<char> CFGVariables1 ({'S'});

    std::multimap<char, SymbolString> CFGProductions1;
    CFGProductions1.insert(std::pair<char, SymbolString>('S', "xSz"));
    CFGProductions1.insert(std::pair<char, SymbolString>('S', "ySz"));
    CFGProductions1.insert(std::pair<char, SymbolString>('S', EPSILON));

    char CFGStartsymbol1 = 'S'; 

    unsigned int lookahead1 = 1;

    LLParser parser1 = LLParser(CFGTerminals1, CFGVariables1, CFGProductions1, CFGStartsymbol1, lookahead1);
    parsers.push_back(&parser1);

    std::vector<std::string> correctInput1 ({"xz", "", "yz", "xxyzzz", "yyyyyyyyyyzzzzzzzzzz"});
    correctInput.push_back(&correctInput1);
    std::vector<std::string> wrongInput1 ({"xzz", " ", "z", "y", "x", "xxxxxxxxzzzzzzzzzzzzzzz"});
    wrongInput.push_back(&wrongInput1);

    // Parser 2
    std::set<char> CFGTerminals2 ({'a', 'b'});

    std::set<char> CFGVariables2 ({'S', 'X'});

    std::multimap<char, SymbolString> CFGProductions2;
    CFGProductions2.insert(std::pair<char, SymbolString>('S', "aX"));
    CFGProductions2.insert(std::pair<char, SymbolString>('X', "ab"));
    CFGProductions2.insert(std::pair<char, SymbolString>('X', "bb"));

    char CFGStartsymbol2 = 'S'; 

    unsigned int lookahead2 = 2;

    LLParser parser2 = LLParser(CFGTerminals2, CFGVariables2, CFGProductions2, CFGStartsymbol2, lookahead2);
    parsers.push_back(&parser2);

    std::vector<std::string> correctInput2 ({"abb", "aab"});
    correctInput.push_back(&correctInput2);
    std::vector<std::string> wrongInput2 ({"abbb", "a", "b", ""});
    wrongInput.push_back(&wrongInput2);

    // Testing
    for (unsigned int i = 0; i != 2; i++) {
        // Correct input:
        for (unsigned int j = 0; j != correctInput[i]->size(); j++)
        {
            CHECK(parsers[i]->process(correctInput[i]->at(j)));
        }
        // Wrong input:
        for (unsigned int j = 0; j != wrongInput[i]->size(); j++)
        {
            CHECK(not parsers[i]->process(wrongInput[i]->at(j)));
        }
    }
}

TEST_CASE("RNA CFG", "[LLParser]") {
    SECTION("parse table") {
            processInput("../data/LLP_RNAin.txt", "../data/LLP_RNAout.txt", false);
            CHECK(compare_files("../data/LLP_RNAout.txt", "../data/LLP_RNAexp.txt"));
    }
    SECTION("parsing") {
        std::set<char> CFGTerminals;         
        std::set<char> CFGVariables;
        std::multimap<char, SymbolString> CFGProductions;
        char CFGStartsymbol;
        unsigned int lookahead;

        readInput("../data/LLP_RNAin.txt", CFGTerminals, CFGVariables, CFGProductions, CFGStartsymbol, lookahead);
        LLParser parser (CFGTerminals, CFGVariables, CFGProductions, CFGStartsymbol, lookahead);

        std::vector<std::string> correctInput ({"acgxcgu", "axu", "gxxxxc", "guuuuucaxxxugaaaaac"});
        for (unsigned int i = 0; i != correctInput.size(); ++i) {
            CHECK(parser.process(correctInput[i]));
        }

        std::vector<std::string> wrongInput ({"acgcgu", "axc", "uxg", "axxxuc", "acgxxxxx"});
        for (unsigned int i = 0; i != wrongInput.size(); ++i) {
            CHECK(not parser.process(wrongInput[i]));
        }
    }
}

TEST_CASE("RNAParser", "[LLParser]") {
    SECTION("correct input & correct stemsize") {
        std::vector<std::string> input ({"ACGACGU", "AAU", "GGCAUC", "GUUUUUCACGAUGAAAAAC"});
        std::vector<unsigned int> stemSize ({3, 1, 1, 8});
        for (unsigned int i = 0; i != input.size(); ++i) {
            CHECK(RNAParser::parse(input[i], stemSize[i]));
        }
    }

    SECTION("correct input & wrong stemsize") {
        std::vector<std::string> input ({"AAU", "GGCAUC", "GUUUUUCACGAUGAAAAAC"});
        std::vector<unsigned int> stemSize ({2, 2, 9});
        for (unsigned int i = 0; i != input.size(); ++i) {
            CHECK(not RNAParser::parse(input[i], stemSize[i]));
        }
    }

    SECTION("wrong input & correct stemsize") {
        std::vector<std::string> input ({"HIDKKDLKEODKKD", "ACGCGU", "UAG", "ACGCUC"});
        std::vector<unsigned int> stemSize ({3, 3, 1, 1});
        for (unsigned int i = 0; i != input.size(); ++i) {
            CHECK(not RNAParser::parse(input[i], stemSize[i]));
        }
    }

    SECTION("calculate stemsize") {
        std::vector<std::string> input ({"ACGACGU", "AAU", "GGCAUC", "GUUUUUCACGAUGAAAAAC", "HIDKKDLKEODKKD", "UAG", "ACGCUC"});
        std::vector<unsigned int> stemSize ({3, 1, 1, 8, 0, 0, 0});
        for (unsigned int i = 0; i != input.size(); ++i) {
            CHECK(RNAParser::parse(input[i]) == stemSize[i]);
        }
    }

    SECTION("find stemloop") {
        std::vector<std::string> input ({"ACGACGU", "AAU", "GGCAUC", "GUUUUUCACGAUGAAAAAC", "HIDKKDLKEODKKD", "UAG", "ACGCUC", "UUACGACGUGG"});
        std::vector<unsigned int> stemSize ({3, 1, 1, 8, 0, 0, 1, 3});
        std::vector<unsigned int> begins ({0, 0, 0, 0, 0, 0, 0, 2});
        std::vector<unsigned int> ends ({7, 3, 6, 19, 14, 3, 5, 9});
        for (unsigned int i = 0; i != input.size(); ++i) {
            unsigned int result=0, begin=0, end=0;
            CHECK(RNAParser::parse(input[i], result, begin, end) == stemSize[i]);
            if (stemSize[i] != 0) {
                CHECK(begin == begins[i]);
                CHECK(end == ends[i]);
            }
        }
    }
}