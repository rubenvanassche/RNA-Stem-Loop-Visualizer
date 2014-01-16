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
#include "RNAVisualizer.h"

RNAVisualizer::RNAVisualizer()
    : fRadius(24), fAdenine(fRadius, 5), fCytosine(fRadius, 5),
    fGuanine(fRadius, 5), fUracil(fRadius, 5), fWindow(
            sf::VideoMode(1024, 640),
            "RNA Stem Loop Visualizer",
            sf::Style::Close | sf::Style::Titlebar
            ), fStandard(fWindow.getView())  {
        // set colors
        fAdenine.setFillColor( sf::Color(0, 0, 255) );
        fCytosine.setFillColor( sf::Color(255, 0, 0) );
        fGuanine.setFillColor( sf::Color(0, 255, 0) );
        fUracil.setFillColor( sf::Color(255, 153, 0) );

        // Set framerate to reduce the CPU load, also hide the mouse, since it's
        // keyboard controlled (VIm like)
        fWindow.setFramerateLimit(60);
        fWindow.setMouseCursorVisible(false);
    }

void RNAVisualizer::event() {
    sf::Event event;

    while ( fWindow.pollEvent(event) ) {
        if (event.type == sf::Event::Closed) {
            // close fWindow
            fWindow.close();
        } else if (event.type == sf::Event::MouseWheelMoved) {
            // zoom in or out
            fStandard.zoom(1.f+event.mouseWheel.delta*0.1f);
        } else if (event.type == sf::Event::KeyPressed) {
            // move operations
            switch (event.key.code) {
            case sf::Keyboard::R:
                fStandard.rotate(4.f);
                break;
            case sf::Keyboard::T:
                fStandard.rotate(-4.f);
                break;
            case sf::Keyboard::Left:
            case sf::Keyboard::H:
                fStandard.move(4.f, 0.f);
                break;
            case sf::Keyboard::Right:
            case sf::Keyboard::L:
                fStandard.move(-4.f, 0.f);
                break;
            case sf::Keyboard::Up:
            case sf::Keyboard::K:
                fStandard.move(0.f, 4.f);
                break;
            case sf::Keyboard::Down:
            case sf::Keyboard::J:
                fStandard.move(0.f, -4.f);
                break;
            case sf::Keyboard::Q:
                fWindow.close();
                break;
            default:
                // do nothing
                break;
            } // end case
        } else {
            // do nothing
        } // end if-else
    } // end while

    return;
}

void RNAVisualizer::draw(const char& type, const int& x, const int& y) {
    switch(type) {
    case 'A':
        fAdenine.setPosition(x, y);
        fWindow.draw(fAdenine);
        break;
    case 'U':
        fUracil.setPosition(x, y);
        fWindow.draw(fUracil);
        break;
    case 'G':
        fGuanine.setPosition(x, y);
        fWindow.draw(fGuanine);
        break;
    case 'C':
        fCytosine.setPosition(x, y);
        fWindow.draw(fCytosine);
        break;
    } // end switch

    return;
}

void RNAVisualizer::visualize(
        const std::string& sequence,
        const unsigned int& stemsize,
        const unsigned int& loopstart,
        const unsigned int& loopend
        ) {
    // as long as the window is open, display the view
    while( fWindow.isOpen() ) {
        // first handles events that has to do witht the window
        this->event();

        // clear-draw-display-cycle
        fWindow.clear();

        // use the standard view to display the RNA
        fWindow.setView(fStandard);

        int y = 0;
        for (unsigned int index = 0; index < sequence.size(); ++index) {
            if (index < loopstart) {
                this->draw(sequence.at(index), 2 * fRadius, y);

                // add a binding
                sf::Vertex binding[4] = {
                    sf::Vertex( sf::Vector2f(4 * fRadius, y + (2 * fRadius / 3) ) ),
                    sf::Vertex( sf::Vector2f(6 * fRadius, y + (2 * fRadius / 3) ) ),
                    sf::Vertex( sf::Vector2f(4 * fRadius, y + (4 * fRadius / 3) ) ),
                    sf::Vertex( sf::Vector2f(6 * fRadius, y + (4 * fRadius / 3) ) ),
                };

                fWindow.draw(binding, 4, sf::Lines);

                y += 2 * fRadius;
            } else if (index <= loopend) {
                if ( (loopend - loopstart) / 2 == (index - loopstart) ) {
                    this->draw(sequence.at(index), 4 * fRadius, y);

                    y -= 2 * fRadius;
                } else if ( (index - loopstart) < ( (loopend - loopstart) / 2) ) {
                    this->draw(sequence.at(index), 0, y);

                    y += 2 * fRadius;
                } else {
                    this->draw(sequence.at(index), 8 * fRadius, y);

                    y -= 2 * fRadius;
                } // end if-else
            } else {
                this->draw(sequence.at(index), 6 * fRadius, y);

                y -= 2 * fRadius;
            } // end if-else
        } // end for

        fWindow.display();
    } // end while

    return;
}
