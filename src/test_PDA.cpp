/*
 * test_PDA.cpp
 *
 *  Created on: 25 nov. 2013
 *      Author: Ruben
 */
#include "Catch.h"
#include "PDA.h"

TEST_CASE("Add states and transitions", "[PDA]"){
	std::set<char> alphabet;
	std::set<char> stackAlphabet;
	char stackStartChar = 1;
	PDAFinal pdaType = STATE;

	alphabet.insert('0');
	alphabet.insert('1');
	stackAlphabet.insert('a');
	stackAlphabet.insert('b');

	PDA pda(alphabet, stackAlphabet, stackStartChar, pdaType);

}



