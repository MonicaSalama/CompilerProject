/*
 * DFA.cpp
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#include "DFA.h"

DFA::DFA(NFAState* s0, string input) {
	countId = 0;
	DFAGenerator(s0, input);
}

void DFA::DFAGenerator(NFAState* s0, string input) {
	initialDTran(s0, input.size());
	while (!incompleteDFAstates.empty()) {
		DFAState* d = incompleteDFAstates.front();
		incompleteDFAstates.pop();
		for (int i = 0; i < (int) input.size(); i++) {
			char a = input[i];
			vector<NFAState*> v = move(d, a);
			DFAState* u = new DFAState(0);
			u->addToStates(v);
			epsilonClosure(u);
			if (!checkEquivelent(u) && u->states.size() != 0) {
				u->setId(countId++);
				setFinal(u);
				vector<int> v;
				v.resize(input.size());
				transitionTable.push_back(v);
				incompleteDFAstates.push(u);
				DFAStates.push_back(u);
				groups.push_back(make_pair(u->id, u->isFinal));
			}
			if (u->states.size() != 0)
				transitionTable[d->id][i] = u->id;
			else
				transitionTable[d->id][i] = -1;
		}
	}
}

void DFA::initialDTran(NFAState* s0, int inputSize) {
	DFAState* initial = new DFAState(countId++);
	vector<int> v;
	v.resize(inputSize);
	transitionTable.push_back(v);
	initial->addToStates(s0);
	if (s0->edges.find('$') != s0->edges.end()) {
		initial->addToStates(s0->edges['$']);
		epsilonClosure(initial);
	}
	incompleteDFAstates.push(initial);
	DFAStates.push_back(initial);
	setFinal(initial);
	groups.push_back(make_pair(initial->id, initial->isFinal));
}

void DFA::epsilonClosure(DFAState* d) {
	stack<NFAState*> s;
	for (int i = 0; i < (int) d->states.size(); i++) {
		NFAState* x = d->states[i];
		s.push(x);
	}
	while (!s.empty()) {
		NFAState* t = s.top();
		s.pop();
		if (t->edges.find('$') == t->edges.end())
			continue;
		vector<NFAState*> v = t->edges['$'];
		for (int i = 0; i < (int) v.size(); i++) {
			if (notExist(v[i], d)) {
				NFAState* x = v[i];
				d->addToStates(x);
				s.push(x);
			}
		}
	}
}

bool DFA::cmp(NFAState* a, NFAState* b) {
	return a->id < b->id;
}

vector<NFAState*> DFA::move(DFAState* d, char a) {
	vector<NFAState*> transitions;
	for (int i = 0; i < (int) d->states.size(); i++) {
		if (d->states[i]->edges.find(a) == d->states[i]->edges.end())
			continue;
		vector<NFAState*> v = d->states[i]->edges[a];
		transitions.insert(transitions.end(), v.begin(), v.end());
	}
	set<NFAState*> s(transitions.begin(), transitions.end());
	transitions.assign(s.begin(), s.end());
	sort(transitions.begin(), transitions.end(), cmp);
	return transitions;
}

void DFA::setFinal(DFAState* d) {
	vector<int> v;
	for (int i = 0; i < (int) d->states.size(); i++)
		if (d->states[i]->isFinal != 0)
			v.push_back(d->states[i]->isFinal);
	if (v.size() != 0) {
		sort(v.begin(), v.end());
		d->isFinal = v[0];
	} else
		d->isFinal = 0;
}

bool DFA::notExist(NFAState* t, DFAState* d) {
	for (int i = 0; i < (int) d->states.size(); i++)
		if (t->id == d->states[i]->id)
			return false;
	return true;
}

bool DFA::checkEquivelent(DFAState* u) {
	for (int i = 0; i < (int) DFAStates.size(); i++) {
		if (same(u->states, DFAStates[i]->states)) {
			u->id = DFAStates[i]->id;
			return true;
		}
	}
	return false;
}

bool DFA::same(vector<NFAState*> v1, vector<NFAState*> v2) {
	vector<int> x;
	vector<int> y;
	if (v1.size() == v2.size()) {
		for (int j = 0; j < (int) v1.size(); j++) {
			x.push_back(v1[j]->id);
			y.push_back(v2[j]->id);
		}
		sort(x.begin(), x.end());
		sort(y.begin(), y.end());
		if (equal(x.begin(), x.end(), y.begin()))
			return true;
		else
			return false;
	} else
		return false;
}

vector<vector<int> > DFA::getTransitionTable() {
	return transitionTable;
}
vector<pair<int, int> > DFA::getGroups() {
	return groups;
}
