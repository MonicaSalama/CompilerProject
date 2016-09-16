/*
 * ParsingTable.h
 *
 *  Created on: Apr 20, 2016
 *      Author: monica
 */

#ifndef PARSINGTABLE_H_
#define PARSINGTABLE_H_

#include <vector>
#include "CFG.h"
#include "PredictiveParsing.h"

using namespace std;
#define EMPTY -1
#define EPS -2
#define SYNCH -3

class ParsingTable {
public:
	ParsingTable(CFG* cfg);
	map<int, int> nonTermRow;
	map<string, int> termSCol;
	map<int, vector<int> > parsingTableMap;
	vector<vector<int> > parseTable;
	int epsilonID;
	void print();
private:
	CFG* cfg;
	vector<vector<int> > first;
	vector<set<int> > follow;
	vector<bool> visited;
	vector<bool> hasEps;
	vector<bool> done;
	map<int, vector<pair<int, int> > > mapInputProduction;
	map<vector<int>, int> mapProduction;
	map<int, int> termCol;
	map<int, int> colTerm;
	map<int, int> rowNonTerm;
	int productionNum;
	void getFirst();
	void getFirstTerminals();
	bool getFirstNonTerminals(int);
	void getFollow();
	bool helpFollow(int currentState, map<int, vector<int> > need, int left);
	void sub(int currentState, int left);
	void checkDone(int cur, map<int, vector<int> > need);
	void constructTable();
	void test();
};

#endif /* PARSINGTABLE_H_ */
