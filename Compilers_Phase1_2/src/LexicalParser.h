/*
 * LexicalParser.h
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#ifndef LEXICALPARSER_H_
#define LEXICALPARSER_H_

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Helper.h"

using namespace std;

class LexicalParser {
public:
	LexicalParser(string fileName);
	map<string, string> getDefinitions();
	vector<string> getKeywordsPunc();
	vector<pair<string, string> > getRE();

private:
	map<string, string> definitions;
	vector<string> keywordsPunc;
	vector<pair<string, string> > RE;
	void readFile(string fileName);
	void parsing(string line);
	bool isMatchingBracket(char c1, char c2);
	bool nextCharRange(string s, int i);
	string getstringForRange(char start, char end);
};

#endif /* LEXICALPARSER_H_ */
