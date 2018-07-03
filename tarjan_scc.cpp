// tarjan_scc.cpp
// Eric K. Zhang; Nov. 22, 2017

#include <bits/stdc++.h>
using namespace std;

#define MAXN 1000

namespace tarjan {
	int N;
	vector<int> adj[MAXN];
	int scc[MAXN], sccnum = 0;
	int in[MAXN], low[MAXN], t = 0;
	stack<int> s;
	bool instack[MAXN];

	void dfs(int n) {
		low[n] = in[n] = t++;
		s.push(n);
		instack[n] = true;
		for (int m : adj[n]) {
			if (in[m] == -1) {
				dfs(m);
				low[n] = min(low[n], low[m]);
			}
			else if (instack[m]) {
				low[n] = min(low[n], in[m]);
			}
		}
		if (low[n] == in[n]) {
			while (true) {
				int u = s.top();
				s.pop();
				scc[u] = sccnum;
				instack[u] = false;
				if (u == n) break;
			}
			++sccnum;
		}
	}

	void tarjan() {
		memset(scc, -1, sizeof scc);
		memset(in, -1, sizeof in);
		for (int i = 0; i < N; i++) {
			if (scc[i] == -1) {
				dfs(i);
			}
		}
	}
}

int main() {
	// 0 ---> 1 ---> 2 <--> 3
	// ^    /`              ^
	// |  /`  |      |      |
	//  v`    v      v      v
	// 4 ---> 5 <--> 6 <--- 7
	tarjan::N = 8;
	vector<pair<int, int> > edges = {
		{0, 1}, {1, 2}, {1, 4}, {1, 5},
		{2, 3}, {2, 6}, {3, 2}, {3, 7},
		{4, 0}, {4, 5}, {5, 6}, {6, 5},
		{7, 3}, {7, 6}
	};
	for (auto p : edges) {
		tarjan::adj[p.first].push_back(p.second);
	}

	tarjan::tarjan();

	for (int i = 0; i < tarjan::sccnum; i++) {
		cout << "SCC #" << tarjan::sccnum - i << ":";
		for (int j = 0; j < tarjan::N; j++) {
			if (tarjan::scc[j] == i) {
				cout << ' ' << j;
			}
		}
		cout << endl;
	}
	return 0;
}
