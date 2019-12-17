// mincost_maxflow.cpp
// Eric K. Zhang; Nov. 22, 2017

#include <bits/stdc++.h>
using namespace std;

/* Minimum-Cost, Maximum-Flow solver using Successive Shortest Paths with Dijkstra and SPFA-SLF.
 * Requirements:
 *   - Duplicate or antiparallel edges with different costs are allowed.
 *   - No negative cycles.
 * Time Complexity: O(Ef lg V) average-case, O(VE + Ef lg V) worst-case with negative costs.
 */
template<int V, class T=long long>
class mcmf {
	/* making this static breaks compilation on -O0, but not on -O2; unsure why */
	const T INF = numeric_limits<T>::max();

	struct edge {
		int t, rev;
		T cap, cost, f;
	};

	vector<edge> adj[V];
	T dist[V];
	int pre[V];
	bool vis[V];

	void spfa(int s) { /* only needed if there are negative costs */
		list<int> q;

		memset(pre, -1, sizeof pre);
		memset(vis, 0, sizeof vis);
		fill(dist, dist + V, INF);

		dist[s] = 0;
		q.push_back(s);
		while (!q.empty()) {
			int v = q.front();
			q.pop_front();
			vis[v] = false;
			for (auto e : adj[v]) if (e.cap != e.f) {
				int u = e.t;
				T d = dist[v] + e.cost;
				if (d < dist[u]) {
					dist[u] = d, pre[u] = e.rev;
					if (!vis[u]) {
						if (q.size() && d < dist[q.front()]) q.push_front(u);
						else q.push_back(u);
						vis[u] = true;
					}
				}
			}
		}
	}

	priority_queue<pair<T, int>, vector<pair<T, int> >,
		greater<pair<T, int> > > pq; /* for dijkstra */

	void dijkstra(int s) {
		memset(pre, -1, sizeof pre);
		memset(vis, 0, sizeof vis);
		fill(dist, dist + V, INF);

		dist[s] = 0;
		pq.emplace(0, s);
		while (!pq.empty()) {
			int v = pq.top().second;
			pq.pop();
			if (vis[v]) continue;
			vis[v] = true;
			for (auto e : adj[v]) if (e.cap != e.f) {
				int u = e.t;
				T d = dist[v] + e.cost;
				if (d < dist[u]) {
					dist[u] = d, pre[u] = e.rev;
					pq.emplace(d, u);
				}
			}
		}
	}

	void reweight() {
		for (int v = 0; v < V; v++)
			for (auto& e : adj[v])
				e.cost += dist[v] - dist[e.t];
	}

public:
	void add(int u, int v, T cap=1, T cost=0) {
		adj[u].push_back({ v, (int) adj[v].size(), cap, cost, 0 });
		adj[v].push_back({ u, (int) adj[u].size() - 1, 0, -cost, 0 });
	}

	pair<T, T> calc(int s, int t) {
		spfa(s); /* comment out if all costs are non-negative */
		T totalflow = 0, totalcost = 0;
		T fcost = dist[t];
		while (true) {
			reweight();
			dijkstra(s);
			if (~pre[t]) {
				fcost += dist[t];
				T flow = INF;
				for (int v = t; ~pre[v]; v = adj[v][pre[v]].t) {
					edge& r = adj[v][pre[v]];
					edge& e = adj[r.t][r.rev];
					flow = min(flow, e.cap - e.f);
				}
				for (int v = t; ~pre[v]; v = adj[v][pre[v]].t) {
					edge& r = adj[v][pre[v]];
					edge& e = adj[r.t][r.rev];
					e.f += flow;
					r.f -= flow;
				}
				totalflow += flow;
				totalcost += flow * fcost;
			}
			else break;
		}
		return { totalflow, totalcost };
	}

	void clear() {
		for (int i = 0; i < V; i++) {
			adj[i].clear();
			dist[i] = pre[i] = vis[i] = 0;
		}
	}
};

int main() {
	/* Example of usage */
	mcmf<4> network;
	network.add(0, 1, 75, 15);
	network.add(0, 2, 50, 30);
	network.add(1, 2, 40, 5);
	network.add(1, 3, 50, 10);
	network.add(2, 3, 30, 20);

	int flow, cost;
	tie(flow, cost) = network.calc(0, 3);

	/* Max-flow should be 80, and min-cost should be 2500. */
	cout << flow << ' ' << cost << endl;
	return 0;
}
