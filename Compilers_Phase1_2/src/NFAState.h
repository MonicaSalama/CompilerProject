/*
 * NFAState.h
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#ifndef NFASTATE_H_
#define NFASTATE_H_

#include <map>
#include <vector>
using namespace std;

class NFAState {
public:
	int id;
	int isFinal;
	map<char, vector<NFAState*> > edges;
	NFAState(int state_id);
	void addToMap(char c, NFAState* next);
};

#endif /* NFASTATE_H_ */
