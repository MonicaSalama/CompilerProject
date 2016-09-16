/*
 * CFG.cpp
 *
 *  Created on: Apr 20, 2016
 *      Author: mostafa
 */

#include "CFG.h"

using namespace std;

CFG::CFG(string fileName) {
	count = 0;
	terminals.clear();
	nonTerminals.clear();
	em.clear();
	me.clear();
	idToPos.clear();
	posRHSMap.clear();
	parse(fileName);
	if (me.find("$") != me.end())
		terminals.insert(getTokenId("$"));
}

set<int> CFG::getTerminals() {
	return terminals;
}

vector<pair<int, vector<vector<int> > > > CFG::getNonTerminals() {
	return nonTerminals;
}

map<int, string> CFG::getMap() {
	return em;
}

bool CFG::isTerminal(int token) {
	bool isTer = terminals.find(token) != terminals.end();
	return isTer;
}

int CFG::getNumberOfElements() {
	return terminals.size() + nonTerminals.size();
}

map<int, vector<pair<int, pair<int, int> > > > CFG::getPosRightHandSide() {
	return posRHSMap;
}

int CFG::getNonTerminalPos(int id) {
	return idToPos[id];
}

int CFG::getEpsId() {
	return me["$"];
}

int CFG::getTokenId(string token) {
	if (me.find(token) == me.end()) {
		me[token] = count++;
		em[me[token]] = token;
	}
	return me[token];
}

vector<string> CFG::getTokens(string s) {
	vector<string> tokens;
	string token = "";
	int i = Helper::deleteWhiteSpaces(s, 0);
	while (i < (int) s.length()) {
		if (s[i] == ' ') {
			if (token != "") {
				tokens.push_back(token);
				token = "";
			}
			i = Helper::deleteWhiteSpaces(s, i);
		} else {
			token = token + s[i++];
		}
	}
	if (token != "") {
		tokens.push_back(token);
	}
	return tokens;
}

bool CFG::isTerminalToken(string s) {
	return s.size() > 2 && s[0] == '\'' && s[s.size() - 1] == '\'';
}

void print(Trie* node, int d) {
	map<int, Trie*>::iterator iter;
	for (iter = node->m.begin(); iter != node->m.end(); iter++) {
		int x = iter->first;
		Trie* nxt = iter->second;
		cout << x << " Depth: " << d << endl;
		print(nxt, d + 1);
	}
}

vector<int> CFG::getLongestCommonPrefix(Trie* node) {
	vector<int> v;
	v.clear();
	if (!node->getCommon())
		return v;
	map<int, Trie*>::iterator iter;
	for (iter = node->m.begin(); iter != node->m.end(); iter++) {
		vector<int> tmp = getLongestCommonPrefix(iter->second);
		if (!tmp.empty()) {
			if (tmp.size() == 1 && tmp[0] == -1)
				tmp[0] = iter->first;
			else
				tmp.insert(tmp.begin(), iter->first);
		}
		if (tmp.size() > v.size())
			v = tmp;
	}
	if (v.empty() && !node->isRoot)
		v.push_back(-1);
	return v;
}

void CFG::addToPosRHSMap(int id, int x, int y, int z) {
	if (posRHSMap.find(id) == posRHSMap.end()) {
		vector<pair<int, pair<int, int> > > v;
		posRHSMap[id] = v;
	}
	posRHSMap[id].push_back(make_pair(x, make_pair(y, z)));
}

void CFG::addNonTerminal(int lhs, vector<vector<int> > rhs) {
	nonTerminals.push_back(make_pair(lhs, rhs));
	idToPos[lhs] = nonTerminals.size() - 1;
	for (int i = 0; i < (int) rhs.size(); i++) {
		for (int j = 0; j < (int) rhs[i].size(); j++) {
			addToPosRHSMap(rhs[i][j], idToPos[lhs], i, j);
		}
	}
}

string CFG::getDashedName(string s) {
	while (me.find(s) != me.end())
		s = s + "\\";
	return s;
}

bool CFG::startsWith(vector<int> v, vector<int> prefix) {
	if (v.size() < prefix.size())
		return false;
	for (int i = 0; i < (int) prefix.size(); i++)
		if (v[i] != prefix[i])
			return false;
	return true;
}

void CFG::eliminateLF(int lhs, vector<vector<int> > rhs) {
	Trie* root = new Trie();
	root->isRoot = true;
	for (int i = 0; i < (int) rhs.size(); i++)
		root->insert(rhs[i]);
	vector<int> lcp = getLongestCommonPrefix(root);
	if (lcp.empty()) {
		return addNonTerminal(lhs, rhs);
	}
	string lhsDash = getDashedName(em[lhs]);
	int lhsNew = getTokenId(lhsDash);
	vector<vector<int> > rhsDash;
	for (int i = 0; i < (int) rhs.size(); i++) {
		if (startsWith(rhs[i], lcp)) {
			vector<int> postfix;
			for (int j = lcp.size(); j < (int) rhs[i].size(); j++)
				postfix.push_back(rhs[i][j]);
			if (postfix.empty())
				postfix.push_back(getTokenId("$"));
			rhsDash.push_back(postfix);
			rhs.erase(rhs.begin() + i);
			i--;
		}
	}
	lcp.push_back(lhsNew);
	rhs.push_back(lcp);
	addNonTerminal(lhs, rhs);
	addNonTerminal(lhsNew, rhsDash);
}

void CFG::eliminateLR(int lhs, vector<vector<int> > rhs) {
	vector<vector<int> > nrhs;
	bool v[rhs.size()];
	memset(v, false, sizeof(v));
	for (int i = 0; i < (int) nonTerminals.size(); i++) {
		int j = nonTerminals[i].first;
		vector<vector<int> > jrhs = nonTerminals[i].second;
		for (int ii = 0; ii < (int) rhs.size(); ii++) {
			if (rhs[ii][0] == j) {
				vector<int> x = rhs[ii];
				int jpos = getNonTerminalPos(j);
				for (int jj = 0; jj < (int) nonTerminals[jpos].second.size();
						jj++) {
					vector<int> y = nonTerminals[jpos].second[jj];
					vector<int> merged;
					merged.reserve(y.size() + x.size());
					merged.insert(merged.end(), y.begin(), y.end());
					merged.insert(merged.end(), x.begin() + 1, x.end());
					nrhs.push_back(merged);
				}
				v[ii] = true;
			}
		}
	}
	for (int i = 0; i < (int) rhs.size(); i++) {
		if (!v[i])
			nrhs.push_back(rhs[i]);
	}
	bool hasLR = false;
	for (int i = 0; i < (int) nrhs.size() && !hasLR; i++)
		hasLR |= nrhs[i][0] == lhs;
	if (!hasLR) {
		eliminateLF(lhs, nrhs);
		return;
	}
	string lhsDash = getDashedName(em[lhs]);
	int lhsNew = getTokenId(lhsDash);
	vector<vector<int> > rhsDash;
	for (int i = 0; i < (int) nrhs.size(); i++) {
		if (nrhs[i][0] == lhs) {
			vector<int> tmp;
			tmp.reserve(nrhs[i].size());
			tmp.insert(tmp.end(), nrhs[i].begin() + 1, nrhs[i].end());
			tmp.push_back(lhsNew);
			rhsDash.push_back(tmp);
			nrhs.erase(nrhs.begin() + i);
			i--;
		} else {
			nrhs[i].push_back(lhsNew);
		}
	}
	vector<int> tmp;
	tmp.push_back(getTokenId("$"));
	rhsDash.push_back(tmp);
	eliminateLF(lhs, nrhs);
	eliminateLF(lhsNew, rhsDash);
}

void CFG::parseProduction(string production) {
	vector<string> tokens = getTokens(production);
	if (!(tokens.size() > 1 && tokens[1] == "=")) {
		return;
	}
	int lhs = getTokenId(tokens[0]);
	vector<vector<int> > rhs;
	vector<int> rhs_element;
	for (int i = 2; i < (int) tokens.size(); i++) {
		if (tokens[i] == "|") {
			if (!rhs_element.empty())
				rhs.push_back(rhs_element);
			rhs_element.clear();
		} else {
			if (isTerminalToken(tokens[i])) {
				tokens[i] = tokens[i].substr(1, tokens[i].length() - 2);
				if (tokens[i].size() > 1 && tokens[i][0] == '\\') {
					tokens[i] =
							tokens[i] == "\\L" ?
									"$" :
									tokens[i].substr(1, tokens[i].length() - 1);
				}
				terminals.insert(getTokenId(tokens[i]));
			}
			rhs_element.push_back(getTokenId(tokens[i]));
		}
	}
	if (!rhs_element.empty())
		rhs.push_back(rhs_element);
	eliminateLR(lhs, rhs);
}

void CFG::parse(string fileName) {
	ifstream inputFile(fileName.c_str());
	string line;
	string production = "";
	while (getline(inputFile, line)) {
		int i = Helper::deleteWhiteSpaces(line, 0);
		if (i == (int) line.size())
			continue;
		if (line[i] == '#') {
			parseProduction(production);
			i = Helper::deleteWhiteSpaces(line, i + 1);
			production = line.substr(i, line.size() - i + 1);
		} else {
			production = production + " " + line.substr(i, line.size() - i + 1);
		}
	}
	parseProduction(production);
}
