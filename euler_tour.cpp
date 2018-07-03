// euler_tour.cpp
// Eric K. Zhang; Dec. 22, 2017

#include <bits/stdc++.h>
using namespace std;

#define MAXN 100013
#define MAXM 1000013
int N, M;
vector<pair<int, int> > adj[MAXN];
int cur[MAXN];
bool used[MAXM];
vector<int> tour;

void dfs(int n) {
	while (cur[n] != adj[n].size()) {
		if (used[adj[n][cur[n]].second]) {
			++cur[n];
			continue;
		}
		auto p = adj[n][cur[n]++];
		used[p.second] = true;
		dfs(p.first);
	}
	tour.push_back(n);
}

int main() {
	cin >> N >> M;
	for (int i = 0; i < M; i++) {
		int u, v;
		cin >> u >> v;
		--u; --v;
		adj[u].emplace_back(v, i);
		adj[v].emplace_back(u, i);
	}
	dfs(0);
	cout << tour.size() << endl;
	for (int v : tour) {
		cout << v + 1 << ' ';
	}
	cout << endl;
	return 0;
}
