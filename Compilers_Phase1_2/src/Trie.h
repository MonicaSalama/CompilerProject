/*
 * Trie.h
 *
 *  Created on: Apr 23, 2016
 *      Author: mostafa
 */

#ifndef TRIE_H_
#define TRIE_H_

#include <map>
#include <vector>
#include <iostream>

using namespace std;

class Trie {
public:
	Trie();
	void insert(vector<int> v);
	bool getCommon();
	map<int, Trie*> m;
	bool isRoot;
private:
	int isEnd;
	int cnt;
};

#endif /* TRIE_H_ */
