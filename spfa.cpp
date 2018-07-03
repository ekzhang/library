// spfa.cpp
// Eric Zhang; July 17, 2016

// Shortest Path Faster Algorithm (SPFA)
// SPFA is a variant of bellman-ford that finds
// single-source minimum paths from a vertex. It
// has an amazing average case O(E) time complexity,
// but keeps bellman-ford's O(VE) worst-case.

#include <bits/stdc++.h>
using namespace std;

typedef long long LL;

#define MAXN 100013
#define MAXM 100013
#define INF (1<<30)

int N, M;
vector<pair<int, int> > adj[MAXN];
LL dist[MAXN];
int pre[MAXN];
bool inQueue[MAXN];

void spfa(int start) {
	fill(dist, dist + N, INF);
	memset(pre, -1, sizeof pre);
	memset(inQueue, 0, sizeof inQueue);

	dist[start] = 0;
	list<int> q;
	q.push_back(start);
	inQueue[start] = true;

	while (!q.empty()) {
		int v = q.front();
		q.pop_front();
		inQueue[v] = false;
		for (auto p : adj[v]) {
			int u = p.first;
			LL d = dist[v] + p.second;
			if (d < dist[u]) {
				dist[u] = d, pre[u] = v;
				if (!inQueue[u]) {
					if (q.size() && d < dist[q.front()]) q.push_front(u);
					else q.push_back(u);
					inQueue[u] = true;
				}
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cin >> N >> M;
	for (int i = 0; i < M; i++) {
		int A, B, D;
		cin >> A >> B >> D;
		A--; B--;
		adj[A].emplace_back(B, D);
		adj[B].emplace_back(A, D);
	}

	spfa(0);
	for (int i = 0; i < N; i++) {
		cout << dist[i] << '\n';
	}

	cout.flush();
	return 0;
}
