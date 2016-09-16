/*
 * LexicalParser.cpp
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#include "LexicalParser.h"

using namespace std;

LexicalParser::LexicalParser(string fileName) {
	definitions.clear();
	keywordsPunc.clear();
	RE.clear();
	readFile(fileName);
}

void LexicalParser::readFile(string fileName) {
	string line;
	ifstream myfile(fileName.c_str());
	while (getline(myfile, line))
		parsing(line);
	myfile.close();
}

void LexicalParser::parsing(string line) {
	string temp;
	if (line[0] == '{' || line[0] == '[') {
		string t = "";
		for (int i = 1;
				i < (int) line.size() && !isMatchingBracket(line[0], line[i]);
				i++) {
			if (Helper::isWhitespaceChar(line[i])) {
				if (t != "") {
					keywordsPunc.push_back(t);
					t = "";
				}
			} else if (line[i] == '\\') {
				t += line[++i];
			} else
				t += line[i];
		}
		if (t != "")
			keywordsPunc.push_back(t);
	} else {
		int i = Helper::deleteWhiteSpaces(line, 0);
		while (i < (int) line.size()) {
			i = Helper::deleteWhiteSpaces(line, i);
			if (line[i] == ':') {
				i++;
				i = Helper::deleteWhiteSpaces(line, i);
				RE.push_back(make_pair(temp, line.substr(i, line.size() - i)));
				break;
			} else if (line[i] != '=') {
				temp += line[i];
				i++;
			} else {
				string temp2 = "";
				i++;
				i = Helper::deleteWhiteSpaces(line, i);
				while (i < (int) line.size()) {
					if ((i + 2) < (int) line.size()
							&& nextCharRange(line, i + 1)) {
						char first = line[i];
						i++;
						i = Helper::deleteWhiteSpaces(line, i);
						i++;
						i = Helper::deleteWhiteSpaces(line, i);
						temp2.append(getstringForRange(first, line[i]));
						i++;
					} else if (!Helper::isWhitespaceChar(line[i])) {
						temp2 += line[i];
						i++;
					} else {
						i++;
					}
				}
				definitions[temp] = temp2;
				break;
			}
		}
	}
}

bool LexicalParser::isMatchingBracket(char c1, char c2) {
	return (c1 == '[' && c2 == ']') || (c1 == '{' && c2 == '}');
}

bool LexicalParser::nextCharRange(string s, int i) {
	return s[Helper::deleteWhiteSpaces(s, i)] == '-';
}

string LexicalParser::getstringForRange(char start, char end) {
	string v = "";
	for (char a = start; a < end; a++) {
		v += a;
		v += '|';
	}
	v += end;
	return v;
}

map<string, string> LexicalParser::getDefinitions() {
	return definitions;
}
vector<string> LexicalParser::getKeywordsPunc() {
	return keywordsPunc;
}
vector<pair<string, string> > LexicalParser::getRE() {
	return RE;
}

