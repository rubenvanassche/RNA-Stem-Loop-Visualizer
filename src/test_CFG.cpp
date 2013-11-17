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

TEST_CASE("Unit pairs", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'S', 'A', 'B'};
    const char start = 'S';

    SECTION("Only base case") {
        const std::multimap<char, SymbolString> productions = {
                                                            {'S', "AB"},
                                                            {'A', "a"},
                                                            {'B', "b"}
                                                            };

        const CFG c(terminals, variables, productions, start);

        std::set< std::pair<char, char> > units = { 
                                                {'S', 'S'}, 
                                                {'A', 'A'},
                                                {'B', 'B'}
                                                };
        CHECK(c.units() == units);
    }


    SECTION("With recursion") {
        const std::multimap<char, SymbolString> productions = {
                                                            {'S', "AB"},
                                                            {'S', "B"},
                                                            {'S', "A"},
                                                            {'A', "a"},
                                                            {'B', "b"}
                                                            };

        const CFG c(terminals, variables, productions, start);

        std::set< std::pair<char, char> > units = { 
                                                {'S', 'S'}, 
                                                {'S', 'A'}, 
                                                {'S', 'B'}, 
                                                {'A', 'A'},
                                                {'B', 'B'},
                                                };
        CHECK(c.units() == units);
    }
}

TEST_CASE("Eleminating unit productions", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'S', 'A', 'B'};
    const char start = 'S';

    const std::multimap<char, SymbolString> productions = {
                                                        {'S', "AB"},
                                                        {'S', "B"},
                                                        {'S', "A"},
                                                        {'A', "a"},
                                                        {'A', "AA"},
                                                        {'B', "b"},
                                                        {'B', "BB"}
                                                        };

    CFG c(terminals, variables, productions, start);

    std::set<SymbolString> s_productions = {"AB", "A", "B"};
    std::set<SymbolString> a_productions = {"a", "AA"};
    std::set<SymbolString> b_productions = {"b", "BB"};
    std::set< std::pair<char, char> > units = { 
                                            {'S', 'S'}, 
                                            {'S', 'A'}, 
                                            {'S', 'B'}, 
                                            {'A', 'A'},
                                            {'B', 'B'},
                                            };

    REQUIRE(c.productions('S') == s_productions);
    REQUIRE(c.productions('A') == a_productions);
    REQUIRE(c.productions('B') == b_productions);
    REQUIRE(c.units() == units);
 
    c.eleminateUnitProductions();

    std::set<SymbolString> s_productions_u = {"AB", "a", "AA", "b", "BB"};

    CHECK(c.productions('S') == s_productions_u);
    CHECK(c.productions('A') == a_productions);
    CHECK(c.productions('B') == b_productions);
}

TEST_CASE("Generating symbols", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'S', 'A', 'B'};
    const char start = 'S';

    const std::multimap<char, SymbolString> productions = {
                                                        {'S', "AB"},
                                                        {'S', "a"},
                                                        {'A', "b"}
                                                        };

    const CFG c(terminals, variables, productions, start);

    std::set<char> generating = {'S', 'A', 'b', 'a'};

    CHECK(c.generating() == generating);
}

TEST_CASE("Reachable symbols", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'S', 'A', 'B'};
    const char start = 'S';

    SECTION("Some symbols are reachable") {
        const std::multimap<char, SymbolString> productions = {
                                                            {'S', "a"},
                                                            {'A', "b"}
                                                            };

        const CFG c(terminals, variables, productions, start);

        std::set<char> reachable = {'S', 'a'};

        CHECK(c.reachable() == reachable);
    }

    SECTION("All symbols are reachable") {
        const std::multimap<char, SymbolString> productions = {
                                                            {'S', "AB"},
                                                            {'S', "a"},
                                                            {'A', "b"}
                                                            };

        const CFG c(terminals, variables, productions, start);

        std::set<char> reachable = {'S', 'A', 'b', 'B', 'a'};

        CHECK(c.reachable() == reachable);
    }
}

TEST_CASE("Eleminating useless symbols", "[CFG]") {
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'S', 'A', 'B'};
    const char start = 'S';

    const std::multimap<char, SymbolString> productions = {
                                                        {'S', "AB"},
                                                        {'S', "a"},
                                                        {'A', "b"}
                                                        };

    CFG c(terminals, variables, productions, start);

    std::set<SymbolString> s_bodies = {"AB", "a"};
    std::set<SymbolString> a_bodies = {"b"};
    std::set<char> generating = {'S', 'A', 'b', 'a'};
    std::set<char> reachable = {'S', 'A', 'b', 'B', 'a'};

    REQUIRE(c.productions('S') == s_bodies);
    REQUIRE(c.productions('A') == a_bodies);
    REQUIRE(c.productions('B').empty());
    REQUIRE(c.generating() == generating);
    REQUIRE(c.reachable() == reachable);

    c.eleminateUselessSymbols();
    std::set<SymbolString> s_bodies_u = {"a"};

    CHECK(c.productions('S') == s_bodies_u);
    CHECK(c.productions('A').empty());
    CHECK(c.productions('B').empty());
}
