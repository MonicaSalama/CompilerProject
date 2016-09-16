#ifndef PREDICTIVEPARSING_H_
#define PREDICTIVEPARSING_H_

#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <set>
#include "ParsingTable.h"

using namespace std;

class PredictiveParsing {
public:

	PredictiveParsing(map<int, int> row1, map<string, int> col1,
			map<int, vector<int> > productive1, map<int, string> getmap1,
			set<int> isterminal1, vector<vector<int> > parsingtable,
			int epsilonId);

	void parse(vector<string> v);

private:
	void print(vector<string> v1, int i, int error1);
	void printstack();
	vector<int> match(int nonterminal, string in,
			vector<vector<int> > parsingtable);
	map<int, int> row;
	map<string, int> col;
	map<int, vector<int> > productive;
	map<int, string> getmap;
	set<int> isterminal;
	stack<int> s;
	vector<int> vec;
	vector<vector<int> > parsingtable;
	int epsilonId;

};

#endif /* PREDICTIVEPARSING_H_ */
