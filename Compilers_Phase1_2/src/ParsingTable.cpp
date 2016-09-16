/*
 * ParsingTable.cpp
 *
 *  Created on: Apr 20, 2016
 *      Author: monica
 */

#include "ParsingTable.h"

using namespace std;

ParsingTable::ParsingTable(CFG* cfg) {
	this->cfg = cfg;
	int size = cfg->getNumberOfElements();
	epsilonID = cfg->getEpsId();
	first.resize(size);
	follow.resize(size);
	visited.resize(size);
	hasEps.resize(size);
	done.resize(size);
	fill(visited.begin(), visited.begin() + size, false);
	fill(done.begin(), done.begin() + size, false);
	fill(hasEps.begin(), hasEps.begin() + size, false);
	mapProduction.clear();
	parsingTableMap.clear();
	vector<int> epsVector;
	epsVector.clear();
	epsVector.push_back(epsilonID);
	mapProduction[epsVector] = EPS;
	parsingTableMap[EPS] = epsVector;
	productionNum = 1;
	getFirst();
	getFollow();
	constructTable();
}

void ParsingTable::constructTable() {
	vector<int> vv;
	vv.push_back(SYNCH);
	parsingTableMap[SYNCH] = vv;
	vv.clear();
	vv.push_back(EMPTY);
	parsingTableMap[EMPTY] = vv;
	vector<int> epsVector;
	epsVector.clear();
	epsVector.push_back(epsilonID);
	int noOfTerminals = cfg->getTerminals().size();
	vector<pair<int, vector<vector<int> > > > v = cfg->getNonTerminals();
	// for all non terminals
	for (int i = 0; i < (int) v.size(); i++) {
		int nonTerminalId = v[i].first;
		vector<int> v;
		v.resize(noOfTerminals);
		fill(v.begin(), v.end(), EMPTY);
		parseTable.push_back(v);

		//mapping row to id of nonTerminal
		if (nonTermRow.find(nonTerminalId) == nonTermRow.end()) {
			nonTermRow[nonTerminalId] = i;
			rowNonTerm[i] = nonTerminalId;
		}

		vector<pair<int, int> > inputFirst = mapInputProduction[nonTerminalId];

		for (int j = 0; j < (int) inputFirst.size(); j++) {
			pair<int, int> termPro = inputFirst[j];
			int col = termCol[termPro.first];
			parseTable[i][col] = termPro.second;
		}

		set<int>::iterator it;
		set<int> s = follow[nonTerminalId];

		for (it = s.begin(); it != s.end(); ++it) {
			int f = *it;
			int col = termCol[f];
			if (parseTable[i][col] == -1) {
				if (hasEps[nonTerminalId])
					parseTable[i][col] = mapProduction[epsVector];
				else
					parseTable[i][col] = SYNCH;
			}
		}
	}
}
void ParsingTable::print() {
	/*  vector<pair<int, vector<vector<int> > > > v = cfg->getNonTerminals();
	 for (int i = 0; i < v.size(); i++)
	 {
	 vector<pair<int,int>> vv=mapINputProduction[v[i].first];
	 for(int j=0; j<vv.size(); j++)
	 {
	 cout<<"id: "<<cfg->getMap()[v[i].first]<<" input: "<<cfg->getMap()[vv[j].first]<<" proNum: "<<vv[j].second<<" : ";
	 vector<int> k =mapProduction2[vv[j].second];
	 for(int l=0; l<k.size(); l++)
	 {
	 cout<<cfg->getMap()[k[l]]<<" ";
	 }
	 cout<<endl;
	 }
	 }
	 for (int i = 0; i < productionNum; i++)
	 {
	 cout<<i<<" : ";
	 vector<int> v= mapProduction2[i];
	 for(int j=0; j<v.size(); j++)
	 {
	 cout<<cfg->getMap()[v[j]];
	 }
	 cout<<endl;

	 }
	 */
	for (int col = 0; col < (int) cfg->getTerminals().size(); col++) {
		cout << cfg->getMap()[colTerm[col]] << "  ";
	}
	cout << endl;
	for (int i = 0; i < (int) parseTable.size(); i++) {
		cout << cfg->getMap()[rowNonTerm[i]] << " :: ";
		for (int j = 0; j < (int) parseTable[i].size(); j++) {
			if (parseTable[i][j] >= 0) {
				vector<int> v = parsingTableMap[parseTable[i][j]];
				for (int k = 0; k < (int) v.size(); k++) {
					cout << cfg->getMap()[v[k]] << " ";
				}
				cout << "  ";
			} else
				cout << parseTable[i][j] << "  ";
		}
		cout << endl << endl;
	}

}
void ParsingTable::getFirst() {
	getFirstTerminals();
	vector<pair<int, vector<vector<int> > > > v = cfg->getNonTerminals();
	for (int i = v.size() - 1; i >= 0; i--) {
		if (!done[v[i].first]) {
			if (!getFirstNonTerminals(v[i].first)) {
				cout << "error" << endl;
				break;
			}
		}
	}

}
void ParsingTable::getFirstTerminals() {
	set<int>::iterator it;
	set<int> terminals = cfg->getTerminals();
	int col = 0;
	for (it = terminals.begin(); it != terminals.end(); ++it) {
		int f = *it;
		vector<int> v;
		v.push_back(f);
		first[f] = v;
		visited[f] = true;
		done[f] = true;
		if (termCol.find(*it) == termCol.end()) {
			termCol.insert(make_pair(*it, col));
			termSCol.insert(make_pair(cfg->getMap()[*it], col));
			colTerm.insert(make_pair(col, *it));
			col++;
		}
	}
}
bool ParsingTable::getFirstNonTerminals(int id) {
	vector<vector<int> > productions =
			cfg->getNonTerminals()[cfg->getNonTerminalPos(id)].second;
	visited[id] = true;
	vector<int> v;
	set<int> temp;
	vector<int> pro;
	vector<pair<int, int> > m;
	for (int i = 0; i < (int) productions.size(); i++) {
		bool flag = 0;
		pro = productions[i];
		for (int j = 0; j < (int) productions[i].size(); j++) {
			if (visited[productions[i][j]] && !done[productions[i][j]])
				return false;
			//not terminal
			if (!done[productions[i][j]]) {
				if (!getFirstNonTerminals(productions[i][j]))
					return false;
			}
			flag = 0;
			vector<int> p = first[productions[i][j]];
			for (int k = 0; k < (int) p.size(); k++) {
				if (temp.find(p[k]) == temp.end() && p[k] != epsilonID) {
					v.push_back(p[k]);
					temp.insert(p[k]);
					if (mapProduction.find(pro) != mapProduction.end()) {
						m.push_back(make_pair(p[k], mapProduction[pro]));
					} else
						m.push_back(make_pair(p[k], productionNum));
				} else if (p[k] == epsilonID) {
					flag = 1;
//					hasEps[id] = true;
				} else {
					cout << "Grammar needs left Factoring" << endl;
					return false;
				}

			}
			if (!flag)
				break;
		}
		if (flag) {
			hasEps[id] = true;
			v.push_back(epsilonID);
			if (mapProduction.find(pro) != mapProduction.end()) {
				m.push_back(make_pair(epsilonID, mapProduction[pro]));
			} else {
				m.push_back(make_pair(epsilonID, productionNum));
			}
		}
		if (mapProduction.find(pro) == mapProduction.end()) {
			mapProduction[pro] = productionNum;
			parsingTableMap[productionNum] = pro;
			productionNum++;
		}

	}
	mapInputProduction[id] = m;
	first[id] = v;
	done[id] = true;
	return true;

}
void ParsingTable::getFollow() {
	fill(visited.begin(), visited.end(), false);
	fill(done.begin(), done.end(), false);
	map<int, vector<int> > need;
	vector<pair<int, vector<vector<int> > > > v = cfg->getNonTerminals();
	for (int i = 0; i < (int) v.size(); i++) {
		int currentState = v[i].first;
		if (!currentState)
			follow[currentState].insert(epsilonID);
		vector<pair<int, pair<int, int> > > lines =
				cfg->getPosRightHandSide()[currentState];
		visited[currentState] = true;
		for (int p = 0; p < (int) lines.size(); p++) {
			int r = lines[p].first;
			int c = lines[p].second.first;
			int index = lines[p].second.second + 1;
			vector<int> pro = v[r].second[c];
			bool hasEps = false;
			int k = index;
			while (k < (int) pro.size()) {
				hasEps = false;
				for (int j = 0; j < (int) first[pro[k]].size(); j++) {
					if (first[pro[k]][j] == epsilonID) {
						hasEps = true;
					} else {
						follow[currentState].insert(first[pro[k]][j]);
					}
				}
				if (!hasEps)
					break;
				k++;
			}
			if (hasEps || index == (int) pro.size()) {
				int left = v[r].first;
				if (!visited[left]) {
					if (need.find(currentState) == need.end()) {
						vector<int> vv;
						need[currentState] = vv;
					}
					need[currentState].push_back(left);
				} else {
					done[currentState] = helpFollow(currentState, need, left);

				}
			}
		}

	}
	for (int i = 0; i < (int) done.size(); i++) {
		if (!done[i])
			checkDone(i, need);
	}

}
void ParsingTable::sub(int currentState, int left) {
	set<int>::iterator it;
	set<int> s = follow[left];
	for (it = s.begin(); it != s.end(); ++it) {
		int f = *it;
		follow[currentState].insert(f);
	}
}
bool ParsingTable::helpFollow(int currentState, map<int, vector<int> > need,
		int left) {
	if (left == currentState)
		return true;
	sub(currentState, left);
	if (done[left] || need.find(left) == need.end())
		return true;
	bool done = true;
	vector<int> followNeed = need[left];
	for (int i = 0; i < (int) followNeed.size(); i++) {
		if (followNeed[i] != currentState) {
			if (visited[followNeed[i]]) {
				done = done & helpFollow(currentState, need, followNeed[i]);
			}
		}
	}

	return done;
}

void ParsingTable::checkDone(int cur, map<int, vector<int> > need) {
	if (need.find(cur) == need.end())
		return;
	vector<int> v = need[cur];
	for (int i = 0; i < (int) v.size(); i++) {
		if (!done[v[i]]) {
			checkDone(v[i], need);
		}
		sub(cur, v[i]);
	}
	done[cur] = true;
}

void ParsingTable::test() {
	for (int i = 0; i < (int) first.size(); i++) {
		cout << cfg->getMap()[i] << "	first : ";
		for (int j = 0; j < (int) first[i].size(); j++) {
			cout << cfg->getMap()[first[i][j]] << "	";
		}
		cout << endl;
	}
	cout << endl;

	for (int i = 0; i < (int) follow.size(); i++) {
		if (!cfg->isTerminal(i)) {
			cout << cfg->getMap()[i] << "	follow : ";

			std::set<int>::iterator it;
			set<int> tmp = follow[i];
			for (it = tmp.begin(); it != tmp.end(); ++it) {
				int f = *it;
				cout << cfg->getMap()[f] << "	";
			}
			cout << endl;
		}
	}
	cout << endl;

}

