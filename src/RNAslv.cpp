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
#include "CFG.h"
#include <iostream>

int main(int argc, char* argv[]) {
	  const std::set<char> terminals = {'a', 'b'};
	    const std::set<char> variables = {'A', 'B'};
	    const std::multimap<char, SymbolString> all_nullable = {
	                                                        {'A', "AB"},
	                                                        {'A', ""},
	                                                        {'B', ""}
	                                                        };
	    const CFG c0(terminals, variables, all_nullable , 'A');
	    PDA pda(c0);
	    std::cout << pda << std::endl;
}
