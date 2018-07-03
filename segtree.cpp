// segtree.cpp
// Eric K. Zhang; Nov. 22, 2017

#include <bits/stdc++.h>
using namespace std;

#define MAXSEG 262144

int N;
int st[MAXSEG];

void update(int i, int x, int lo=0, int hi=-1, int node=0) {
	if (hi == -1) hi = N - 1;
	if (hi < i || lo > i) return;
	if (lo == hi) { st[node] += x; return; }
	int mid = (lo + hi) / 2;
	update(i, x, lo, mid, 2 * node + 1);
	update(i, x, mid + 1, hi, 2 * node + 2);
	st[node] = st[2 * node + 1] + st[2 * node + 2];
}

int query(int s, int e, int lo=0, int hi=-1, int node=0) {
	if (hi == -1) hi = N - 1;
	if (hi < s || lo > e) return 0;
	if (lo >= s && hi <= e) return st[node];
	int mid = (lo + hi) / 2;
	return query(s, e, lo, mid, 2 * node + 1) + 
			query(s, e, mid + 1, hi, 2 * node + 2);
}


int main() {
	N = 100;
	for (int i = 0; i < N; i++) {
		update(i, i + 1); // add i+1 to the i-th number (initially 0)
	}
	cout << query(0, N - 1) << endl;
}
