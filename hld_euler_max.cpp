// hld_euler_max.cpp
// Eric K. Zhang; Nov. 22, 2017

#include <bits/stdc++.h>
using namespace std;

typedef long long LL;

#define MAXN 100005
#define MAXSEG 262144
int N;
vector<int> adj[MAXN];

namespace hld {
	int parent[MAXN];
	vector<int> ch[MAXN];
	int depth[MAXN];
	int sz[MAXN];
	int in[MAXN];
	int rin[MAXN];
	int nxt[MAXN];
	int out[MAXN];
	int t = 0;

	void dfs_sz(int n=0, int p=-1, int d=0) {
		parent[n] = p, sz[n] = 1;
		depth[n] = d;
		for (auto v : adj[n]) if (v != p) {
			dfs_sz(v, n, d + 1);
			sz[n] += sz[v];
			ch[n].push_back(v);
			if (sz[v] > sz[ch[n][0]])
				swap(ch[n][0], ch[n].back());
		}
	}

	void dfs_hld(int n=0) {
		in[n] = t++;
		rin[in[n]] = n;
		for (auto v : ch[n]) {
			nxt[v] = (v == ch[n][0] ? nxt[n] : v);
			dfs_hld(v);
		}
		out[n] = t;
	}

	void init() {
		dfs_sz();
		dfs_hld();
	}

	int lca(int u, int v) {
		while (nxt[u] != nxt[v]) {
			if (depth[nxt[u]] < depth[nxt[v]]) swap(u, v);
			u = parent[nxt[u]];
		}
		return depth[u] < depth[v] ? u : v;
	}

	LL st[MAXSEG];
	LL lazy[MAXSEG];

	void push(int node, int lo, int hi) {
		if (lazy[node] == 0) return;
		st[node] += lazy[node];
		if (lo != hi) {
			lazy[2 * node + 1] += lazy[node];
			lazy[2 * node + 2] += lazy[node];
		}
		lazy[node] = 0;
	}

	void update_range(int s, int e, int x, int lo=0, int hi=-1, int node=0) {
		if (hi == -1) hi = N - 1;
		push(node, lo, hi);
		if (hi < s || lo > e) return;
		if (lo >= s && hi <= e) {
			lazy[node] = x;
			push(node, lo, hi);
			return;
		}
		int mid = (lo + hi) / 2;
		update_range(s, e, x, lo, mid, 2 * node + 1);
		update_range(s, e, x, mid + 1, hi, 2 * node + 2);
		st[node] = max(st[2 * node + 1], st[2 * node + 2]);
	}

	LL query(int s, int e, int lo=0, int hi=-1, int node=0) {
		if (hi == -1) hi = N - 1;
		push(node, lo, hi);
		if (hi < s || lo > e) return 0;
		if (lo >= s && hi <= e) return st[node];
		int mid = (lo + hi) / 2;
		return max(query(s, e, lo, mid, 2 * node + 1),
				query(s, e, mid + 1, hi, 2 * node + 2));
	}

	void update_subtree(int n, int x) {
		update_range(in[n], out[n] - 1, x);
	}

	LL query_subtree(int n) {
		return query(in[n], out[n] - 1);
	}

	void update_path(int u, int v, int x, bool ignore_lca=false) {
		while (nxt[u] != nxt[v]) {
			if (depth[nxt[u]] < depth[nxt[v]]) swap(u, v);
			update_range(in[nxt[u]], in[u], x);
			u = parent[nxt[u]];
		}
		if (depth[u] < depth[v]) swap(u, v);
		update_range(in[v] + ignore_lca, in[u], x);
	}

	LL query_path(int u, int v, bool ignore_lca=false) {
		LL ret = 0;
		while (nxt[u] != nxt[v]) {
			if (depth[nxt[u]] < depth[nxt[v]]) swap(u, v);
			ret = max(ret, query(in[nxt[u]], in[u]));
			u = parent[nxt[u]];
		}
		if (depth[u] < depth[v]) swap(u, v);
		ret = max(ret, query(in[v] + ignore_lca, in[u]));
		return ret;
	}
}

int main() {
	// http://usaco.org/index.php?page=viewproblem2&cpid=576

	ios_base::sync_with_stdio(false);
	cin.tie(0);

	freopen("maxflow.in", "r", stdin);
	freopen("maxflow.out", "w", stdout);

	int K;
	cin >> N >> K;
	for (int i = 1; i < N; i++) {
		int A, B;
		cin >> A >> B;
		A--; B--;
		adj[A].push_back(B);
		adj[B].push_back(A);
	}

	hld::init();

	for (int i = 0; i < K; i++) {
		int s, t;
		cin >> s >> t;
		--s; --t;
		hld::update_path(s, t, 1);
	}

	cout << hld::query_subtree(0) << endl;
	return 0;
}
