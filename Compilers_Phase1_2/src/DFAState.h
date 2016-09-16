/*
 * DFAState.h
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#ifndef DFASTATE_H_
#define DFASTATE_H_

#include "NFAState.h"
#include <vector>
using namespace std;

class DFAState {
public:
	DFAState(int stateId);
	void addToStates(NFAState* s);
	void addToStates(vector<NFAState*> v);
	void setFinal(int newFinal);
	void setId(int newId);
	int isFinal;
	int id;
	vector<NFAState*> states;
};

#endif /* DFASTATE_H_ */
