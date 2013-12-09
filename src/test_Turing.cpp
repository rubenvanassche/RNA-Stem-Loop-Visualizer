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
        REQUIRE(TMT1.getSymbolsAtHead() == cell1a);
        REQUIRE(TMT1.getSymbolsAtHead() != cell1d);
        TMT1.replaceSymbolsAtHead(cell1d);
        TMT1.moveHead(L);
        REQUIRE(TMT1.getSymbolsAtHead() == cell1e);
        REQUIRE(TMT1.getSymbolsAtHead() != cell1a);
        TMT1.moveHead(R);
        TMT1.moveHead(R);
        REQUIRE(TMT1.getSymbolsAtHead() == cell1b);
        REQUIRE(TMT1.getSymbolsAtHead() != cell1c);
        TMT1.replaceSymbolsAtHead(cell1c);
        TMT1.moveHead(R);
        REQUIRE(TMT1.getSymbolsAtHead() == cell1c);
        REQUIRE(TMT1.getSymbolsAtHead() != cell1b);
        TMT1.replaceSymbolsAtHead(cell1b);
        TMT1.moveHead(R);
        REQUIRE(TMT1.getSymbolsAtHead() == cell1d);
        REQUIRE(TMT1.getSymbolsAtHead() != cell1a);
        TMT1.replaceSymbolsAtHead(cell1a);
        REQUIRE(TMT1.getSymbolsAtHead() == cell1a);
        REQUIRE(TMT1.getSymbolsAtHead() != cell1d);
        TMT1.moveHead(L);
        REQUIRE(TMT1.getSymbolsAtHead() == cell1b);
        REQUIRE(TMT1.getSymbolsAtHead() != cell1c);
        TMT1.moveHead(L);
        REQUIRE(TMT1.getSymbolsAtHead() == cell1c);
        REQUIRE(TMT1.getSymbolsAtHead() != cell1b);
        TMT1.moveHead(L);
        REQUIRE(TMT1.getSymbolsAtHead() == cell1d);
        REQUIRE(TMT1.getSymbolsAtHead() != cell1a);

        REQUIRE(TMT2.getSymbolsAtHead() == cell2a);
        REQUIRE(TMT2.getSymbolsAtHead() != cell2e);
        TMT2.replaceSymbolsAtHead(cell2e);
        TMT2.moveHead(R);
        REQUIRE(TMT2.getSymbolsAtHead() == cell2b);
        REQUIRE(TMT2.getSymbolsAtHead() != cell2e);
        TMT2.replaceSymbolsAtHead(cell2e);
        TMT2.moveHead(R);
        REQUIRE(TMT2.getSymbolsAtHead() == cell2c);
        REQUIRE(TMT2.getSymbolsAtHead() != cell2f);
        TMT2.replaceSymbolsAtHead(cell2f);
        TMT2.moveHead(R);
        REQUIRE(TMT2.getSymbolsAtHead() == cell2d);
        REQUIRE(TMT2.getSymbolsAtHead() != cell2e);
        TMT2.moveHead(L);
        REQUIRE(TMT2.getSymbolsAtHead() == cell2f);
        REQUIRE(TMT2.getSymbolsAtHead() != cell2c);
        TMT2.moveHead(L);
        REQUIRE(TMT2.getSymbolsAtHead() == cell2e);
        REQUIRE(TMT2.getSymbolsAtHead() != cell2b);
        TMT2.moveHead(L);
        REQUIRE(TMT2.getSymbolsAtHead() == cell2e);
        REQUIRE(TMT2.getSymbolsAtHead() != cell2a);

        REQUIRE(TMT3.getSymbolsAtHead() == cell3a);
        REQUIRE(TMT3.getSymbolsAtHead() != cell3e);
        TMT3.replaceSymbolsAtHead(cell3e);
        TMT3.moveHead(R);
        REQUIRE(TMT3.getSymbolsAtHead() == cell3b);
        REQUIRE(TMT3.getSymbolsAtHead() != cell3e);
        TMT3.replaceSymbolsAtHead(cell3e);
        TMT3.moveHead(R);
        REQUIRE(TMT3.getSymbolsAtHead() == cell3c);
        REQUIRE(TMT3.getSymbolsAtHead() != cell3e);
        TMT3.replaceSymbolsAtHead(cell3e);
        TMT3.moveHead(R);
        REQUIRE(TMT3.getSymbolsAtHead() == cell3d);
        REQUIRE(TMT3.getSymbolsAtHead() != cell3e);
        TMT3.moveHead(L);
        REQUIRE(TMT3.getSymbolsAtHead() == cell3e);
        TMT3.moveHead(L);
        REQUIRE(TMT3.getSymbolsAtHead() == cell3e);
        TMT3.moveHead(L);
        REQUIRE(TMT3.getSymbolsAtHead() == cell3e);
        TMT3.moveHead(L);
        REQUIRE(TMT3.getSymbolsAtHead() == cell3d);
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
        REQUIRE(ID1.getStateAndSymbols() == out1);
        ID1.step(TS2, IDvec3, R);
        REQUIRE(ID1.getStateAndSymbols() == out2);
        ID1.step(TS3, IDvec4, R);
        REQUIRE(ID1.getStateAndSymbols() == out3);
        ID1.step(TS4, IDvec5, R);
        ID1.step(TS4, IDvec5, L);
        REQUIRE(ID1.getStateAndSymbols() == out6);
        ID1.step(TS5, IDvec5, L);
        REQUIRE(ID1.getStateAndSymbols() == out5);
        ID1.step(TS1, IDvec5, L);
        REQUIRE(ID1.getStateAndSymbols() == out4);
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
    try {   //Note that these constructors are called twice: once to check if runtime error is thrown, once to check if it's the right one
        REQUIRE_THROWS_AS(TuringMachine TM3(alphabet, alphabetT, 'D'), std::runtime_error);
        TuringMachine TM3(alphabet, alphabetT, 'D');
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error in TuringMachine constructor: Blank symbol not in tape alphabet. Blank symbol set to 'B' and added to tape alphabet if necessary. Correct behaviour of this TM no longer guaranteed.");
    }
    try {
        REQUIRE_THROWS(TuringMachine TM4(alphabetT, alphabet, 'B'));
        TuringMachine TM4(alphabetT, alphabet, 'B');
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error in TuringMachine constructor: Blank symbol not in tape alphabet. Blank symbol set to 'B' and added to tape alphabet if necessary. Correct behaviour of this TM no longer guaranteed.");
    }
}

TEST_CASE("State Adding", "[TM]") {
    std::set<char> alphabet;
    std::set<char> alphabetT;
    alphabet.insert('a'); alphabet.insert('b'); alphabet.insert('c');  alphabet.insert('d');
    alphabetT.insert('a'); alphabetT.insert('b'); alphabetT.insert('c'); alphabetT.insert('B');
    TuringMachine TM1(alphabet, alphabetT, 'B');
    std::vector<char> storage1; std::vector<char> storage2; std::vector<char> storage3; std::vector<char> storage4; std::vector<char> storageEmpty;
    storage1.push_back('a'); storage1.push_back('b'); storage2.push_back('c'); storage2.push_back('d'); storage3.push_back('a');
    storage4.push_back('a'); storage4.push_back('b'); storage4.push_back('c');
    SECTION("Adding states without storage") {
        try {
            CHECK(TM1.addState("Q1", 1, 0));
            CHECK(TM1.addState("Q2", 0, 0));
            CHECK(TM1.addState("Q3", 0, 0));
            CHECK(TM1.addState("Q4", 0, 1));
            CHECK(TM1.addState("Q5", 0, 1));
            CHECK(TM1.addState("Somethingelse", 0, 1));
        }
        catch(...) {
            std::cout << "Error while adding valid states" << std::endl;
        }
    }
    SECTION("Adding states with storage") {
        try {
            CHECK(TM1.addState("Q1", 1, 0, storage1));
            CHECK(TM1.addState("Q1", 0, 0, storage2));
            CHECK(TM1.addState("Q2", 0, 1, storage1));
        }
        catch(...) {
            std::cout << "Error while adding valid states with storage" << std::endl;
        }
    }
    SECTION("Adding invalid states without storage") {
        CHECK(TM1.addState("Q1", 1, 0));
        SECTION ("Throw 1") {
            try {
                REQUIRE_THROWS(TM1.addState("Q1", 0, 0));
                TM1.addState("Q1", 0, 0);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding state: Name is not unique!");
            }
        }
        SECTION ("Throw 2") {
            try {
                REQUIRE_THROWS(TM1.addState("Q2", 1, 0));
                TM1.addState("Q2", 1, 0);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding state: Trying to create second start state!");
            }
        }
    }
    SECTION("Adding invalid states with storage") {
        CHECK(TM1.addState("Q1", 1, 0, storage1));
        CHECK(TM1.addState("Q1", 0, 0, storage2));
        SECTION("Throw 1") {
            try {
                REQUIRE_THROWS(TM1.addState("Q1", 0, 0, storage1));
                TM1.addState("Q1", 0, 0, storage1);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding state: Name + storage is not unique!");
            }
        }
        SECTION("Throw 2") {
            try {
                REQUIRE_THROWS(TM1.addState("Q1", 0, 0, storage3));
                TM1.addState("Q1", 0, 0, storage3);

            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding state: All storages should have same size!");
            }
        }
        SECTION("Throw 3") {
            try {
                REQUIRE_THROWS(TM1.addState("Q1", 0, 0, storage4));
                TM1.addState("Q1", 0, 0, storage4);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding state: All storages should have same size!");
            }
        }
        SECTION("Throw 4") {
            try {
                REQUIRE_THROWS(TM1.addState("Q1", 0, 0, storageEmpty));
                TM1.addState("Q1", 0, 0, storageEmpty);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding state: All storages should have same size!");
            }
        }
    }
}
TEST_CASE("Transition Adding", "[TM]") {
    std::set<char> alphabet;
    std::set<char> alphabetT;
    alphabet.insert('a'); alphabet.insert('b'); alphabet.insert('c');  alphabet.insert('d');
    alphabetT.insert('a'); alphabetT.insert('b'); alphabetT.insert('c'); alphabetT.insert('d'); alphabetT.insert('B');
    TuringMachine TM1(alphabet, alphabetT, 'B');
    std::vector<char> storage1; std::vector<char> storage2; std::vector<char> storage3; std::vector<char> storage4; std::vector<char> storage5; std::vector<char> storageEmpty;
    storage1.push_back('a'); storage1.push_back('b'); storage2.push_back('c'); storage2.push_back('d'); storage3.push_back('a');
    storage4.push_back('a'); storage4.push_back('b'); storage4.push_back('c'); storage5.push_back('a'); storage5.push_back('e');
    SECTION ("Adding valid transitions without storage or multitrack") {
        TM1.addState("Q1", 1, 0);
        TM1.addState("Q2", 0, 0);
        TM1.addState("Q3", 0, 0);
        TM1.addState("Q4", 0, 1);
        CHECK(TM1.addTransition("Q1", "Q2", 'a', 'b', L));
        CHECK(TM1.addTransition("Q2", "Q2", 'c', 'c', L));
        CHECK(TM1.addTransition("Q2", "Q3", 'a', 'B', R));
        CHECK(TM1.addTransition("Q4", "Q1", 'b', 'a', R));
    }
    SECTION ("Adding valid transitions with storage and multitrack") {
        TM1.addState("Q1", 1, 0, storage1);
        TM1.addState("Q1", 0, 0, storage2);
        TM1.addState("Q2", 0, 0, storage1);
        TM1.addState("Q2", 0, 1, storage2);
        TM1.addState("Q3", 0, 1, storage1);
        TM1.addState("Q3", 0, 0, storage2);
        CHECK(TM1.addTransition("Q1", "Q2", storage1, storage2, L, storage1, storage1));
        CHECK(TM1.addTransition("Q2", "Q2", storage2, storage1, L, storage2, storage2));
        CHECK(TM1.addTransition("Q2", "Q3", storage1, storage1, R, storage1, storage2));
        CHECK(TM1.addTransition("Q2", "Q1", storage2, storage2, R, storage2, storage1));
    }
    SECTION ("Adding invalid transitions without storage or multitrack") {
        TM1.addState("Q1", 1, 0);
        TM1.addState("Q2", 0, 0);
        TM1.addState("Q3", 0, 0);
        TM1.addState("Q4", 0, 1);
        CHECK(TM1.addTransition("Q1", "Q2", 'a', 'b', L));
        SECTION("Throw 1") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q1", "Q2", 'a', 'b', L));
                TM1.addTransition("Q1", "Q2", 'a', 'b', L);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: Transition not unique!");
            }

        }
        SECTION("Throw 2") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q5", "Q1", 'a', 'b', L));
                TM1.addTransition("Q5", "Q1", 'a', 'b', L);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: From state not in set of states!");
            }
        }

        SECTION("Throw 3") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q1", "Q5", 'a', 'b', L));
                TM1.addTransition("Q1", "Q5", 'a', 'b', L);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: To state not in set of states!");
            }
        }
        SECTION("Throw 4") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q1", "Q2", 'e', 'b', L));
                TM1.addTransition("Q1", "Q2", 'e', 'b', L);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: Symbol to be read not in tape alphabet!");
            }
        }
        SECTION("Throw 5") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q1", "Q2", 'a', 'e', L));
                TM1.addTransition("Q1", "Q2", 'a', 'e', L);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: Symbol to be written not in tape alphabet!");
            }
        }


    }
    SECTION ("Adding invalid transitions with storage and multitrack") {
        TM1.addState("Q1", 1, 0, storage1);
        TM1.addState("Q1", 0, 0, storage2);
        TM1.addState("Q2", 0, 0, storage1);
        TM1.addState("Q2", 0, 1, storage2);
        TM1.addState("Q3", 0, 1, storage1);
        TM1.addState("Q3", 0, 0, storage2);
        CHECK(TM1.addTransition("Q1", "Q2", storage1, storage2, L, storage1, storage1));
        SECTION ("Throw 1") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q1", "Q2", storage1, storage2, L, storage1, storage1));
                TM1.addTransition("Q1", "Q2", storage1, storage2, L, storage1, storage1);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: Transition not unique!");
            }
        }
        SECTION ("Throw 2") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q1", "Q2", storage1, storage2, L, storage3, storage1));
                TM1.addTransition("Q1", "Q2", storage1, storage2, L, storage3, storage1);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: Storages do not have same size!");
            }
        }
        SECTION ("Throw 3") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q1", "Q2", storage1, storage2, L, storage3, storage3));
                TM1.addTransition("Q1", "Q2", storage1, storage2, L, storage3, storage3);

            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: Storages do not have right size!");
            }
        }
        SECTION ("Throw 4") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q1", "Q2", storage3, storage3, L, storage1, storage1));
                TM1.addTransition("Q1", "Q2", storage3, storage3, L, storage1, storage1);

            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: Read and write character count does not match track count!");
            }
        }
        SECTION ("Throw 5") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q1", "Q2", storage3, storage2, L, storage1, storage1));
                TM1.addTransition("Q1", "Q2", storage3, storage2, L, storage1, storage1);

            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: Read and write do not have same number of characters!");
            }
        }
        SECTION ("Throw 6") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q4", "Q2", storage1, storage2, L, storage1, storage1));
                TM1.addTransition("Q4", "Q2", storage1, storage2, L, storage1, storage1);

            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: From state not in set of states!");
            }
        }
        SECTION ("Throw 7") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q1", "Q4", storage1, storage2, L, storage1, storage1));
                TM1.addTransition("Q1", "Q4", storage1, storage2, L, storage1, storage1);

            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: To state not in set of states!");
            }
        }
        SECTION ("Throw 8") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q1", "Q2", storage5, storage2, L, storage1, storage1));
                TM1.addTransition("Q1", "Q2", storage5, storage2, L, storage1, storage1);

            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: Symbol to be read not in tape alphabet!");
            }
        }
        SECTION ("Throw 9") {
            try {
                REQUIRE_THROWS(TM1.addTransition("Q1", "Q2", storage1, storage5, L, storage1, storage1));
                TM1.addTransition("Q1", "Q2", storage1, storage5, L, storage1, storage1);
            }
            catch(std::runtime_error& e) {
                std::string error(e.what());
                REQUIRE(error == "Error adding transition: Symbol to be written not in tape alphabet!");
            }
        }


    }


}
//other setting functions are considered thorougly tested through the following test
TEST_CASE("XML", "[TM]") {
    SECTION("Turing Machine without storage or multitrack (0^n1^n for n at least 1)") {
        std::string fileName = "TM1.xml";
        TuringPtr TM1 (generateTM(fileName));
        std::string input;
        input = "00001111";
        CHECK(TM1->process(input));
        input = "000011111";
        CHECK_FALSE(TM1->process(input));
        input = "0000111";
        CHECK_FALSE(TM1->process(input));
        input = "0001111";
        CHECK_FALSE(TM1->process(input));
        input = "000001111";
        CHECK_FALSE(TM1->process(input));
        input = "00000";
        CHECK_FALSE(TM1->process(input));
        input = "1";
        CHECK_FALSE(TM1->process(input));
        input = "10";
        CHECK_FALSE(TM1->process(input));
        input = "";
        CHECK_FALSE(TM1->process(input));
        input = "01";
        CHECK(TM1->process(input));
        input = "0101";
        CHECK_FALSE(TM1->process(input));

    }
    SECTION("Turing Machine with storage") {
        std::string fileName = "TM2.xml";
        TuringPtr TM2(generateTM(fileName));
        std::string input2;
        input2 = "01111";
        CHECK(TM2->process(input2));
        input2 = "011110";
        CHECK_FALSE(TM2->process(input2));
        input2 = "";
        CHECK_FALSE(TM2->process(input2));
        input2 = "0";
        CHECK(TM2->process(input2));
        input2 = "1";
        CHECK(TM2->process(input2));
        input2 = "1000";
        CHECK(TM2->process(input2));
        input2 = "10001";
        CHECK_FALSE(TM2->process(input2));
        input2 = "10";
        CHECK(TM2->process(input2));
        input2 = "01";
        CHECK(TM2->process(input2));

    }
    SECTION("Turing Machine with storage and multitrack") {
        std::string fileName = "TM3.xml";
        TuringPtr TM3(generateTM(fileName));
        std::string input3;
        input3 = "010c010";
        CHECK(TM3->process(input3));
        input3 = "01c01";
        CHECK(TM3->process(input3));
        input3 = "0c0";
        CHECK(TM3->process(input3));
        input3 = "00001c00001";
        CHECK(TM3->process(input3));
        input3 = "1111c1111";
        CHECK(TM3->process(input3));
        input3 = "0101c010";
        CHECK_FALSE(TM3->process(input3));
        input3 = "0101c101";
        CHECK_FALSE(TM3->process(input3));
        input3 = "010c010c010";
        CHECK_FALSE(TM3->process(input3));
        input3 = "010010";
        CHECK_FALSE(TM3->process(input3));
        input3 = "";
        CHECK_FALSE(TM3->process(input3));
        input3 = "0c1";
        CHECK_FALSE(TM3->process(input3));
        try {
            input3 = "abc";
            REQUIRE_THROWS_AS(TM3->process(input3), std::runtime_error);
            TM3->process(input3);
        }
        catch (std::runtime_error& e) {
            std::string error = e.what();
            REQUIRE(error == "Error while processing input string: Character in input but not in input alphabet!");
        }
    }
}

TEST_CASE("XML invalid", "[TM]") {
    try {
        std::string fileName = "TMinvalid0.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: File not found!");
    }
    try {
        std::string fileName = "TMinvalid1.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: Failed to load file: No root element.");
    }
    try {
        std::string fileName = "TMinvalid2.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: Not a Turing Machine XML file!");
    }
    try {
        std::string fileName = "TMinvalid3.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: One input symbol per node please");
    }
    try {
        std::string fileName = "TMinvalid4.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: One input symbol per node please");
    }
    try {
        std::string fileName = "TMinvalid5.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: One blank symbol please");
    }
    try {
        std::string fileName = "TMinvalid6.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: Alphabet, tape alphabet or blank symbol missing!");
    }
    try {
        std::string fileName = "TMinvalid7.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: Alphabet, tape alphabet or blank symbol missing!");
    }
    try {
        std::string fileName = "TMinvalid8.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: Alphabet, tape alphabet or blank symbol missing!");
    }
    try {
        std::string fileName = "TMinvalid9.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: State without name");
    }
    try {
        std::string fileName = "TMinvalid10.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: invalid direction");
    }
    try {
        std::string fileName = "TMinvalid11.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: Incomplete transition");
    }
    try {
        std::string fileName = "TMinvalid12.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: Incomplete transition");
    }
    try {
        std::string fileName = "TMinvalid13.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: No name for start state specified");
    }
    try {
        std::string fileName = "TMinvalid14.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error generating TM from XML: No name for accepting state specified");
    }
    try {
        std::string fileName = "TMinvalid15.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error adding transition: Symbol to be written not in tape alphabet!");
    }
    try {
        std::string fileName = "TMinvalid16.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error adding transition: From state not in set of states!");
    }
    try {
        std::string fileName = "TMinvalid17.xml";
        REQUIRE_THROWS_AS(generateTM(fileName), std::runtime_error);
        generateTM(fileName);
    }
    catch(std::runtime_error& e) {
        std::string error(e.what());
        REQUIRE(error == "Error adding state: Name is not unique!");
    }

}
