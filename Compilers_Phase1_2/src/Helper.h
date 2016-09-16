/*
 * ParserHelper.h
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <string>
#include <map>

using namespace std;

class Helper {
public:
	Helper();
	static bool isWhitespaceChar(char c);
	static int deleteWhiteSpaces(string s, int i);
	static string constructInputString(map<char, int> input);
};

#endif /* HELPER_H_ */
