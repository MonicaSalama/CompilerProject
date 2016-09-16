/*
 * Trie.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: mostafa
 */

#include "Trie.h"

Trie::Trie() {
	m.clear();
	isEnd = 0;
	cnt = 0;
	isRoot = false;
}

void Trie::insert(vector<int> v) {
	int pos = 0;
	Trie* root = this;
	while (pos < (int) v.size()) {
		root->cnt++;
		int id = v[pos++];
		if (root->m.find(id) == root->m.end())
			root->m[id] = new Trie();
		root = root->m[id];
	}
	root->isEnd++;
	root->cnt++;
}

bool Trie::getCommon() {
	return cnt > 1;
}
