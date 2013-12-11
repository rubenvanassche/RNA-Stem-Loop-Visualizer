/*
 * test_PDA.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: Ruben
 */
#include "Catch.h"
#include "PDA.h"
#include "CFG.h"
#include <exception>

TEST_CASE("Stack Operation","[PDA]"){
	PDAState Q("Q", false);
	PDAState P("P", false);
	std::stack<char> stack;

	SECTION("push"){
		PDATransition t1(&Q, &P, '0', 0, PUSH, 'a');
		t1.stackOperation(stack);
		CHECK(stack.size() == 1);
		CHECK(stack.top() == 'a');
	}

	SECTION("push multiple"){
		std::vector<char> pushVector = {'a', 'b', 'c'};
		PDATransition t1(&Q, &P, '0', 0, PUSH, pushVector);
		t1.stackOperation(stack);
		CHECK(stack.size() == 3);
		CHECK(stack.top() == 'c');
	}

	SECTION("push when not empty"){
		stack.push('a');
		std::vector<char> pushVector = {'a', 'b', 'c'};
		PDATransition t1(&Q, &P, '0', 'a', PUSH, pushVector);
		t1.stackOperation(stack);
		CHECK(stack.size() == 4);
		CHECK(stack.top() == 'c');
	}

	SECTION("pop"){
		stack.push('a');
		PDATransition t1(&Q, &P, '0', 'a', POP);
		t1.stackOperation(stack);
		CHECK(stack.size() == 0);
	}

	SECTION("stay constant"){
		stack.push('a');
		PDATransition t1(&Q, &P, '0', 'a', STAY);
		t1.stackOperation(stack);
		CHECK(stack.size() == 1);
		CHECK(stack.top() == 'a');
	}

}

TEST_CASE("construction", "[PDA]"){
	std::set<char> alphabet;
	std::set<char> stackAlphabet;
	PDAFinal pdaType = STATE;

	alphabet.insert('0');
	alphabet.insert('1');
	stackAlphabet.insert('a');
	stackAlphabet.insert('b');

	PDA pda(alphabet, stackAlphabet, pdaType);

	PDAState Q("Q", false);
	PDAState P("P", false);
	PDAState R("R", false);

	CHECK(pda.addState(Q) == true);
	CHECK(pda.addState(P) == true);
	CHECK(pda.addState(R) == true);
	CHECK_THROWS_AS(pda.addState(Q), std::runtime_error);
	CHECK_THROWS_AS(pda.addState(P), std::runtime_error);
	CHECK_THROWS_AS(pda.addState(R), std::runtime_error);


	PDATransition t1(&Q, &P, '0', 'a', PUSH, 'b');
	CHECK(pda.addTransition(t1) == true);
	CHECK_THROWS_AS(pda.addTransition(t1), std::runtime_error);

	PDATransition t2(&Q, &P, '0', 'a', POP);
	CHECK(pda.addTransition(t2) == true);

	// c is not in stack alphabet
	PDATransition t3(&Q, &P, '0', 'c', POP);
	CHECK_THROWS_AS(pda.addTransition(t3), std::runtime_error);

	// 3 is not in stack alphabet
	PDATransition t4(&Q, &P, '3', 'a', POP);
	CHECK_THROWS_AS(pda.addTransition(t4), std::runtime_error);

	// State S doesn't exist in the PDA
	PDAState S("S", false);
	PDATransition t5(&Q, &S, '0', 'a', POP);
	CHECK_THROWS_AS(pda.addTransition(t5), std::runtime_error);
}

TEST_CASE("PDA 1", "[PDA]"){
	PDAState Q("Q");
	PDAState R("R");
	PDAState S("S", true);

	PDATransition t11(&Q, &R, '1', 9, PUSH, '1');
	PDATransition t12(&Q, &R, '0', 9, PUSH, '0');

	PDATransition t21(&R, &S, '1', '0', POP);
	PDATransition t22(&R, &S, '0', '1', POP);

	PDATransition t31(&S, &R, '1', 9, PUSH, '1');
	PDATransition t32(&S, &R, '0', 9, PUSH, '0');

	std::set<char> alphabet = {'0', '1'};
	std::set<char> stackAlphabet = {'0', '1'};
	PDAFinal pdaType = STATE;


	PDA pda(alphabet, stackAlphabet, pdaType);

	pda.addState(Q, true);
	pda.addState(R);
	pda.addState(S);

	pda.addTransition(t11);
	pda.addTransition(t12);
	pda.addTransition(t21);
	pda.addTransition(t22);
	pda.addTransition(t31);
	pda.addTransition(t32);

	CHECK(pda.process("10") == true);
	CHECK(pda.process("01") == true);
	CHECK(pda.process("11") == false);
	CHECK(pda.process("00") == false);
	CHECK(pda.process("0") == false);
	CHECK(pda.process("1") == false);
	CHECK(pda.process("") == false);

	CHECK(pda.process("1010") == true);
	CHECK(pda.process("0101") == true);
	CHECK(pda.process("011001") == true);
	CHECK(pda.process("01010111101010") == false);
	CHECK(pda.process("111") == false);
	CHECK(pda.process("001") == false);
}


TEST_CASE("PDA 2", "[PDA]"){
	PDAState P("P");
	PDAState Q("Q");
	PDAState R("R", true);

	PDATransition t11(&P, &Q, 0, 9, PUSH, 'Z');

	PDATransition t21(&Q, &Q, 'e', 'Z', POP);
	PDATransition t22(&Q, &Q, 'i', 'Z', PUSH, 'Z');

	PDATransition t31(&Q, &R, 5, 9, POP);


	std::set<char> alphabet = {'e', 'i'};
	std::set<char> stackAlphabet = {'Z', 'X'};
	PDAFinal pdaType = STATE;


	PDA pda(alphabet, stackAlphabet, pdaType);

	pda.addState(P, true);
	pda.addState(Q);
	pda.addState(R);

	pda.addTransition(t11);
	pda.addTransition(t21);
	pda.addTransition(t22);
	pda.addTransition(t31);

	CHECK(pda.process("") == false);
	CHECK(pda.process("e") == true);
	CHECK(pda.process("ie") == false);
	CHECK(pda.process("iee") == true);
	CHECK(pda.process("iiee") == false);
	CHECK(pda.process("ieieieie") == false);

}


TEST_CASE("PDA 3", "[PDA]"){
	PDAState Q("Q");

	PDATransition t21(&Q, &Q, 'e', 'Z', POP);
	PDATransition t22(&Q, &Q, 'i', 'Z', PUSH, 'Z');
	PDATransition t23(&Q, &Q, 'i', 9, PUSH, 'Z');


	std::set<char> alphabet = {'e', 'i'};
	std::set<char> stackAlphabet = {'Z'};
	PDAFinal pdaType = STACK;


	PDA pda(alphabet, stackAlphabet, pdaType);

	pda.addState(Q, true);

	pda.addTransition(t21);
	pda.addTransition(t22);
	pda.addTransition(t23);


	CHECK(pda.process("") == true);
	CHECK(pda.process("e") == false);
	CHECK(pda.process("i") == false);
	CHECK(pda.process("ie") == true);
	CHECK(pda.process("ii") == false);
	CHECK(pda.process("iie") == false);
	CHECK(pda.process("iiee") == true);
	CHECK(pda.process("iieiieeiee") == true);
	CHECK(pda.process("iieiieeie") == false);
}

TEST_CASE("CFGPDA 1","[PDA]"){
    const std::set<char> terminals = {'a', 'b'};
    const std::set<char> variables = {'A', 'B', 'C'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> productions = {
                                                        {'A', "ABB"},
                                                        {'A', ""},
                                                        {'B', "a"}
                                                        };
    CFG c0(terminals, variables, productions, 'A');
    PDA pda(c0);

    CHECK(pda.process("") == true);
    CHECK(pda.process("a") == false);
    CHECK(pda.process("aa") == true);
    CHECK(pda.process("aaa") == false);
    CHECK(pda.process("aaaa") == true);
}

TEST_CASE("CFGPDA 2","[PDA]"){
    const std::set<char> terminals = {'x', 'y'};
    const std::set<char> variables = {'P', 'Q', 'R'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> productions = {
                                                        {'P', "Q"},
                                                        {'P', "R"},
                                                        {'R', "xy"},
                                                        {'R', "xQy"},
                                                        {'R', "xyy"},
                                                        {'R', "xRyy"},
                                                        {'Q', "xy"},
                                                        {'Q', "xQy"}
                                                        };
    CFG c0(terminals, variables, productions, 'P');
    PDA pda(c0);

    CHECK(pda.process("xy") == true);
    CHECK(pda.process("xxyy") == true);
    CHECK(pda.process("xxyyyy") == true);
    CHECK(pda.process("xxxxy") == false);
    CHECK(pda.process("xyyy") == false);
    CHECK(pda.process("y") == false);
    CHECK(pda.process("x") == false);
}

TEST_CASE("CFGPDA 3","[PDA]"){
    const std::set<char> terminals = {'0', '1'};
    const std::set<char> variables = {'A'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> productions = {
                                                        {'A', "0A1"},
                                                        {'A', "0"},
                                                        {'A', "1"}
                                                        };
    CFG c0(terminals, variables, productions, 'A');
    PDA pda(c0);

    CHECK(pda.process("001") == true);
    CHECK(pda.process("01") == false);
    CHECK(pda.process("1") == true);
    CHECK(pda.process("011") == true);
    CHECK(pda.process("00111") == true);
    CHECK(pda.process("000111") == false);
}

TEST_CASE("CFGPDA 4","[PDA]"){
    const std::set<char> terminals = {'x', 'y'};
    const std::set<char> variables = {'S', 'Q', 'R'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> productions = {
                                                        {'S', "QR"},
                                                        {'Q', "xQy"},
                                                        {'Q', "xy"},
                                                        {'R', "xxRy"},
                                                        {'R', "xxy"}
                                                        };
    CFG c0(terminals, variables, productions, 'S');
    PDA pda(c0);

    CHECK(pda.process("xyxxy") == true);
    CHECK(pda.process("xxyyxxxxyy") == true);
    CHECK(pda.process("xy") == false);
    CHECK(pda.process("xyxy") == false);
}


TEST_CASE("CFGPDA 5","[PDA]"){
    const std::set<char> terminals = {'0', '1', '(', ')', '+', '*'};
    const std::set<char> variables = {'P'};

    const std::multimap<char, SymbolString> empty;
    const std::multimap<char, SymbolString> productions = {
                                                        {'P', ""},
                                                        {'P', "0"},
                                                        {'P', "1"},
                                                        {'P', "(P)"},
                                                        {'P', "P*"},
                                                        {'P', "P+P"},
                                                        {'P', "PP"}
                                                        };
    CFG c0(terminals, variables, productions, 'P');
    PDA pda(c0);

    CHECK(pda.process("0+1") == true);
    try{
    	REQUIRE_THROWS(pda.process("2+1"));
    	pda.process("2+1");
    } catch(std::runtime_error& e){
    	std::string error(e.what());
    	CHECK(error == "There is a symbol in the input string which is not in the PDA's alphabet");
    }
    CHECK(pda.process("(0+1)*") == true);
    //CHECK(pda.process("(0)+1+0*+(0+1)") == true);
    //CHECK(pda.process("00+11") == true);
    try{
		REQUIRE_THROWS(pda.process("[0]"));
		pda.process("[0]");
	} catch(std::runtime_error& e){
		std::string error(e.what());
		CHECK(error == "There is a symbol in the input string which is not in the PDA's alphabet");
	}
}




