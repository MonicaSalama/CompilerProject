/*
 * DFAState.cpp
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#include "DFAState.h"

DFAState::DFAState(int stateId) {
	states.clear();
	id = stateId;
	isFinal = 0;
}


void DFAState::addToStates(NFAState* s) {
	states.push_back(s);
}

void DFAState::addToStates(vector<NFAState*> v) {
	for (int i = 0; i < (int) v.size(); i++)
		states.push_back(v[i]);
}

void DFAState::setFinal(int newFinal) {
	isFinal = newFinal;
}

void DFAState::setId(int newId) {
	id = newId;
}
