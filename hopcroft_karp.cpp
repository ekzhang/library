// hopcroft_karp.cpp
// Eric K. Zhang; Nov. 22, 2017

#include <bits/stdc++.h>
using namespace std;

#define MAXG 100013
int N, M, P;
vector<int> adj[MAXG];
int match[MAXG];
int dist[MAXG];

bool bfs() {
	queue<int> q;
	memset(dist, -1, sizeof dist);
	for (int i = 0; i < N; i++) {
		if (match[i] == -1) {
			q.push(i);
			dist[i] = 0;
		}
	}
	bool reached = false;
	while (!q.empty()) {
		int n = q.front();
		q.pop();
		for (int v : adj[n]) {
			if (match[v] == -1) reached = true;
			else if (dist[match[v]] == -1) {
				dist[match[v]] = dist[n] + 1;
				q.push(match[v]);
			}
		}
	}
	return reached;
}

bool dfs(int n) {
	if (n == -1) return true;
	for (int v : adj[n]) {
		if (match[v] == -1 || dist[match[v]] == dist[n] + 1) {
			if (dfs(match[v])) {
				match[v] = n, match[n] = v;
				return true;
			}
		}
	}
	return false;
}

int hopcroft_karp() {
	memset(match, -1, sizeof match);

	int matching = 0;
	while (bfs()) {
		for (int i = 0; i < N; i++)
			if (match[i] == -1 && dfs(i))
				matching++;
	}
	return matching;
}

int main() {
	// http://www.spoj.com/problems/MATCHING/

	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cin >> N >> M >> P;
	for (int i = 0; i < P; i++) {
		int A, B; cin >> A >> B; --A, --B;
		adj[A].push_back(N + B);
		adj[N + B].push_back(A);
	}

	int matching = hopcroft_karp();

	cout << matching << endl;
	return 0;
}
