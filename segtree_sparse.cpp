// segtree_sparse.cpp
// Eric K. Zhang; Dec. 31, 2017

#include <bits/stdc++.h>
using namespace std;

typedef long long LL;
#define MAXN 100013
#define MAXLGN 18
#define MAXSEG 262144
#define MAXSEG2 (2 * MAXN * MAXLGN * MAXLGN)

int N;
struct node {
	node *l, *r;
	LL x;
} vals[MAXSEG2]; int t = 0;
node* st[MAXSEG];

void update2(node*& n, int i, int x, int lo=0, int hi=-1) {
	if (hi == -1) hi = N - 1;
	if (hi < i || lo > i) return;
	if (!n) n = &vals[t++];
	if (lo == hi) {
		n->x += x;
		return;
	}
	int mid = (lo + hi) / 2;
	if (i <= mid) update2(n->l, i, x, lo, mid);
	else update2(n->r, i, x, mid + 1, hi);
	n->x += x;
}

void update(int i, int j, int x, int lo=0, int hi=-1, int node=0) {
	if (hi == -1) hi = N - 1;
	if (hi < i || lo > i) return;
	if (lo == hi) {
		update2(st[node], j, x);
		return;
	}
	int mid = (lo + hi) / 2;
	update(i, j, x, lo, mid, 2 * node + 1);
	update(i, j, x, mid + 1, hi, 2 * node + 2);
	update2(st[node], j, x);
}

LL query2(node* n, int s, int e, int lo=0, int hi=-1) {
	if (hi == -1) hi = N - 1;
	if (hi < s || lo > e || !n) return 0;
	if (s <= lo && hi <= e) return n->x;
	int mid = (lo + hi) / 2;
	return query2(n->l, s, e, lo, mid) + query2(n->r, s, e, mid + 1, hi);
}

LL query(int s, int e, int s2, int e2, int lo=0, int hi=-1, int node=0) {
	if (hi == -1) hi = N - 1;
	if (hi < s || lo > e) return 0;
	if (s <= lo && hi <= e) return query2(st[node], s2, e2);
	int mid = (lo + hi) / 2;
	return query(s, e, s2, e2, lo, mid, 2 * node + 1)
			+ query(s, e, s2, e2, mid + 1, hi, 2 * node + 2);
}

int main() {
	N = 100;
	update(0, 0, 5);
	update(2, 2, 30);
	update(3, 1, 10);
	update(0, 2, 10);
	cout << query(1, 3, 0, 4) << endl;
}
