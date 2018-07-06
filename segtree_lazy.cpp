// segtree_lazy.cpp
// Eric K. Zhang; Nov. 22, 2017

#include <bits/stdc++.h>
using namespace std;

#define MAXSEG 262144

int N, Q;
int st[MAXSEG];
int lazy[MAXSEG];

void push(int lo, int hi, int node) {
	if (lazy[node] == 0) return;
	st[node] += (hi - lo + 1) * lazy[node];
	if (lo != hi) {
		lazy[2 * node + 1] += lazy[node];
		lazy[2 * node + 2] += lazy[node];
	}
	lazy[node] = 0;
}

void update_range(int s, int e, int x, int lo=0, int hi=-1, int node=0) {
	if (hi == -1) hi = N - 1;
	push(lo, hi, node);
	if (hi < s || lo > e) return;
	if (lo >= s && hi <= e) {
		lazy[node] = x;
		push(lo, hi, node);
		return;
	}
	int mid = (lo + hi) / 2;
	update_range(s, e, x, lo, mid, 2 * node + 1);
	update_range(s, e, x, mid + 1, hi, 2 * node + 2);
	st[node] = st[2 * node + 1] + st[2 * node + 2];
}

int query(int s, int e, int lo=0, int hi=-1, int node=0) {
	if (hi == -1) hi = N - 1;
	push(lo, hi, node);
	if (hi < s || lo > e) return 0;
	if (lo >= s && hi <= e) return st[node];
	int mid = (lo + hi) / 2;
	return query(s, e, lo, mid, 2 * node + 1) + 
			query(s, e, mid + 1, hi, 2 * node + 2);
}

int main() {
	freopen("segtree-lazy.in", "r", stdin);

	cin >> N >> Q;
	for (int i = 0; i < N; i++) {
		int a;
		cin >> a;
		update_range(i, i, a); // single-update
	}
	for (int i = 0; i < Q; i++) {
		int T;
		cin >> T;
		if (T == 0) {
			// update
			int x, y, z;
			cin >> x >> y >> z;
			update_range(x - 1, y - 1, z);
		}
		else {
			int x, y;
			cin >> x >> y;
			cout << query(x - 1, y - 1) << '\n';
		}
	}
	cout.flush();
	return 0;
}
