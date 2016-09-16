#include "PredictiveParsing.h"

PredictiveParsing::PredictiveParsing(map<int, int> row, map<string, int> col,
		map<int, vector<int> > productive, map<int, string> getmap,
		set<int> isterminal, vector<vector<int> > parsingtable, int epsilonId) {

	this->row = row;
	this->col = col;
	this->productive = productive;
	this->getmap = getmap;
	this->isterminal = isterminal;
	this->parsingtable = parsingtable;
	this->epsilonId = epsilonId;
}

void PredictiveParsing::printstack() {
	for (int i = 0; i < (int) vec.size(); i++) {
		if (getmap[vec[i]] == "$") {
			cout << "" << endl;
		} else {
			cout << getmap[vec[i]];
		}
	}
}

void PredictiveParsing::print(vector<string> v1, int i, int error1) {
	stack<int> s1;
	// cout<<"stack:";

	while (!s.empty()) {
		if (getmap[s.top()] != "$") {
			cout << getmap[s.top()];
			cout << "   ";
		}
		s1.push(s.top());
		vec.push_back(s.top());
		s.pop();
	}
	while (s1.size() != 0) {
		s.push(s1.top());
		s1.pop();
	}
	cout << endl;
	cout << endl;
	//  cout<<"input:";
	//   for(int j =i ;j<v1.size();j++)
	//  {
	//     cout<<v1[j] + "  ";
	// }
	//  cout<<""<<endl;
	// cout<<endl;

}

vector<int> PredictiveParsing::match(int nonterminal, string in,
		vector<vector<int> > parsingtable) {
	int r = 0;
	int c = 0;
	int p = 0;
	vector<int> ans;
	r = row[nonterminal];
	c = col[in];
	p = parsingtable[r][c];
	//pair<int,int> temp =productive[p];
	ans = productive[p];
	return ans;
}

void PredictiveParsing::parse(vector<string> v) {
	while (!s.empty())
		s.pop();
	int i = 0;
	int flag = 0;
	vector<int> ans;

	v.push_back("$");
	s.push(EPS);

	s.push(0);
	print(v, i, 1);

	while (s.top() != EPS) {
		if (v[i] == "error") {
			i++;
			cout << "Error from lexical analyzer" << endl;
			continue;
		}
		if (isterminal.find(s.top()) != isterminal.end()
				&& v[i] == getmap[s.top()]) {
			s.pop();
			i++;
			cout << "MATCHED " << v[i - 1] << endl;
			print(v, i, 1);
		} else if (isterminal.find(s.top()) != isterminal.end()) {
			cout << "error " << getmap[s.top()] << endl;
			s.pop();
			print(v, i, 1);
		} else {

			ans = match(s.top(), v[i], parsingtable);

			if (ans[0] == EMPTY) {
				cout << "error, skip " + v[i] << endl;
				i++;
				print(v, i, 1);
			} else if (ans[0] == epsilonId) {
				s.pop();
				cout << "MATCHED $" << endl;
				print(v, i, 1);
			} else if (ans[0] == SYNCH) {
				if (s.size() == 2 && flag == 0) {
					cout << "error, skip " + v[i] << endl;
					i++;
				} else {
					cout << "error, sync pop " + getmap[s.top()] << endl;
					s.pop();
				}
				print(v, i, 1);
			} else {
				s.pop();
				for (int j = ans.size() - 1; j >= 0; j--) {
					s.push(ans[j]);
				}
				print(v, i, 1);
				ans.clear();
			}
		}
		flag = 1;
	}
	while (i != (int) v.size() - 1) {
		i++;
	}
}
