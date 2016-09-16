/*
 * DFAMinimizer.h
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#ifndef DFAMINIMIZER_H_
#define DFAMINIMIZER_H_

#include <vector>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
#include <string.h>

using namespace std;

class DFAMinimizer {
public:
	DFAMinimizer(vector<vector<int> > transitionTable, string inputString,
			vector<pair<int, int> > groups);
	vector<vector<int> > consturctTransitionTable(
			vector<vector<int> > transitionTable);
	map<int, int> getFinal();
	int getStartState();
private:
	void minimize(vector<vector<int> > transitionTable, string input,
			vector<pair<int, int> > vec);
	static bool compare1(const pair<int, int>& i, const pair<int, int>& j);
	static bool compare2(const pair<int, int>& i, const pair<int, int>& j);
	void getclasses(vector<pair<int, int> > v);
	void compare(int in, vector<pair<int, int> > v,
			vector<vector<int> > transitionTable);
	void mapping();
	queue<vector<pair<int, int> > > split;
	map<int, int> final;
	map<int, int> join;
	int startState;
};

#endif /* DFAMINIMIZER_H_ */
