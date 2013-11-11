/*
 * CFG.h
 *
 *  Created on: 8 nov. 2013
 *      Author: Ruben
 */

#ifndef CFG_H_
#define CFG_H_

#include <vector>
#include <map>
#include <string>

/**
 * @brief Class representing a sequence of terminals and variables
 */
class symbolString{
public:
	 /**
	 * @brief Constructor
	 *
	 * @param string A string containing variables and symbols
	 */
	symbolString(std::string string);

	 /**
	 * @brief Get the string
	 *
	 * @param string A string containing variables and symbols
	 *
	 * @return A string representing the symbolString
	 */
	std::string get();

	 /**
	 * @brief Get a symbol in the string at a certain point
	 *
	 * @param index The index of the symbol you want
	 *
	 * @return The symbol you wanted
	 */
	char get(int index);

	 /**
	 * @brief Remove a symbol in the string at a certain point
	 *
	 * @param index The index of the symbol you want removed
	 *
	 * @return A bool telling if the operation was successfull
	 */
	bool remove(int index);

	 /**
	 * @brief Append two symbolStrings
	 *
	 * @param second The second symbolString
	 *
	 * @return This symbolString object
	 */
	symbolString operator+(symbolString second);

	 /**
	 * @brief Get the length of the symbolString
	 *
	 * @return An integer
	 */
	int size();
private:
	std::string symbols;
};

/**
 * @brief Class representing a CFG
 */
class CFG {
public:
	 /**
	 * @brief Constructor
	 *
	 * @param CFGTerminals A vector containing the terminals of the CFG
	 * @param CFGVariables A vector containing the variables of the CFG
	 * @param CFGTransitions A multimap that maps a symbol to an symbolString
	 * @param CFGStartsymbol The startsymbol for the CFG
	 */
	CFG(std::vector<char> CFGTerminals, std::vector<char> CFGVariables,std::multimap<char, symbolString> CFGTransitions, char CFGStartsymbol);
	virtual ~CFG();
private:
	std::multimap<char, symbolString> transitions;
	std::vector<char> terminals;
	std::vector<char> variables;

	char startSymbol;
};

#endif /* CFG_H_ */
