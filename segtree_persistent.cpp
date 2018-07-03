// segtree_persistent.cpp
// Eric K. Zhang; Nov. 22, 2017

#include <bits/stdc++.h>
using namespace std;

#define MAXN 100013
#define MAXLGN 18
#define MAXSEG (2 * MAXN * MAXLGN)
int N;
struct node {
	node *l, *r;
	int x;
} vals[MAXSEG]; int t = 0;
node* tree[MAXN];

node* build_tree(int lo=0, int hi=-1) {
	if (hi == -1) hi = N - 1;
	node* cur = &vals[t++];
	if (lo != hi) {
		int mid = (lo + hi) / 2;
		cur->l = build_tree(lo, mid);
		cur->r = build_tree(mid + 1, hi);
	}
	return cur;
}

node* update(node* n, int i, int x, int lo=0, int hi=-1) {
	if (hi == -1) hi = N - 1;
	if (hi < i || lo > i) return n;
	node* v = &vals[t++];
	if (lo == hi) { v->x = n->x + x; return v; }
	int mid = (lo + hi) / 2;
	v->l = update(n->l, i, x, lo, mid);
	v->r = update(n->r, i, x, mid + 1, hi);
	v->x = v->l->x + v->r->x;
	return v;
}

int query(node* n, int s, int e, int lo=0, int hi=-1) {
	if (hi == -1) hi = N - 1;
	if (hi < s || lo > e) return 0;
	if (lo >= s && hi <= e) return n->x;
	int mid = (lo + hi) / 2;
	return query(n->l, s, e, lo, mid) + 
			query(n->r, s, e, mid + 1, hi);
}

int main() {
	N = 100;
	tree[0] = build_tree();
	for (int i = 0; i < N; i++) {
		tree[i + 1] = update(tree[i], i, 1);
	}
	// how many of the numbers in indices [0..4] are in the range [3..5]?
	cout << query(tree[5], 3, 5) << endl;
}
