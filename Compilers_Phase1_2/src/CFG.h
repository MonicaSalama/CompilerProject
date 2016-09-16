/*
 * CFG.h
 *
 *  Created on: Apr 20, 2016
 *      Author: mostafa
 */

#ifndef CFG_H_
#define CFG_H_

#include <set>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include "Helper.h"
#include "Trie.h"

using namespace std;

class CFG {
public:
	CFG(string fileName);
	set<int> getTerminals();
	vector<pair <int, vector<vector<int> > > > getNonTerminals();
	map<int, string> getMap();
	bool isTerminal(int token);
	int getNumberOfElements();
	int getNonTerminalPos(int id);
	int getEpsId();
	map<int , vector <pair<int , pair<int, int> > > > getPosRightHandSide();
private:
	int count;
	set<int> terminals;
	vector<pair <int, vector<vector<int> > > > nonTerminals;
	map<int , vector <pair<int , pair<int, int> > > > posRHSMap;
	map<int, string> em;
	map<string, int> me;
	map<int, int> idToPos;
	void parse(string fileName);
	vector<string> getTokens(string s);
	int getTokenId(string token);
	void parseProduction(string production);
	bool isTerminalToken(string s);
	void addNonTerminal(int lhs, vector<vector<int> > rhs);
	void addToPosRHSMap(int id, int x, int y, int z);
	void eliminateLR(int lhs, vector<vector<int> > rhs);
	void eliminateLF(int lhs, vector<vector<int> > rhs);
	vector<int> getLongestCommonPrefix(Trie* node);
	string getDashedName(string s);
	bool startsWith(vector<int> v, vector<int> prefix);
};

#endif /* CFG_H_ */
