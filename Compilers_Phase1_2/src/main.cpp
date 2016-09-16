//============================================================================
// Name        : Compilers.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "LexicalParser.h"
#include "NFA.h"
#include "NFAState.h"
#include "DFA.h"
#include "DFAMinimizer.h"
#include "TokensGenerator.h"
#include "CFG.h"
#include "ParsingTable.h"
using namespace std;

void csvFile(vector<vector<int> > table, map<int, int> is_final, string in,
		map<int, string> classes) {
	ifstream indata;
	ofstream outdata;
	outdata.open("TransitionTable.csv", ios_base::out);
	outdata.clear();
	outdata << "State^";
	for (int i = 0; i < (int) in.size(); i++)
		outdata << in[i] << "^";
	outdata << "class" << endl;
	for (int i = 0; i < (int) table.size(); i++) {
		outdata << i;
		for (int j = 0; j < (int) table[i].size(); j++) {
			outdata << "^" << table[i][j];
		}
		if (is_final[i] != 0)
			outdata << "^" << classes[is_final[i]];
		else
			outdata << "^" << "Not Final";
		outdata << endl;
	}
	outdata.close();
}

int main(int argc, char *argv[]) {
	string rules = "rules";
	string outputFileName = "tokens";
	string cfgFileName = "cfg";
	if (argc >= 2) {
		rules = argv[1];
	}
	if (argc >= 3) {
		outputFileName = argv[2];
	}
	if (argc >= 4) {
		cfgFileName = argv[3];
	}

	LexicalParser* lp = new LexicalParser(rules);

	NFA* nfa = new NFA(lp->getDefinitions(), lp->getKeywordsPunc(),
			lp->getRE());

	NFAState* startState = nfa->getStartState();

	string inputString = Helper::constructInputString(nfa->getInputMap());

	DFA* dfa = new DFA(startState, inputString);

	DFAMinimizer* dfaMinimizer = new DFAMinimizer(dfa->getTransitionTable(),
			inputString, dfa->getGroups());

	vector<vector<int> > miniDFA = dfaMinimizer->consturctTransitionTable(
			dfa->getTransitionTable());

	csvFile(miniDFA, dfaMinimizer->getFinal(), inputString, nfa->getClasses());

	CFG* cfg = new CFG(cfgFileName);

	map<int, string> m = cfg->getMap();
	set<int> terminals = cfg->getTerminals();
	vector<pair<int, vector<vector<int> > > > nonTerminals =
			cfg->getNonTerminals();

	vector<pair<int, vector<vector<int> > > >::iterator iter2;
	for (iter2 = nonTerminals.begin(); iter2 != nonTerminals.end(); ++iter2) {
		pair<int, vector<vector<int> > > x = *iter2;
		vector<vector<int> > vvi = x.second;
		for (int i = 0; i < (int) vvi.size(); i++) {
			cout << m[x.first] << ":";
			for (int j = 0; j < (int) vvi[i].size(); j++) {
				cout << " " << m[vvi[i][j]];
			}
			cout << endl;
		}
		cout << endl;
	}

	ParsingTable* pt = new ParsingTable(cfg);

	pt->print();

	TokensGenerator* tg = new TokensGenerator(miniDFA, dfaMinimizer->getFinal(),
			dfaMinimizer->getStartState(), nfa->getInputMap(),
			nfa->getClasses());

	PredictiveParsing* pp = new PredictiveParsing(pt->nonTermRow, pt->termSCol,
			pt->parsingTableMap, cfg->getMap(), cfg->getTerminals(),
			pt->parseTable, pt->epsilonID);

	string inputFileName;

	cout << "File name?" << endl;
	while (getline(cin, inputFileName)) {
		vector<string> tokens = tg->getTokens(inputFileName, outputFileName);
		pp->parse(tokens);
		cout << "File name?" << endl;
	}

	return 0;
}
