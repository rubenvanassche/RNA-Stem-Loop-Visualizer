/* RNAVisualizer
 * Visualize the stem loop.
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

#ifndef RNAVisualizer_H_
#define RNAVisualizer_H_

#include <string>
#include "SFML/Graphics.hpp"

class RNAVisualizer {
public:
    RNAVisualizer();

    void visualize(
            const std::string& sequence,
            const unsigned int& stemsize,
            const unsigned int& loopstart,
            const unsigned int& loopend
            );

private:
    unsigned int fRadius;
    sf::CircleShape fAdenine;
    sf::CircleShape fCytosine;
    sf::CircleShape fGuanine;
    sf::CircleShape fUracil;

    sf::RenderWindow fWindow;
    sf::View fStandard;

    void event();
    void draw(const char& type, const int& x, const int& y);
};

#endif /* RNAVisualizer_H_ */
