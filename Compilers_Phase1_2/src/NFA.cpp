/*
 * NFA.cpp
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#include "NFA.h"

NFA::NFA(map<string, string> definitionsMap, vector<string> keywordsPuncVector,
		vector<pair<string, string> > REVector) {
	definitions = definitionsMap;
	keywordsPunc = keywordsPuncVector;
	RE = REVector;
	input.clear();
	classes.clear();
	countId = 0;
	input_count = 0;
}

NFAState* NFA::getStartState() {
	map<int, pair<NFAState*, NFAState*> > nfas;
	int pri = 1;
	for (int i = 0; i < (int) keywordsPunc.size(); i++) {
		classes[pri] = keywordsPunc[i];
		nfas[pri] = getNFAFromKeyword(keywordsPunc[i], pri);
		pri++;
	}
	for (int i = 0; i < (int) RE.size(); i++) {
		classes[pri] = RE[i].first;
		nfas[pri] = nfaFromRE(RE[i].second, pri);
		pri++;
	}
	NFAState* startState = new NFAState(countId++);
	for (int i = 1; i < pri; i++) {
		startState->addToMap('$', nfas[i].first);
	}
	return startState;
}

pair<NFAState*, NFAState*> NFA::getNFAForChar(char c) {
	if (c != '$' && input.find(c) == input.end()) {
		input[c] = input_count++;
	}
	NFAState* x = new NFAState(countId++);
	NFAState* y = new NFAState(countId++);
	x->addToMap(c, y);
	return make_pair(x, y);
}

pair<NFAState*, NFAState*> NFA::getNFAFromKeyword(string s, int isFinal) {
	NFAState* x = new NFAState(countId++);
	NFAState* prev = x;
	for (int i = 0; i < (int) s.size(); i++) {
		NFAState* tmp = new NFAState(countId++);
		if (input.find(s[i]) == input.end()) {
			input[s[i]] = input_count++;
		}
		prev->addToMap(s[i], tmp);
		prev = tmp;
	}
	prev->isFinal = isFinal;
	return make_pair(x, prev);
}

pair<NFAState*, NFAState*> NFA::unionNFAs(pair<NFAState*, NFAState*> first,
		pair<NFAState*, NFAState*> second) {
	NFAState* x = new NFAState(countId++);
	NFAState* y = new NFAState(countId++);
	x->addToMap('$', first.first);
	x->addToMap('$', second.first);
	first.second->addToMap('$', y);
	second.second->addToMap('$', y);
	return make_pair(x, y);
}

pair<NFAState*, NFAState*> NFA::concatenateNFAs(
		pair<NFAState*, NFAState*> first, pair<NFAState*, NFAState*> second) {
	first.second->addToMap('$', second.first);
	return make_pair(first.first, second.second);
}

pair<NFAState*, NFAState*> NFA::zeroOrMore(pair<NFAState*, NFAState*> nfa) {
	NFAState* x = new NFAState(countId++);
	NFAState* y = new NFAState(countId++);
	x->addToMap('$', y);
	x->addToMap('$', nfa.first);
	nfa.second->addToMap('$', nfa.first);
	nfa.second->addToMap('$', y);
	return make_pair(x, y);
}

pair<NFAState*, NFAState*> NFA::oneOrMore(pair<NFAState*, NFAState*> nfa) {
	return concatenateNFAs(nfa, zeroOrMore(nfa));
}

vector<string> NFA::tokensToPostfix(vector<string> expression) {
	vector<string> postfix;
	stack<string> s;
	for (int i = 0; i < (int) expression.size(); i++) {
		if (expression[i] == ")") {
			while (!s.empty()) {
				string top = s.top();
				s.pop();
				if (top == "(") {
					break;
				}
				postfix.push_back(top);
			}
		} else if (isOperator(expression[i])) {
			if (!s.empty()) {
				string top = s.top();
				if (top == "(" || higherPercedence(expression[i], top)) {
					s.push(expression[i]);
				} else {
					s.pop();
					postfix.push_back(top);
					i--;
				}
			} else {
				s.push(expression[i]);
			}
		} else {
			postfix.push_back(expression[i]);
		}
	}
	while (!s.empty()) {
		string top = s.top();
		s.pop();
		postfix.push_back(top);
	}
	return postfix;
}

pair<NFAState*, NFAState*> NFA::nfaFromRE(string re, int final) {
	return evaluatePostfix(tokensToPostfix(expressionToTokens(re)), final);
}

pair<NFAState*, NFAState*> NFA::evaluatePostfix(vector<string> postfix,
		int isFinal) {
	stack<pair<NFAState*, NFAState*> > s;
	for (int i = 0; i < (int) postfix.size(); i++) {
		if (postfix[i] == "+") {
			pair<NFAState*, NFAState*> top = s.top();
			s.pop();
			s.push(oneOrMore(top));
		} else if (postfix[i] == "*") {
			pair<NFAState*, NFAState*> top = s.top();
			s.pop();
			s.push(zeroOrMore(top));
		} else if (postfix[i] == "|") {
			pair<NFAState*, NFAState*> a = s.top();
			s.pop();
			pair<NFAState*, NFAState*> b = s.top();
			s.pop();
			s.push(unionNFAs(a, b));
		} else if (postfix[i] == "@") {
			pair<NFAState*, NFAState*> a = s.top();
			s.pop();
			pair<NFAState*, NFAState*> b = s.top();
			s.pop();
			s.push(concatenateNFAs(b, a));
		} else {
			vector<char> v;
			if (definitions.find(postfix[i]) != definitions.end()) {
				s.push(nfaFromRE(definitions[postfix[i]], 0));
			} else {

				if (postfix[i][0] == '\\')
					s.push(getNFAForChar(postfix[i][1]));
				else
					s.push(getNFAForChar(postfix[i][0]));

			}
		}
	}
	pair<NFAState*, NFAState*> nfa = s.top();
	nfa.second->isFinal = isFinal;
	return nfa;
}

void NFA::getTokens(string s, vector<string>& tokens) {
	if (definitions.find(s) != definitions.end()) {
		tokens.push_back(s);
		return;
	}
	for (int i = 0; i < (int) s.size(); i++) {
		string tmp = "";
		tmp += s[i];
		tokens.push_back(tmp);
	}
}

bool NFA::isSep(char c) {
	return c == '|' || c == '+' || c == '*' || c == '(' || c == ')';
}

vector<string> NFA::expressionToTokens(string expression) {
	vector<string> tokens;
	string tmp = "";
	int i = 0;
	while (i < (int) expression.size()) {
		if (expression[i] == '\\') {
			if (tmp != "") {
				tokens.push_back(tmp);
			}
			if (expression[i + 1] == 'L')
				tmp = "$";
			else
				tmp = "\\";
			tmp += expression[i + 1];
			tokens.push_back(tmp);
			tmp = "";
			i += 2;
		} else if (Helper::isWhitespaceChar(expression[i])) {
			if (tmp != "") {
				getTokens(tmp, tokens);
				tmp = "";
			}
			i = Helper::deleteWhiteSpaces(expression, i);
		} else if (!isSep(expression[i])) {
			tmp += expression[i];
			i++;
		} else if (isSep(expression[i])) {
			if (tmp != "") {
				getTokens(tmp, tokens);
			}
			tmp = expression[i];
			tokens.push_back(tmp);
			tmp = "";
			i++;
		}
	}
	if (tmp != "") {
		getTokens(tmp, tokens);
	}
	vector<string> finalTokens;
	i = 0;
	while (i < (int) tokens.size()) {
		if (tokens[i] == "|" || tokens[i] == "(") {
			finalTokens.push_back(tokens[i]);
			i++;
		} else {
			finalTokens.push_back(tokens[i]);
			if (i + 1 < (int) tokens.size()) {
				if (tokens[i + 1] != ")" && tokens[i + 1] != "*"
						&& tokens[i + 1] != "+" && tokens[i + 1] != "|") {
					finalTokens.push_back("@");
				}
			}
			i++;
		}
	}
	return finalTokens;
}

bool NFA::isOperator(string s) {
	if (s == "*" || s == "+" || s == "|" || s == "@" || s == "(")
		return true;
	return false;
}

bool NFA::higherPercedence(string s1, string s2) {
	if (s1 == "(")
		return true;
	if (s1 == "*" || s1 == "+")
		return (s2 != "*" && s2 != "+" && s2 != "(");
	if (s1 == "@")
		return s2 == "|";
	return false;
}

map<char, int> NFA::getInputMap() {
	return input;
}
map<int, string> NFA::getClasses() {
	return classes;
}

