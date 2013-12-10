/* runCYK
 * Simple program that reads an xml file and a string and verify whether 
 * this is accepted by the given CFG.
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
 * Last modified: 09 December 2013.
 * By: Stijn Wouters.
 */
#include "TinyXML/tinyxml.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <string> 
#include "CNF.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage:" << std::endl;
        std::cout << '\t' << argv[0] << " [XML-FILE] [STRING]" << std::endl;
    } else {
        try {
            // start to parse
            TiXmlDocument doc;
            if (!doc.LoadFile(argv[1]))
                throw std::runtime_error("XML file does not exist");

            TiXmlElement* root = doc.FirstChildElement();

            if (root == NULL) {
                doc.Clear();
                throw std::runtime_error("No root element");
            } // end if

            std::string rootname = root->Value();
            std::transform(rootname.begin(), rootname.end(), rootname.begin(), ::tolower);

            if ( !( (rootname == "cfg") || (rootname == "cnf") ) ) {
                doc.Clear();
                throw std::runtime_error("Not a CFG or CNF XML file.");
            } // end if

            // ok, now ACTUALLY parse the stuffs
            std::set<char> terminals;
            std::set<char> variables;
            std::multimap<char, SymbolString> productions;
            char start = 0;

            for (TiXmlElement* el = root->FirstChildElement(); el != NULL; el = el->NextSiblingElement()) {
                std::string elName = el->Value();
                std::transform(elName.begin(), elName.end(), elName.begin(), ::tolower);

                if (elName == "terminals") {
                    TiXmlNode* n = el->FirstChild();
                    TiXmlText* text = NULL;

                    if (n != NULL)
                        text = n->ToText();

                    if (text == NULL)
                        continue;

                    std::string terms = text->Value();
                    for (char t : terms) {
                        terminals.insert(t);
                    } // end for

                } else if (elName == "variables") {
                    TiXmlNode* n = el->FirstChild();
                    TiXmlText* text = NULL;

                    if (n != NULL)
                        text = n->ToText();

                    if (text == NULL)
                        continue;

                    std::string vars = text->Value();
                    for (char v : vars) {
                        variables.insert(v);
                    } // end for

                } else if (elName == "productions") {
                    for (TiXmlElement* rule = el->FirstChildElement(); rule != NULL; rule = rule->NextSiblingElement()) {
                        std::string ruleName = rule->Value();
                        std::transform(ruleName.begin(), ruleName.end(), ruleName.begin(), ::tolower);

                        if (ruleName == "rule") {
                            char head = 0;

                            for (TiXmlAttribute* attr = rule->FirstAttribute(); attr != NULL; attr = attr->Next()) {
                                std::string attrName = attr->Name();
                                std::transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);

                                if (attrName == "head") {
                                    std::string headStr = attr->Value();

                                    if (headStr.size() == 1)
                                        head = headStr.front();
                                } else {
                                    // ignore other information
                                } // end if-else
                            } // end for

                            TiXmlNode* n = rule->FirstChild();
                            TiXmlText* text = NULL;

                            if (n != NULL)
                                text = n->ToText();

                            std::string body;

                            if (text == NULL) {
                                body = "";
                            } else {
                                body = text->Value();
                            } // end if-else

                            productions.insert(std::pair<char, SymbolString>(head, body));
                        } else  {
                            // ignore other information
                        } // end if-else
                    } // end for
                } else if (elName == "start") {
                    TiXmlNode* n = el->FirstChild();
                    TiXmlText* text = NULL;

                    if (n != NULL)
                        text = n->ToText();

                    if (text == NULL)
                        continue;

                    std::string symbol = text->Value();

                    if (symbol.size() == 1)
                        start = symbol.front();
                } else {
                    // ignore node
                } // end if-else
            } // end for

            // now check the string
            std::string sequence = argv[2];
            CNF cnf(terminals, variables, productions, start);
            if (cnf.CYK(sequence)) {
                std::cout << "\033[1;32mYES!\033[0m" << std::endl;
            } else {
                std::cout << "\033[1;31mNO!\033[0m" << std::endl;
            } // end if-else

        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        } // end try-catch
    } // end if-else

    return 0;
}
