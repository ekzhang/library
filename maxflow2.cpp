// maxflow2.cpp
// Eric K. Zhang; Aug. 7, 2017

#include <bits/stdc++.h>
using namespace std;

/* Maximum-Flow solver using Dinic's Blocking Flow Algorithm.
 * Time Complexity:
 *   - O(V^2 E) for general graphs, but in practice ~O(E^1.5)
 *   - O(V^(1/2) E) for bipartite matching
 *   - O(min(V^(2/3), E^(1/2)) E) for unit capacity graphs
 */
template<int V, class T=long long>
class max_flow {
	static const T INF = numeric_limits<T>::max();

	struct edge {
		int t, rev;
		T cap, f;
	};

	vector<edge> adj[V];
	int dist[V];
	int ptr[V];

	bool bfs(int s, int t) {
		memset(dist, -1, sizeof dist);
		dist[s] = 0;
		queue<int> q({ s });
		while (!q.empty() && dist[t] == -1) {
			int n = q.front();
			q.pop();
			for (auto& e : adj[n]) {
				if (dist[e.t] == -1 && e.cap != e.f) {
					dist[e.t] = dist[n] + 1;
					q.push(e.t);
				}
			}
		}
		return dist[t] != -1;
	}

	T augment(int n, T amt, int t) {
		if (n == t) return amt;
		for (; ptr[n] < adj[n].size(); ptr[n]++) {
			edge& e = adj[n][ptr[n]];
			if (dist[e.t] == dist[n] + 1 && e.cap != e.f) {
				T flow = augment(e.t, min(amt, e.cap - e.f), t);
				if (flow != 0) {
					e.f += flow;
					adj[e.t][e.rev].f -= flow;
					return flow;
				}
			}
		}
		return 0;
	}

public:
	void add(int u, int v, T cap=1, T rcap=0) {
		adj[u].push_back({ v, (int) adj[v].size(), cap, 0 });
		adj[v].push_back({ u, (int) adj[u].size() - 1, rcap, 0 });
	}

	T calc(int s, int t) {
		T flow = 0;
		while (bfs(s, t)) {
			memset(ptr, 0, sizeof ptr);
			while (T df = augment(s, INF, t))
				flow += df;
		}
		return flow;
	}

	void clear() {
		for (int n = 0; n < V; n++)
			adj[n].clear();
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
