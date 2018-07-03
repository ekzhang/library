// tarjan_bcc.cpp
// Eric K. Zhang; Dec. 21, 2017

#include <bits/stdc++.h>
using namespace std;

#define MAXN 1000

namespace tarjan {
	int N;
	vector<int> adj[MAXN];
	vector<int> bcc[MAXN];
	int bccnum = 0;
	int in[MAXN], low[MAXN], t = 0;
	stack<pair<int, int> > s;
	bool visited[MAXN];

	void dfs(int n, int p=-1) {
		visited[n] = true;
		low[n] = in[n] = t++;
		for (int v : adj[n]) if (v != p) {
			if (!visited[v]) {
				s.emplace(v, n);
				dfs(v, n);
				low[n] = min(low[n], low[v]);
				if (low[v] >= in[n]) {
					while (true) {
						auto p = s.top();
						s.pop();
						int a = p.first, b = p.second;
						if (bcc[a].empty() || bcc[a].back() != bccnum)
							bcc[a].push_back(bccnum);
						if (bcc[b].empty() || bcc[b].back() != bccnum)
							bcc[b].push_back(bccnum);
						if (a == v && b == n) break;
					}
					++bccnum;
				}
			}
			else if (in[v] < in[n]) {
				low[n] = min(low[n], in[v]);
				s.emplace(v, n);
			}
		}
	}

	void tarjan() {
		for (int i = 0; i < N; i++) {
			if (!visited[i])
				dfs(i);
		}
	}

	bool biconnected(int u, int v) {
		for (int c : bcc[u]) {
			if (binary_search(bcc[v].begin(), bcc[v].end(), c))
				return true;
		}
		return false;
	}
}

int main() {
	tarjan::N = 9;
	vector<pair<int, int> > edges = {
		{0, 1}, {1, 2}, {2, 3}, {3, 1},
		{1, 4}, {4, 5}, {5, 0}, {0, 6},
		{6, 7}, {7, 8}, {8, 6}
	};
	for (auto p : edges) {
		tarjan::adj[p.first].push_back(p.second);
		tarjan::adj[p.second].push_back(p.first);
	}

	tarjan::tarjan();

	for (int i = 0; i < tarjan::bccnum; i++) {
		cout << "BCC #" << i + 1 << ":";
		for (int j = 0; j < tarjan::N; j++) {
			auto& v = tarjan::bcc[j];
			if (find(v.begin(), v.end(), i) != v.end()) {
				cout << ' ' << j;
			}
		}
		cout << endl;
	}
	return 0;
}
