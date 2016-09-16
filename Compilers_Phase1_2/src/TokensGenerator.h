/*
 * TokensGenerator.h
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#ifndef TOKENSGENERATOR_H_
#define TOKENSGENERATOR_H_

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include "Helper.h"

using namespace std;

class TokensGenerator {
public:
	TokensGenerator(vector<vector<int> > table, map<int, int> isFinal,
			int startState, map<char, int> input, map<int, string> classes);
	vector<string> getTokens(string inputFile, string outputFile);
private:
	vector<vector<int> > table;
	map<int, int> isFinal;
	int startState;
	map<char, int> input;
	map<int, string> classes;
};

#endif /* TOKENSGENERATOR_H_ */
