/*
 * TokensGenerator.cpp
 *
 *  Created on: Mar 28, 2016
 *      Author: mostafa
 */

#include "TokensGenerator.h"

TokensGenerator::TokensGenerator(vector<vector<int> > table,
		map<int, int> isFinal, int startState, map<char, int> input,
		map<int, string> classes) {
	this->table = table;
	this->isFinal = isFinal;
	this->startState = startState;
	this->input = input;
	this->classes = classes;
}

vector<string> TokensGenerator::getTokens(string inputFileName,
		string outputFileName) {
	vector<string> tokens;
	tokens.clear();
	set<string> symbolTable;
	ifstream inputFile(inputFileName.c_str());
	ofstream outputFile(outputFileName.c_str());
	string s = "";
	string line;
	while (getline(inputFile, line)) {
		int i = 0;
		while (i < (int) line.size()) {
			if (Helper::isWhitespaceChar(line[i])) {
				i++;
				continue;
			}
			int lastAC = -1;
			int lastFinal = 0;
			int j = i;
			int state = startState;
			while (j < (int) line.size() && !Helper::isWhitespaceChar(line[j])) {
				if (input.find(line[j]) == input.end())
					break;
				state = table[state][input[line[j]]];
				if (state == -1)
					break;
				if (isFinal[state] != 0) {
					lastAC = j;
					lastFinal = isFinal[state];
				}
				j++;
			}
			if (lastAC != -1) {
				if (classes[lastFinal] == "id")
					symbolTable.insert(line.substr(i, lastAC - i + 1));
				tokens.push_back(classes[lastFinal]);
				outputFile << classes[lastFinal] << endl;
				i = lastAC + 1;
			} else {
				outputFile << "an error message" << endl;
				tokens.push_back("error");
				i++; // skipping this char
			}
		}
	}
	vector<string> symTable(symbolTable.begin(), symbolTable.end());
	outputFile << "\nSymbol Table:" << endl;
	for (int i = 0; i < (int) symTable.size(); i++)
		outputFile << symTable[i] << endl;
	inputFile.close();
	outputFile.close();
	return tokens;
}
