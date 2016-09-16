/*
 * Helper.cpp
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#include "Helper.h"

Helper::Helper() {
	// TODO Auto-generated constructor stub

}

bool Helper::isWhitespaceChar(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r\n';
}

int Helper::deleteWhiteSpaces(string s, int i) {
	while (i < (int) s.size() && isWhitespaceChar(s[i]))
		i++;
	return i;
}

string Helper::constructInputString(map<char, int> input) {
	string s = "";
	map<char, int>::iterator it;
	map<int, char> tmp;
	for (it = input.begin(); it != input.end(); it++)
		tmp[it->second] = it->first;
	for (int i = 0; i < (int) input.size(); i++)
		s += tmp[i];
	return s;
}

