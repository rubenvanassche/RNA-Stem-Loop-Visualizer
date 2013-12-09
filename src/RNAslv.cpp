/* RNA Stem Loop Visualizer
 * The main program.
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
 * Last modified: 10 November 2013
 * By: Stijn Wouters
 */
#include "PDA.h"
#include <iostream>

int main(int argc, char* argv[]) {
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


	if(pda.process("ie") == true){
		std::cout << "GELUKT!!" <<std::endl;
	}
}
