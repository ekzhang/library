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
		edge(int t, int r, T c, T f) : t(t), rev(r), cap(c), f(f) { }
	};

	vector<edge> adj[V];
	int dist[V];
	int ptr[V];

	void bfs(int s) {
		memset(dist, -1, sizeof dist);
		dist[s] = 0;
		queue<int> q;
		q.push(s);
		while (!q.empty()) {
			int n = q.front();
			q.pop();
			for (auto& e : adj[n]) {
				if (dist[e.t] == -1 && e.cap != e.f) {
					dist[e.t] = dist[n] + 1;
					q.push(e.t);
				}
			}
		}
	}

	T augment(int n, T amt, int t) {
		if (n == t) return amt;
		while (ptr[n] != adj[n].size()) {
			auto& e = adj[n][ptr[n]++];
			int v = e.t;
			if (dist[v] == dist[n] + 1 && e.cap != e.f) {
				T flow = augment(v, min(amt, e.cap - e.f), t);
				if (flow != 0) {
					e.f += flow;
					adj[v][e.rev].f -= flow;
					return flow;
				}
			}
		}
		return 0;
	}

public:
	void add(int u, int v, T cap=1) {
		adj[u].emplace_back(v, adj[v].size(), cap, 0);
		adj[v].emplace_back(u, adj[u].size() - 1, 0, 0);
	}

	T calc(int s, int t) {
		T flow = 0;
		while (true) {
			bfs(s);
			if (dist[t] == -1) break;
			memset(ptr, 0, sizeof ptr);
			while (true) {
				T df = augment(s, INF, t);
				if (!df) break;
				flow += df;
			}
		}
		return flow;
	}

	void clear() {
		for (int i = 0; i < V; i++) {
			adj[i].clear();
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
