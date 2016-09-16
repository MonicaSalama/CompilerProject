/*
 * DFA.h
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#ifndef DFA_H_
#define DFA_H_

#include "DFAState.h"
#include <vector>
#include <stack>
#include <set>
#include <string>
#include <algorithm>
#include <queue>
using namespace std;

class DFA {
public:
	DFA(NFAState* s0, string input);
	vector<vector<int> > getTransitionTable();
	vector<pair<int, int> > getGroups();

private:
	void DFAGenerator(NFAState* s0, string input);
	void initialDTran(NFAState* s0, int input);
	void epsilonClosure(DFAState* d);
	vector<NFAState*> move(DFAState* d, char a);
	void setFinal(DFAState* d);
	bool notExist(NFAState* t, DFAState* d);
	static bool cmp(NFAState* a, NFAState* b);
	bool checkEquivelent(DFAState* u);
	bool same(vector<NFAState*> v1, vector<NFAState*> v2);
	int countId;
	vector<DFAState*> DFAStates;
	queue<DFAState*> incompleteDFAstates;
	vector<vector<int> > transitionTable;
	vector<pair<int, int> > groups;
};

#endif /* DFA_H_ */
