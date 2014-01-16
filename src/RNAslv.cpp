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
 * Last modified: 16 January 2014.
 * By: Stijn Wouters.
 */
#include "RNAVisualizer.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cout << "Usage: " << argv[0] << " <rnastring> <stemsize> <beginloop> <endloop>" << std::endl;
    } else {
        std::string rnaseq = argv[1];
        int stemsize = std::atoi(argv[2]);
        int beginloop = std::atoi(argv[3]);
        int endloop = std::atoi(argv[4]);
        //HOTFIX converting stem indices to loop indices
        beginloop += stemsize;
        endloop -= (stemsize - 1);
        RNAVisualizer rna_visualizer;
        std::cout << stemsize << beginloop << endloop << std::endl;

        rna_visualizer.visualize(rnaseq, stemsize, beginloop, endloop);
    } // end if-else
    return 0;
}
