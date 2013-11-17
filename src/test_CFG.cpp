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
 * Last modified: 17 November 2013
 * By: Stijn Wouters
 */
#include "Catch.h"
#include "CFG.h"
#include <set>
#include <map>
#include <stdexcept>

TEST_CASE("Production rules", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'S', 'A', 'B', 'C'};
    const char start = 'S';

    const std::multimap<char, SymbolString> productions = {
                                                        {'S', "AB"},
                                                        {'A', ""},
                                                        {'A', "a"},
                                                        {'A', "AA"},
                                                        {'B', "b"},
                                                        {'B', "BB"}
                                                        };

    const CFG c(terminals, variables, productions, start);

    std::set<SymbolString> s_productions = {"AB"};
    std::set<SymbolString> a_productions = {"a", "AA", ""};
    std::set<SymbolString> b_productions = {"b", "BB"};

    CHECK(c.productions('S') == s_productions);
    CHECK(c.productions('A') == a_productions);
    CHECK(c.productions('B') == b_productions);
    CHECK(c.productions('C').empty());

    CHECK_THROWS_AS(c.productions('D'), std::invalid_argument);
}

TEST_CASE("Nullable symbols", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'S', 'A', 'B'};
    const char start = 'S';

    SECTION("no nullable symbols") {
        const std::multimap<char, SymbolString> productions = {
                                                            {'S', "AB"},
                                                            {'A', "a"},
                                                            {'A', "A"},
                                                            {'B', "b"},
                                                            {'B', "B"}
                                                            };

        const CFG c(terminals, variables, productions, start);

        CHECK(c.nullable().empty());
    }

    SECTION("only base case") {
        const std::multimap<char, SymbolString> productions = {
                                                            {'S', ""},
                                                            {'A', "a"},
                                                            {'A', "A"},
                                                            {'B', "b"},
                                                            {'B', "B"}
                                                            };

        const CFG c(terminals, variables, productions, start);

        std::set<char> s = {'S'};
        CHECK(c.nullable() == s);
    }

    SECTION("induction") {
        const std::multimap<char, SymbolString> productions = {
                                                            {'S', "A"},
                                                            {'A', "a"},
                                                            {'A', "A"},
                                                            {'A', ""},
                                                            {'B', "b"},
                                                            {'B', "B"}
                                                            };

        const CFG c(terminals, variables, productions, start);

        std::set<char> s_a = {'S', 'A'};
        CHECK(c.nullable() == s_a);
    }

    SECTION("all") {
        const std::multimap<char, SymbolString> productions = {
                                                            {'S', "AB"},
                                                            {'A', "a"},
                                                            {'A', "AA"},
                                                            {'A', ""},
                                                            {'B', "b"},
                                                            {'B', "BB"},
                                                            {'B', ""}
                                                            };

        const CFG c(terminals, variables, productions, start);

        CHECK(c.nullable() == variables);
    }
}

TEST_CASE("Eleminating epsilon productions", "[CFG]") {
    const std::set<char> terminals = {'a', 'b', 'c'};
    const std::set<char> variables = {'S', 'A', 'B'};
    const char start = 'S';

    const std::multimap<char, SymbolString> productions = {
                                                        {'S', "AB"},
                                                        {'A', ""},
                                                        {'A', "a"},
                                                        {'A', "AA"},
                                                        {'B', ""},
                                                        {'B', "c"},
                                                        {'B', "BbB"}
                                                        };

    CFG c(terminals, variables, productions, start);

    std::set<SymbolString> s_productions = {"AB"};
    std::set<SymbolString> a_productions = {"a", "AA", ""};
    std::set<SymbolString> b_productions = {"", "c", "BbB"};

    REQUIRE(c.nullable() == variables);
    REQUIRE(c.productions('S') == s_productions);
    REQUIRE(c.productions('A') == a_productions);
    REQUIRE(c.productions('B') == b_productions);
 
    c.eleminateEpsilonProductions();

    std::set<SymbolString> a_productions_e = {"a", "A", "AA"};
    std::set<SymbolString> b_productions_e = {"b", "Bb", "bB", "BbB", "c"};
    std::set<SymbolString> s_productions_e = {"A", "B", "AB"};

    CHECK(c.productions('A') == a_productions_e);
    CHECK(c.productions('B') == b_productions_e);
    CHECK(c.productions('S') == s_productions_e);
}
