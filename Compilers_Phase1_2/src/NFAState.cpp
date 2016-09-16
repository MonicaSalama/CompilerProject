/*
 * NFAState.cpp
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#include "NFAState.h"

NFAState::NFAState(int state_id) {
	isFinal = 0;
	id = state_id;
}

void NFAState::addToMap(char c, NFAState* next) {
	if (edges.find(c) == edges.end()) {
		vector<NFAState*> v;
		edges[c] = v;
	}
	edges[c].push_back(next);
}

