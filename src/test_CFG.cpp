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
 * Last modified: 18 November 2013
 * By: Stijn Wouters
 */
#include "Catch.h"
#include "CFG.h"
#include <set>
#include <map>
#include <stdexcept>

TEST_CASE("Constructing CFG's", "[CFG]") {
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
        const CFG c0(a, A, empty_map, 'A');
        const CFG c1(a, A_B, empty_map, 'A');
        const CFG c2(a_b, A, empty_map, 'A');
        const CFG c3(a_b, A_B, empty_map, 'A');
        const CFG c4(a_b, A_B, empty_map, 'B');
        const CFG c5(a, A, valid_map, 'A');
        const CFG c6(a, A_B, valid_map, 'A');
        const CFG c7(a_b, A, valid_map, 'A');
        const CFG c8(a_b, A_B, valid_map, 'A');
        const CFG c9(a_b, A_B, valid_map, 'B');
    } catch (const std::invalid_argument& e) {
        FAIL("Got exception: " << e.what());
    } // end try-catch

    // invalid set of variables/terminals (not disjoint)
    try {
        const CFG c(A, A, empty_map, 'A');
        FAIL("Could construct const CFG c(A, A, empty_map, 'A');");
    } catch (const std::invalid_argument& e) {
        // expected to be here
    } // end try-catch

    try {
        const CFG c(A_B, A, empty_map, 'A');
        FAIL("Could construct const CFG c(A_B, A, empty_map, 'A');");
    } catch (const std::invalid_argument& e) {
        // expected to be here
    } // end try-catch

    try {
        const CFG c(A, A_B, empty_map, 'A');
        FAIL("Could construct const CFG c(A, A_B, empty_map, 'A');");
    } catch (const std::invalid_argument& e) {
        // expected to be here
    } // end try-catch

    // invalid production rule
    try {
        const CFG c(a_b, A_B, invalid_map, 'A');
        FAIL("Could construct const CFG c(a_b, A_B, invalid_map, 'A');");
    } catch (const std::invalid_argument& e) {
        // expected to be here
    } // end try-catch
}

TEST_CASE("Bodies of the production rules", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'A', 'B'};

    const std::multimap<char, SymbolString> empty_map;
    const std::multimap<char, SymbolString> productions = {
                                                {'A', ""},
                                                {'A', "aa"},
                                                {'A', "aA"},
                                                {'A', "ab"},
                                                {'A', "aB"},
                                                {'A', "Aa"},
                                                {'A', "AA"},
                                                {'A', "Ab"},
                                                {'A', "AB"}
                                                };

    const std::set<SymbolString> A_bodies = { 
                                                "aa", "aA", "ab", 
                                                "aB", "Aa", "AA",
                                                "Ab", "AB", ""
                                                };

    try {
        const CFG c0(terminals, variables, empty_map, 'A');
        CHECK(c0.bodies('A').empty());
        CHECK(c0.bodies('B').empty());
        CHECK_THROWS_AS(c0.bodies('a'), std::invalid_argument);
        CHECK_THROWS_AS(c0.bodies('b'), std::invalid_argument);
        CHECK_THROWS_AS(c0.bodies('C'), std::invalid_argument);

        const CFG c1(terminals, variables, productions, 'A');
        CHECK(c1.bodies('A') == A_bodies);
        CHECK(c1.bodies('B').empty());
        CHECK_THROWS_AS(c1.bodies('a'), std::invalid_argument);
        CHECK_THROWS_AS(c1.bodies('b'), std::invalid_argument);
        CHECK_THROWS_AS(c1.bodies('C'), std::invalid_argument);
    } catch (const std::invalid_argument& e) {
        FAIL("Could not construct CFG's: " << e.what());
    } // end try-catch
}

TEST_CASE("Nullable symbols", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'A', 'B'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> A_nullable = {
                                                        {'A', ""},
                                                        {'B', "b"}
                                                        };
    const std::multimap<char, SymbolString> all_nullable = { 
                                                        {'A', "AB"}, 
                                                        {'A', ""}, 
                                                        {'B', ""}
                                                        };

    try {
        const CFG c0(terminals, variables, empty, 'A');
        CHECK(c0.nullable().empty());

        std::set<char> A = {'A'};
        const CFG c1(terminals, variables, A_nullable, 'A');
        CHECK(c1.nullable() == A);

        const CFG c2(terminals, variables, all_nullable, 'A');
        CHECK(c2.nullable() == variables);
    } catch (const std::invalid_argument& e) {
        FAIL("Could not construct CFG's:" << e.what());
    } // end try-catch
}

TEST_CASE("Eleminate epsilon productions", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'A', 'B'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> A_nullable = {
                                                        {'A', ""},
                                                        {'B', "b"}
                                                        };
    const std::multimap<char, SymbolString> all_nullable = { 
                                                        {'A', "AB"}, 
                                                        {'A', "AaB"}, 
                                                        {'A', ""}, 
                                                        {'B', ""}
                                                        };

    try {
        CFG c0(terminals, variables, empty, 'A');
        REQUIRE(c0.nullable().empty());
        c0.eleminateEpsilonProductions();
        CHECK(c0.nullable().empty());

        std::set<char> A = {'A'};
        std::set<SymbolString> A_bodies_nullable1 = {""};
        std::set<SymbolString> B_bodies_nullable1 = {"b"};
        CFG c1(terminals, variables, A_nullable, 'A');
        REQUIRE(c1.nullable() == A);
        REQUIRE(c1.bodies('A') == A_bodies_nullable1);
        REQUIRE(c1.bodies('B') == B_bodies_nullable1);
        c1.eleminateEpsilonProductions();
        CHECK(c1.nullable().empty());
        CHECK(c1.bodies('A').empty());
        CHECK(c1.bodies('B') == B_bodies_nullable1);

        std::set<SymbolString> A_bodies_nullable2 = {"", "AB", "AaB"};
        std::set<SymbolString> A_bodies_not_nullable2 = {"A", "B", "AB", "a", "Aa", "aB", "AaB"};
        std::set<SymbolString> B_bodies_nullable2 = {""};
        CFG c2(terminals, variables, all_nullable, 'A');
        REQUIRE(c2.nullable() == variables);
        REQUIRE(c2.bodies('A') == A_bodies_nullable2);
        REQUIRE(c2.bodies('B') == B_bodies_nullable2);
        c2.eleminateEpsilonProductions();
        CHECK(c2.nullable().empty());
        CHECK(c2.bodies('A') == A_bodies_not_nullable2);
        CHECK(c2.bodies('B').empty());
    } catch (const std::invalid_argument& e) {
        FAIL("Could not construct CFG's: " << e.what());
    } // end try-catch
}

TEST_CASE("Unit pairs", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'A', 'B', 'C'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> productions = {
                                                        {'A', "AB"},
                                                        {'B', "A"},
                                                        {'B', "a"}
                                                        };

    try {
        std::set<std::pair<char, char>> empty_units = { 
                                                    {'A', 'A'}, 
                                                    {'B', 'B'},
                                                    {'C', 'C'}
                                                    };
        const CFG c0(terminals, variables, empty, 'A');
        CHECK(c0.units() == empty_units);

        std::set<std::pair<char, char>> productions_units = { 
                                                        {'A', 'A'}, 
                                                        {'B', 'B'},
                                                        {'B', 'A'},
                                                        {'C', 'C'}
                                                        };
        const CFG c1(terminals, variables, productions, 'A');
        CHECK(c1.units() == productions_units);
    } catch (const std::invalid_argument& e) {
        FAIL("Could not construct CFG's: " << e.what());
    } // end try-catch
}

TEST_CASE("Eleminating unit productions", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'A', 'B', 'C'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> productions = {
                                                        {'A', "AB"},
                                                        {'B', "A"},
                                                        {'B', "a"}
                                                        };

    try {
        std::set<std::pair<char, char>> empty_units = { 
                                                    {'A', 'A'}, 
                                                    {'B', 'B'},
                                                    {'C', 'C'}
                                                    };
        CFG c0(terminals, variables, empty, 'A');
        REQUIRE(c0.units() == empty_units);
        REQUIRE(c0.bodies('A').empty());
        REQUIRE(c0.bodies('B').empty());
        REQUIRE(c0.bodies('C').empty());
        c0.eleminateUnitProductions();
        CHECK(c0.units() == empty_units);
        CHECK(c0.bodies('A').empty());
        CHECK(c0.bodies('B').empty());
        CHECK(c0.bodies('C').empty());

        std::set<std::pair<char, char>> productions_units = { 
                                                        {'A', 'A'}, 
                                                        {'B', 'B'},
                                                        {'B', 'A'},
                                                        {'C', 'C'}
                                                        };
        std::set<SymbolString> A_bodies = {"AB"};
        std::set<SymbolString> B_bodies = {"A", "a"};
        std::set<SymbolString> B_bodies_no_units = {"AB", "a"};
        CFG c1(terminals, variables, productions, 'A');
        REQUIRE(c1.units() == productions_units);
        REQUIRE(c1.bodies('A') == A_bodies);
        REQUIRE(c1.bodies('B') == B_bodies);
        REQUIRE(c1.bodies('C').empty());
        c1.eleminateUnitProductions();
        CHECK(c1.units() == empty_units);
        CHECK(c1.bodies('A') == A_bodies);
        CHECK(c1.bodies('B') == B_bodies_no_units);
        CHECK(c1.bodies('C').empty());
    } catch (const std::invalid_argument& e) {
        FAIL("Could not construct CFG's: " << e.what());
    } // end try-catch

}

TEST_CASE("Generating symbols of CFG", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'A', 'B'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> productions = {
                                                        {'A', "AB"},
                                                        {'B', "A"},
                                                        {'B', "a"}
                                                        };

    try {
        const CFG c0(terminals, variables, empty, 'A');
        CHECK(c0.generating() == terminals);

        std::set<char> generating_productions = {'a', 'b', 'B'};
        const CFG c1(terminals, variables, productions, 'A');
        CHECK(c1.generating() == generating_productions);
    } catch (const std::invalid_argument& e) {
        FAIL("Could not construct CFG's: " << e.what());
    } // end try-catch
}

TEST_CASE("Reachable symbols", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'A', 'B', 'C'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> productions = {
                                                        {'A', "AB"},
                                                        {'B', "A"},
                                                        {'B', "a"},
                                                        {'C', "abC"}
                                                        };

    try {
        std::set<char> A = {'A'};
        const CFG c0(terminals, variables, empty, 'A');
        CHECK(c0.reachable() == A);

        std::set<char> reachable_symbols = {'A', 'a', 'B'};
        const CFG c1(terminals, variables, productions, 'A');
        CHECK(c1.reachable() == reachable_symbols);
    } catch (const std::invalid_argument& e) {
        FAIL("Could not construct CFG's: " << e.what());
    } // end try-catch
}

TEST_CASE("Eleminating useless symbols", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'A', 'B', 'S'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> productions = {
                                                        {'S', "AB"},
                                                        {'S', "a"},
                                                        {'A', "b"}
                                                        };

    try {
        std::set<char> S = {'S'};
        CFG c0(terminals, variables, empty, 'S');
        REQUIRE(c0.reachable() == S);
        REQUIRE(c0.generating() == terminals);
        REQUIRE(c0.bodies('A').empty());
        REQUIRE(c0.bodies('B').empty());
        REQUIRE(c0.bodies('S').empty());
        c0.eleminateUselessSymbols();
        CHECK(c0.reachable() == S);
        CHECK(c0.generating() == terminals);
        CHECK(c0.bodies('A').empty());
        CHECK(c0.bodies('B').empty());
        CHECK(c0.bodies('S').empty());

        std::set<char> reachable_symbols = {'S', 'A', 'B', 'a', 'b'};
        std::set<char> generating_symbols = {'A', 'S', 'a', 'b'};
        std::set<SymbolString> S_bodies = {"AB", "a"};
        std::set<SymbolString> A_bodies = {"b"};
        std::set<char> reachable_symbols_no_useless = {'S', 'a'};
        std::set<char> generating_symbols_no_useless = {'S', 'a', 'b'};
        std::set<SymbolString> S_bodies_no_useless = {"a"};
        CFG c1(terminals, variables, productions, 'S');
        REQUIRE(c1.reachable() == reachable_symbols);
        REQUIRE(c1.generating() == generating_symbols);
        REQUIRE(c1.bodies('A') == A_bodies);
        REQUIRE(c1.bodies('B').empty());
        REQUIRE(c1.bodies('S') == S_bodies);
        c1.eleminateUselessSymbols();
        CHECK(c1.reachable() == reachable_symbols_no_useless);
        CHECK(c1.generating() == generating_symbols_no_useless);
        REQUIRE(c1.bodies('A').empty());
        REQUIRE(c1.bodies('B').empty());
        REQUIRE(c1.bodies('S') == S_bodies_no_useless);
    } catch (const std::invalid_argument& e) {
        FAIL("Could not construct CFG's: " << e.what());
    } // end try-catch
}

TEST_CASE("Clean up CFG", "[CFG]") {
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
        CFG c0(terminals, variables, empty, 'S');
        REQUIRE(c0.nullable().empty());
        REQUIRE(c0.units() == empty_units);
        REQUIRE(c0.reachable() == S);
        REQUIRE(c0.generating() == terminals);
        REQUIRE(c0.bodies('A').empty());
        REQUIRE(c0.bodies('B').empty());
        REQUIRE(c0.bodies('C').empty());
        REQUIRE(c0.bodies('S').empty());
        c0.cleanUp();
        CHECK(c0.nullable().empty());
        CHECK(c0.units().empty());
        CHECK(c0.reachable() == S);
        CHECK(c0.generating() == terminals);
        CHECK_THROWS_AS(c0.bodies('A'), std::invalid_argument);
        CHECK_THROWS_AS(c0.bodies('B'), std::invalid_argument);
        CHECK_THROWS_AS(c0.bodies('C'), std::invalid_argument);
        CHECK_THROWS_AS(c0.bodies('S'), std::invalid_argument);

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
        std::set<SymbolString> S_productions = {"A", "B"};
        std::set<SymbolString> A_productions = {"", "aAa"};
        std::set<SymbolString> B_productions = {"", "bBb"};
        std::set<SymbolString> C_productions = {"aCa", "bCb"};
        std::set< std::pair<char, char> > units_cleanup = {
                                                {'A', 'A'},
                                                {'B', 'B'},
                                                {'S', 'S'},
                                                };
        std::set<SymbolString> S_productions_cleanup = {"aa", "aAa", "bb", "bBb"};
        std::set<SymbolString> A_productions_cleanup = {"aa", "aAa"};
        std::set<SymbolString> B_productions_cleanup = {"bb", "bBb"};
        CFG c1(terminals, variables, productions, 'S');
        REQUIRE(c1.nullable() == nullable);
        REQUIRE(c1.units() == units);
        REQUIRE(c1.reachable() == reachable);
        REQUIRE(c1.generating() == generating);
        REQUIRE(c1.bodies('A') == A_productions);
        REQUIRE(c1.bodies('B') == B_productions);
        REQUIRE(c1.bodies('C') == C_productions);
        REQUIRE(c1.bodies('S') == S_productions);
        c1.cleanUp();
        CHECK(c1.nullable().empty());
        CHECK(c1.units() == units_cleanup);
        CHECK(c1.reachable() == reachable);
        CHECK(c1.generating() == generating);
        CHECK(c1.bodies('A') == A_productions_cleanup);
        CHECK(c1.bodies('B') == B_productions_cleanup);
        CHECK_THROWS_AS(c0.bodies('C'), std::invalid_argument);
        CHECK(c1.bodies('S') == S_productions_cleanup);
    } catch (const std::invalid_argument& e) {
        FAIL("Could not construct CFG's: " << e.what());
    } // end try-catch
}
