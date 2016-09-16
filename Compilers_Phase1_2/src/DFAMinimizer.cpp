/*
 * DFAMinimizer.cpp
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#include "DFAMinimizer.h"

DFAMinimizer::DFAMinimizer(vector<vector<int> > transitionTable,
		string inputString, vector<pair<int, int> > groups) {
	startState = 0;
	final.clear();
	join.clear();
	while (!split.empty())
		split.pop();
	minimize(transitionTable, inputString, groups);
}

void DFAMinimizer::minimize(vector<vector<int> > transitionTable, string input,
		vector<pair<int, int> > vec) {
	vector<pair<int, int> > v;
	getclasses(vec);
	int num = 0;
	int current = split.size();
	int prev = 0;

	while (current != prev) {
		prev = current;
		mapping();

		for (int i = 0; i < (int) input.size(); i++) {
			num = split.size();

			while (num > 0) {
				if ((split.front()).size() > 1) {
					compare(i, split.front(), transitionTable);
				} else {
					split.push(split.front());
				}
				split.pop();
				num--;
			}
		}
		current = split.size();
	}
}

bool DFAMinimizer::compare1(const pair<int, int>&i, const pair<int, int>&j) {
	return i.second < j.second;
}

bool DFAMinimizer::compare2(const pair<int, int>&i, const pair<int, int>&j) {
	return i.first < j.first;
}

void DFAMinimizer::getclasses(vector<pair<int, int> > v) {
	while (!split.empty())
		split.pop();
	vector<pair<int, int> > vec;
	sort(v.begin(), v.end(), compare1);
	vec.push_back(v[0]);
	for (int i = 0; i < (int) v.size() - 1; i++) {
		if (v[i].second == v[i + 1].second) {
			vec.push_back(v[i + 1]);
		} else {
			sort(vec.begin(), vec.end(), compare2);
			split.push(vec);
			vec.clear();
			vec.push_back(v[i + 1]);
		}
	}
	sort(vec.begin(), vec.end(), compare2);
	split.push(vec);
}

void DFAMinimizer::compare(int in, vector<pair<int, int> > v,
		vector<vector<int> > transitionTable) {
	vector<pair<int, int> > vec;
	bool check[v.size()];

	memset(check, false, sizeof(check));

	for (int i = 0; i < (int) v.size() - 1; i++) {
		if (!check[i]) {
			vec.push_back(v[i]);
			for (int j = i + 1; j < (int) v.size(); j++) {
				if ((join[transitionTable[v[i].first][in]]
						== join[transitionTable[v[j].first][in]])
						&& !check[i]) {

					if (!check[j]) {
						check[j] = true;
						vec.push_back(v[j]);

					}
				}
			}

			if (vec.size() > 0) {
				split.push(vec);
				vec.clear();
			}
		}
	}

	if (!check[v.size() - 1]) {
		vec.clear();
		vec.push_back(v[v.size() - 1]);
		split.push(vec);
	}
}

void DFAMinimizer::mapping() {
	vector<pair<int, int> > v;
	join.clear();

	int s = split.size();

	join.insert(std::pair<int, int>(-1, -1));

	for (int j = 0; j < s; j++) {
		v = split.front();
		for (int i = 0; i < (int) v.size(); i++) {
			join.insert(std::pair<int, int>(v[i].first, j));
		}
		split.push(split.front());
		split.pop();
	}
}

vector<vector<int> > DFAMinimizer::consturctTransitionTable(
		vector<vector<int> > transitionTable) {
	final.clear();
	startState = -1;
	vector<int> t;
	vector<vector<int> > miniDFA;
	map<int, int> mapping;
	int index = 0;
	while (!split.empty()) {
		vector<pair<int, int> > temp = split.front();
		split.pop();
		t.push_back(temp[0].first);
		int min = 1000000;
		for (int i = 0; i < (int) temp.size(); i++) {
			if (temp[i].first == 0)
				startState = index;
			mapping[temp[i].first] = index;
			if (temp[i].second != 0 && temp[i].second < min)
				min = temp[i].second;
		}
		final[index] = (min == 1000000 ? 0 : min);
		index++;
	}
	mapping[-1] = -1;
	final[-1] = 0;
	for (int i = 0; i < (int) t.size(); i++) {
		vector<int> tmp;
		miniDFA.push_back(tmp);
	}
	for (int i = 0; i < (int) t.size(); i++) {
		int current = t[i];
		for (int j = 0; j < (int) transitionTable[0].size(); j++) {
			int next = mapping[transitionTable[current][j]];
			miniDFA[mapping[current]].push_back(next);
		}
	}
	return miniDFA;
}

map<int, int> DFAMinimizer::getFinal() {
	return final;
}

int DFAMinimizer::getStartState() {
	return startState;
}
