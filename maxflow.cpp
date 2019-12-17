// maxflow.cpp
// Eric K. Zhang; Aug. 7, 2017

#include <bits/stdc++.h>
using namespace std;

/* Maximum-Flow solver using Dinic's Blocking Flow Algorithm.
 * Time Complexity:
 *   - O(V^2 E) for general graphs, but in practice ~O(E^1.5)
 *   - O(V^(1/2) E) for bipartite matching
 *   - O(min(V^(2/3), E^(1/2)) E) for unit capacity graphs
 * 
 * WARNING: prefer maxflow2.cpp when possible (which is faster)
 */
template<int V, class T=long long>
class max_flow {
	static const T INF = numeric_limits<T>::max();

	unordered_map<int, T> cap[V];
	int dist[V];
	bool blocked[V];

	void bfs(int s) {
		memset(dist, -1, sizeof dist);
		dist[s] = 0;
		queue<int> q;
		q.push(s);
		while (!q.empty()) {
			int n = q.front();
			q.pop();
			for (auto p : cap[n]) {
				if (p.second && dist[p.first] == -1) {
					dist[p.first] = dist[n] + 1;
					q.push(p.first);
				}
			}
		}
	}

	T augment(int n, T amt, int t) {
		if (n == t) return amt;
		if (blocked[n]) return 0;
		T remaining = amt;
		for (auto p : cap[n]) {
			if (dist[p.first] != dist[n] + 1) continue;
			T flowpart = min(remaining, cap[n][p.first]);
			if (flowpart) {
				T cur = augment(p.first, flowpart, t);
				remaining -= cur;
				cap[n][p.first] -= cur;
				cap[p.first][n] += cur;
			}
		}
		if (remaining) blocked[n] = true;
		return amt - remaining;
	}

public:
	void add(int u, int v, T f=1) {
		cap[u][v] += f;
	}

	T calc(int s, int t) {
		T ans = 0;
		while (true) {
			bfs(s);
			memset(blocked, false, sizeof blocked);
			T aug = augment(s, INF, t);
			if (!aug) break;
			ans += aug;
		}
		return ans;
	}

	void clear() {
		for (int i = 0; i < V; i++) {
			cap[i].clear();
		}
	}
};

int main() {
	/* Example of usage */
	max_flow<4> network;
	network.add(0, 1, 75);
	network.add(0, 2, 50);
	network.add(1, 2, 40);
	network.add(1, 3, 50);
	network.add(2, 3, 30);

	int flow = network.calc(0, 3);

	/* Max-flow should be 80. */
	cout << flow << endl;

	return 0;
}
