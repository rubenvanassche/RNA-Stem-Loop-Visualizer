/* Test Turing Machine
 *
 * Copyright (C) 2013 Jakob Struye
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
 * Last modified: 24 November 2013
 * By: Jakob Struye
 */

#define CATCH_CONFIG_MAIN
#include "Catch.h"
#include "Turing.h"
#include <set>
#include <vector>
#include <stdexcept>



TEST_CASE("TM States", "[TMState]") {

    std::vector<char> storage;
    std::vector<char> storage2;
    std::vector<char> storageEmpty;
    storage.push_back('a');
    storage.push_back('b');
    storage2.push_back('c');
    storage2.push_back('d');
    //no storage
    try {
        TuringState TS1("") ;
        TuringState TS2("Q1");
        TuringState TS3("Q2");
        CHECK(TS1.isCalled(""));
        CHECK_FALSE(TS1.isCalled("Q1"));
        CHECK_FALSE(TS1.isCalled("Q2"));
        CHECK_FALSE(TS2.isCalled(""));
        CHECK(TS2.isCalled("Q1"));
        CHECK_FALSE(TS2.isCalled("Q2"));
        CHECK_FALSE(TS3.isCalled(""));
        CHECK_FALSE(TS3.isCalled("Q1"));
        CHECK(TS3.isCalled("Q2"));
        CHECK(TS1.hasThisStorage(storageEmpty));
        CHECK_FALSE(TS1.hasThisStorage(storage));
        CHECK(TS2.hasThisStorage(storageEmpty));
        CHECK_FALSE(TS2.hasThisStorage(storage));
        CHECK(TS3.hasThisStorage(storageEmpty));
        CHECK_FALSE(TS3.hasThisStorage(storage));
    }
    catch (const std::invalid_argument& e) {
            FAIL("Got exception: " << e.what());
    }
    //Storage
    try {
        TuringState TS4("Q1", storage);
        TuringState TS5("Q2", storage2);
        CHECK(TS4.isCalled("Q1"));
        CHECK_FALSE(TS4.isCalled("Q2"));
        CHECK_FALSE(TS5.isCalled("Q1"));
        CHECK(TS5.isCalled("Q2"));
        CHECK(TS4.hasThisStorage(storage));
        CHECK_FALSE(TS4.hasThisStorage(storage2));
        CHECK_FALSE(TS4.hasThisStorage(storageEmpty));
        CHECK_FALSE(TS5.hasThisStorage(storage));
        CHECK(TS5.hasThisStorage(storage2));
        CHECK_FALSE(TS5.hasThisStorage(storageEmpty));
    }
    catch (const std::invalid_argument& e) {
                FAIL("Got exception: " << e.what());
    }
}

TEST_CASE("TM Transitions", "[TMTrans]") {
    std::vector<char> storage;
    std::vector<char> storage2;
    std::vector<char> storageEmpty;
    storage.push_back('a');
    storage.push_back('b');
    storage2.push_back('c');
    storage2.push_back('d');
    StatePtr TS1(new TuringState(""));
    StatePtr TS2(new TuringState("Q1"));
    StatePtr TS3(new TuringState("Q2"));
    StatePtr TS4(new TuringState("Q3"));
    StatePtr TS5(new TuringState("Q4"));
    std::vector<char> read1;
    std::vector<char> read2;
    std::vector<char> write1;
    std::vector<char> write2;
    read1.push_back('a');
    read2.push_back('b');
    write1.push_back('A');
    write2.push_back('B');
    std::tuple<StatePtr, std::vector<char>, Direction> result1(TS2, write1, L);
    std::tuple<StatePtr, std::vector<char>, Direction> result1b(TS1, write1, L);
    std::tuple<StatePtr, std::vector<char>, Direction> result1c(TS2, write2, L);
    std::tuple<StatePtr, std::vector<char>, Direction> result1d(TS2, write1, R);
    std::tuple<StatePtr, std::vector<char>, Direction> result2(TS2, write2, L);
    std::tuple<StatePtr, std::vector<char>, Direction> result2b(TS2, write1, L);
    std::tuple<StatePtr, std::vector<char>, Direction> result3(TS2, write1, R);
    std::tuple<StatePtr, std::vector<char>, Direction> result3b(TS2, write1, L);
    std::tuple<StatePtr, std::vector<char>, Direction> result4(TS3, write2, L);
    std::tuple<StatePtr, std::vector<char>, Direction> result4b(TS5, write2, L);
    std::tuple<StatePtr, std::vector<char>, Direction> result5(TS1, write2, L);
    std::tuple<StatePtr, std::vector<char>, Direction> result5b(TS2, write1, R);
    try {
        TuringTransition TT1(TS1, TS2, read1, write1, L);
        TuringTransition TT2(TS2, TS2, read2, write2, L);
        TuringTransition TT3(TS5, TS2, read2, write1, R);
        TuringTransition TT4(TS4, TS3, read2, write2, L);
        TuringTransition TT5(TS4, TS1, read2, write2, L);
        CHECK(TT1.match(TS1, read1));
        CHECK_FALSE(TT1.match(TS1, read2));
        CHECK_FALSE(TT1.match(TS2, read1));
        CHECK_FALSE(TT1.match(TS3, read1));
        CHECK_FALSE(TT1.match(TS4, read1));
        CHECK_FALSE(TT1.match(TS5, read1));
        CHECK_FALSE(TT1.match(TS5, read2));
        CHECK(TT2.match(TS2, read2));
        CHECK_FALSE(TT2.match(TS1, read2));
        CHECK_FALSE(TT2.match(TS1, read1));
        REQUIRE(TT1.getTransition() == result1);
        REQUIRE(TT1.getTransition() != result1b);
        REQUIRE(TT1.getTransition() != result1c);
        REQUIRE(TT1.getTransition() != result1d);
        REQUIRE(TT2.getTransition() == result2);
        REQUIRE(TT2.getTransition() != result2b);
        REQUIRE(TT3.getTransition() == result3);
        REQUIRE(TT3.getTransition() != result3b);
        REQUIRE(TT4.getTransition() == result4);
        REQUIRE(TT4.getTransition() != result4b);
        REQUIRE(TT5.getTransition() == result5);
        REQUIRE(TT5.getTransition() != result5b);
        CHECK(TT1.isThisTransition(TS1, TS2, read1, write1, L));
        CHECK(TT2.isThisTransition(TS2, TS2, read2, write2, L));
        CHECK(TT3.isThisTransition(TS5, TS2, read2, write1, R));
        CHECK(TT4.isThisTransition(TS4, TS3, read2, write2, L));
        CHECK(TT5.isThisTransition(TS4, TS1, read2, write2, L));
        CHECK_FALSE(TT1.isThisTransition(TS5, TS2, read1, write1, L));
        CHECK_FALSE(TT2.isThisTransition(TS2, TS1, read2, write2, L));
        CHECK_FALSE(TT3.isThisTransition(TS5, TS2, read1, write1, R));
        CHECK_FALSE(TT4.isThisTransition(TS4, TS3, read2, write1, L));
        CHECK_FALSE(TT5.isThisTransition(TS4, TS1, read2, write2, R));
    }
    catch(...) {
    }

}
TEST_CASE("TM Tape", "[Tape]") {
    std::vector<char> cell1a; std::vector<char> cell1b; std::vector<char> cell1c; std::vector<char> cell1d; std::vector<char> cell1e;
    cell1a.push_back('T'); cell1b.push_back('e'); cell1c.push_back('s'); cell1d.push_back('t'); cell1e.push_back('B');
    std::vector<char> cell2a; std::vector<char> cell2b; std::vector<char> cell2c; std::vector<char> cell2d; std::vector<char> cell2e; std::vector<char> cell2f;
    cell2a.push_back('F'); cell2a.push_back('C');
    cell2b.push_back('o'); cell2b.push_back('C');
    cell2c.push_back('o'); cell2c.push_back('C');
    cell2d.push_back('C'); cell2d.push_back('C');
    cell2e.push_back('F'); cell2e.push_back('x');
    cell2f.push_back('o'); cell2f.push_back('x');
    std::vector<char> cell3a; std::vector<char> cell3b; std::vector<char> cell3c; std::vector<char> cell3d; std::vector<char> cell3e; std::vector<char> cell3f; std::vector<char> cell3g;
    cell3a.push_back('B'); cell3a.push_back('D'); cell3a.push_back('D'); cell3a.push_back('D'); cell3a.push_back('D'); cell3a.push_back('D'); cell3a.push_back('D'); cell3a.push_back('D'); cell3a.push_back('D'); cell3a.push_back('D');
    cell3b.push_back('a'); cell3b.push_back('D'); cell3b.push_back('D'); cell3b.push_back('D'); cell3b.push_back('D'); cell3b.push_back('D'); cell3b.push_back('D'); cell3b.push_back('D'); cell3b.push_back('D'); cell3b.push_back('D');
    cell3c.push_back('r'); cell3c.push_back('D'); cell3c.push_back('D'); cell3c.push_back('D'); cell3c.push_back('D'); cell3c.push_back('D'); cell3c.push_back('D'); cell3c.push_back('D'); cell3c.push_back('D'); cell3c.push_back('D');
    cell3d.push_back('D'); cell3d.push_back('D'); cell3d.push_back('D'); cell3d.push_back('D'); cell3d.push_back('D'); cell3d.push_back('D'); cell3d.push_back('D'); cell3d.push_back('D'); cell3d.push_back('D'); cell3d.push_back('D');
    cell3e.push_back('z'); cell3e.push_back('x'); cell3e.push_back('x');  cell3e.push_back('x');  cell3e.push_back('D');  cell3e.push_back('D');  cell3e.push_back('D');  cell3e.push_back('D');  cell3e.push_back('D');  cell3e.push_back('D');
    std::string str1 = "Test";
    std::string str2 = "Foo";
    std::string str3 = "Bar";
    try {
        Tape TMT1(str1, 'B', 1);
        Tape TMT2(str2, 'C', 2);
        Tape TMT3 (str3, 'D', 10);
        REQUIRE(TMT1.getSymbolAtHead() == cell1a);
        REQUIRE(TMT1.getSymbolAtHead() != cell1d);
        TMT1.replaceSymbolAtHead(cell1d);
        TMT1.moveHead(L);
        REQUIRE(TMT1.getSymbolAtHead() == cell1e);
        REQUIRE(TMT1.getSymbolAtHead() != cell1a);
        TMT1.moveHead(R);
        TMT1.moveHead(R);
        REQUIRE(TMT1.getSymbolAtHead() == cell1b);
        REQUIRE(TMT1.getSymbolAtHead() != cell1c);
        TMT1.replaceSymbolAtHead(cell1c);
        TMT1.moveHead(R);
        REQUIRE(TMT1.getSymbolAtHead() == cell1c);
        REQUIRE(TMT1.getSymbolAtHead() != cell1b);
        TMT1.replaceSymbolAtHead(cell1b);
        TMT1.moveHead(R);
        REQUIRE(TMT1.getSymbolAtHead() == cell1d);
        REQUIRE(TMT1.getSymbolAtHead() != cell1a);
        TMT1.replaceSymbolAtHead(cell1a);
        REQUIRE(TMT1.getSymbolAtHead() == cell1a);
        REQUIRE(TMT1.getSymbolAtHead() != cell1d);
        TMT1.moveHead(L);
        REQUIRE(TMT1.getSymbolAtHead() == cell1b);
        REQUIRE(TMT1.getSymbolAtHead() != cell1c);
        TMT1.moveHead(L);
        REQUIRE(TMT1.getSymbolAtHead() == cell1c);
        REQUIRE(TMT1.getSymbolAtHead() != cell1b);
        TMT1.moveHead(L);
        REQUIRE(TMT1.getSymbolAtHead() == cell1d);
        REQUIRE(TMT1.getSymbolAtHead() != cell1a);

        REQUIRE(TMT2.getSymbolAtHead() == cell2a);
        REQUIRE(TMT2.getSymbolAtHead() != cell2e);
        TMT2.replaceSymbolAtHead(cell2e);
        TMT2.moveHead(R);
        REQUIRE(TMT2.getSymbolAtHead() == cell2b);
        REQUIRE(TMT2.getSymbolAtHead() != cell2e);
        TMT2.replaceSymbolAtHead(cell2e);
        TMT2.moveHead(R);
        REQUIRE(TMT2.getSymbolAtHead() == cell2c);
        REQUIRE(TMT2.getSymbolAtHead() != cell2f);
        TMT2.replaceSymbolAtHead(cell2f);
        TMT2.moveHead(R);
        REQUIRE(TMT2.getSymbolAtHead() == cell2d);
        REQUIRE(TMT2.getSymbolAtHead() != cell2e);
        TMT2.moveHead(L);
        REQUIRE(TMT2.getSymbolAtHead() == cell2f);
        REQUIRE(TMT2.getSymbolAtHead() != cell2c);
        TMT2.moveHead(L);
        REQUIRE(TMT2.getSymbolAtHead() == cell2e);
        REQUIRE(TMT2.getSymbolAtHead() != cell2b);
        TMT2.moveHead(L);
        REQUIRE(TMT2.getSymbolAtHead() == cell2e);
        REQUIRE(TMT2.getSymbolAtHead() != cell2a);

        REQUIRE(TMT3.getSymbolAtHead() == cell3a);
        REQUIRE(TMT3.getSymbolAtHead() != cell3e);
        TMT3.replaceSymbolAtHead(cell3e);
        TMT3.moveHead(R);
        REQUIRE(TMT3.getSymbolAtHead() == cell3b);
        REQUIRE(TMT3.getSymbolAtHead() != cell3e);
        TMT3.replaceSymbolAtHead(cell3e);
        TMT3.moveHead(R);
        REQUIRE(TMT3.getSymbolAtHead() == cell3c);
        REQUIRE(TMT3.getSymbolAtHead() != cell3e);
        TMT3.replaceSymbolAtHead(cell3e);
        TMT3.moveHead(R);
        REQUIRE(TMT3.getSymbolAtHead() == cell3d);
        REQUIRE(TMT3.getSymbolAtHead() != cell3e);
        TMT3.moveHead(L);
        REQUIRE(TMT3.getSymbolAtHead() == cell3e);
        TMT3.moveHead(L);
        REQUIRE(TMT3.getSymbolAtHead() == cell3e);
        TMT3.moveHead(L);
        REQUIRE(TMT3.getSymbolAtHead() == cell3e);
        TMT3.moveHead(L);
        REQUIRE(TMT3.getSymbolAtHead() == cell3d);
    }
    catch(...) {
    }
}

TEST_CASE("TM ID", "[TMID]") {
    std::string str1 = "Foo";
    std::string str2 = "Bar";
    StatePtr TS1(new TuringState(""));
    StatePtr TS2(new TuringState("Q1"));
    StatePtr TS3(new TuringState("Q2"));
    StatePtr TS4(new TuringState("Q3"));
    StatePtr TS5(new TuringState("Q4"));
    std::vector<char> IDvec1; std::vector<char> IDvec2; std::vector<char> IDvec3; std::vector<char> IDvec4; std::vector<char> IDvec5;
    IDvec1.push_back('F'); IDvec2.push_back('o'); IDvec3.push_back('B'); IDvec4.push_back('a'); IDvec5.push_back('r');
    std::pair<StatePtr, std::vector<char>> out1(TS1, IDvec1);
    std::pair<StatePtr, std::vector<char>> out2(TS2, IDvec2);
    std::pair<StatePtr, std::vector<char>> out3(TS3, IDvec2);
    std::pair<StatePtr, std::vector<char>> out4(TS1, IDvec3);
    std::pair<StatePtr, std::vector<char>> out5(TS5, IDvec4);
    std::pair<StatePtr, std::vector<char>> out6(TS4, IDvec5);
    try {
        TMID ID1(str1, TS1, 'B', 1);
        REQUIRE(ID1.getStateAndSymbol() == out1);
        ID1.step(TS2, IDvec3, R);
        REQUIRE(ID1.getStateAndSymbol() == out2);
        ID1.step(TS3, IDvec4, R);
        REQUIRE(ID1.getStateAndSymbol() == out3);
        ID1.step(TS4, IDvec5, R);
        ID1.step(TS4, IDvec5, L);
        REQUIRE(ID1.getStateAndSymbol() == out6);
        ID1.step(TS5, IDvec5, L);
        REQUIRE(ID1.getStateAndSymbol() == out5);
        ID1.step(TS1, IDvec5, L);
        REQUIRE(ID1.getStateAndSymbol() == out4);
    }
    catch(...) {

    }
}

TEST_CASE("TM Construction", "[TM]") {
    std::set<char> alphabet;
    std::set<char> alphabetT;
    alphabet.insert('a'); alphabet.insert('b'); alphabet.insert('c');
    alphabetT.insert('a'); alphabetT.insert('b'); alphabetT.insert('c'); alphabetT.insert('B');
    //valid
    try {
        TuringMachine TM1(alphabet, alphabetT, 'B');
        TuringMachine TM2(alphabet, alphabetT, 'c');

    }
    catch(const std::invalid_argument& e) {
        std::cout << "Error while constructing valid TM: " << e.what() << std::endl;
    }
    //invalid
    try {
        TuringMachine TM3(alphabet, alphabetT, 'D');
        FAIL("Could construct TM TM3(alphabet, alphabetT, 'D')");
    }
    catch(...) {
    }
    try {
        TuringMachine TM4(alphabetT, alphabet, 'B');
        FAIL("Could construct TM TM4(alphabetT, alphabet, 'B')");
    }
    catch(...) {
    }
}

//All adding functions are hard to test separately, but are all used for processing and reading TMs from XML files


