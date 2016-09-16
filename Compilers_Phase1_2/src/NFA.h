/*
 * NFA.h
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#ifndef NFA_H_
#define NFA_H_

#include <vector>
#include <map>
#include <string>
#include <stack>
#include "Helper.h"
#include "NFAState.h"

using namespace std;

class NFA {
public:
	NFA(map<string, string> definitionsMap, vector<string> keywordsPuncVector,
			vector<pair<string, string> > REVector);
	NFAState* getStartState();
	map<char, int> getInputMap();
	map<int, string> getClasses();
private:
	map<string, string> definitions;
	vector<string> keywordsPunc;
	vector<pair<string, string> > RE;
	map<char, int> input;
	map<int, string> classes;
	int countId;
	int input_count;
	pair<NFAState*, NFAState*> nfaFromRE(string re, int final);
	pair<NFAState*, NFAState*> evaluatePostfix(vector<string> postfix,
			int isFinal);
	vector<string> tokensToPostfix(vector<string> expression);
	pair<NFAState*, NFAState*> getNFAForChar(char c);
	pair<NFAState*, NFAState*> getNFAFromKeyword(string s, int isFinal);
	pair<NFAState*, NFAState*> unionNFAs(pair<NFAState*, NFAState*> first,
			pair<NFAState*, NFAState*> second);
	pair<NFAState*, NFAState*> concatenateNFAs(pair<NFAState*, NFAState*> first,
			pair<NFAState*, NFAState*> second);
	pair<NFAState*, NFAState*> zeroOrMore(pair<NFAState*, NFAState*> nfa);
	pair<NFAState*, NFAState*> oneOrMore(pair<NFAState*, NFAState*> nfa);
	bool isOperator(string s);
	bool higherPercedence(string s1, string s2);
	vector<string> expressionToTokens(string expression);
	void getTokens(string s, vector<string>& tokens);
	bool isSep(char c);
};

#endif /* NFA_H_ */
