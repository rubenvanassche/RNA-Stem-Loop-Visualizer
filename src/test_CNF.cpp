/* Test Context Free Grammar
 * Test the CFG interface.
 *
 * Copyright (C) 2013 Stijn Wouters
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
 * Last modified: 20 November 2013
 * By: Stijn Wouters
 */
#include "Catch.h"
#include "CNF.h"
#include <set>
#include <map>
#include <stdexcept>

TEST_CASE("Constructing CNF's", "[CNF]") {
    const std::set<char> empty_set;
    const std::set<char> a = {'a'};
    const std::set<char> a_b = {'a', 'b'};
    const std::set<char> A = {'A'};
    const std::set<char> A_B = {'A', 'B'};

    const std::multimap<char, SymbolString> empty_map;
    const std::multimap<char, SymbolString> valid_map = { {'A', "a"} };
    const std::multimap<char, SymbolString> invalid_map = { {'a', "A"} };

    // first, valid construction
    try {
        const CNF c0(a, A, empty_map, 'A');
        const CNF c1(a, A_B, empty_map, 'A');
        const CNF c2(a_b, A, empty_map, 'A');
        const CNF c3(a_b, A_B, empty_map, 'A');
        const CNF c4(a_b, A_B, empty_map, 'B');
        const CNF c5(a, A, valid_map, 'A');
        const CNF c6(a, A_B, valid_map, 'A');
        const CNF c7(a_b, A, valid_map, 'A');
        const CNF c8(a_b, A_B, valid_map, 'A');
        const CNF c9(a_b, A_B, valid_map, 'B');
    } catch (const std::invalid_argument& e) {
        FAIL("Got exception: " << e.what());
    } // end try-catch

    // invalid set of variables/terminals (not disjoint)
    try {
        const CNF c(A, A, empty_map, 'A');
        FAIL("Could construct const CNF c(A, A, empty_map, 'A');");
    } catch (const std::invalid_argument& e) {
        // expected to be here
    } // end try-catch

    try {
        const CNF c(A_B, A, empty_map, 'A');
        FAIL("Could construct const CNF c(A_B, A, empty_map, 'A');");
    } catch (const std::invalid_argument& e) {
        // expected to be here
    } // end try-catch

    try {
        const CNF c(A, A_B, empty_map, 'A');
        FAIL("Could construct const CNF c(A, A_B, empty_map, 'A');");
    } catch (const std::invalid_argument& e) {
        // expected to be here
    } // end try-catch

    // invalid production rule
    try {
        const CNF c(a_b, A_B, invalid_map, 'A');
        FAIL("Could construct const CNF c(a_b, A_B, invalid_map, 'A');");
    } catch (const std::invalid_argument& e) {
        // expected to be here
    } // end try-catch
}

TEST_CASE("CNF with CFG methods", "[CNF]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'A', 'B', 'C', 'S'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> productions = {
                                                        {'S', "A"},
                                                        {'S', "B"},
                                                        {'A', "aAa"},
                                                        {'A', ""},
                                                        {'B', "bBb"},
                                                        {'B', ""},
                                                        {'C', "aCa"},
                                                        {'C', "bCb"}
                                                        };

    try {
        std::set<char> S = {'S'};
        std::set<std::pair<char, char>> empty_units = { 
                                                    {'A', 'A'}, 
                                                    {'B', 'B'},
                                                    {'C', 'C'}, 
                                                    {'S', 'S'}
                                                    };
        CNF c0(terminals, variables, empty, 'S');
        CHECK(c0.nullable().empty());
        CHECK(c0.units() == empty_units);
        CHECK(c0.reachable() == S);
        CHECK(c0.generating() == terminals);
        CHECK(c0.bodies('A').empty());
        CHECK(c0.bodies('B').empty());
        CHECK(c0.bodies('C').empty());
        CHECK(c0.bodies('S').empty());

        std::set<char> nullable = {'S', 'A', 'B'};
        std::set< std::pair<char, char> > units = {
                                                {'A', 'A'},
                                                {'C', 'C'},
                                                {'B', 'B'},
                                                {'S', 'S'},
                                                {'S', 'A'},
                                                {'S', 'B'}
                                                };
        std::set<char> reachable = {'S', 'A', 'B', 'a', 'b'};
        std::set<char> generating = {'a', 'b', 'A', 'B', 'S'};
        std::set<SymbolString> S_productions_cleanup = {"aa", "aAa", "bb", "bBb"};
        std::set<SymbolString> A_productions_cleanup = {"aa", "aAa"};
        std::set<SymbolString> B_productions_cleanup = {"bb", "bBb"};
        CNF c1(terminals, variables, productions, 'S');
        CHECK(c1.nullable().empty());
        CHECK(c1.units() == empty_units);
        CHECK(c1.reachable() == reachable);
        CHECK(c1.generating() == generating);
        CHECK(c1.bodies('A') == A_productions_cleanup);
        CHECK(c1.bodies('B') == B_productions_cleanup);
        CHECK(c1.bodies('C').empty());
        CHECK(c1.bodies('S') == S_productions_cleanup);
    } catch (const std::invalid_argument& e) {
        FAIL("Could not construct CNF's: " << e.what());
    } // end try-catch
}
