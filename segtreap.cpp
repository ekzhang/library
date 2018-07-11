// segtreap.cpp
// Eric K. Zhang; Jan. 1, 2018

#include <bits/stdc++.h>
using namespace std;

#define MAXN 100013
#define MAXLGN 18
#define MAXSEG 262144

int N;

struct tnode {
	tnode* l;
	tnode* r;
	int x, y;
	int val, ma;
	tnode() { y = rand() ^ (rand() << 16); }
	void update() { ma = max(val, max((l ? l->ma : 0), (r ? r->ma : 0))); }
} vals[MAXN * MAXLGN];
int mem = 0;
tnode* st[MAXSEG];

void split(tnode* t, tnode*& l, tnode*& r, int k) {
	if (!t) { l = r = nullptr; return; }
	if (t->x < k)
		l = t, split(t->r, t->r, r, k);
	else
		r = t, split(t->l, l, t->l, k);
	t->update();
}

void merge(tnode*& t, tnode* l, tnode* r) {
	if (!l || !r) { t = l ? l : r; return; }
	if (l->y < r->y)
		t = l, merge(t->r, t->r, r);
	else
		t = r, merge(t->l, l, t->l);
	t->update();
}

void upd(tnode*& t, int y, int val) {
	tnode *l, *r;
	split(t, l, t, y);
	split(t, t, r, y + 1);
	if (!t) t = &vals[mem++], t->x = y;
	t->val += val;
	t->update();
	merge(t, l, t);
	merge(t, t, r);
}

void update(int x, int y, int val, int lo=0, int hi=-1, int node=0) {
	if (hi == -1) hi = N - 1;
	if (lo > x || hi < x) return;
	upd(st[node], y, val);
	if (lo == hi) return;
	int mid = (lo + hi) / 2;
	update(x, y, val, lo, mid, 2 * node + 1);
	update(x, y, val, mid + 1, hi, 2 * node + 2);
}

int qry(tnode*& t, int y1, int y2) {
	tnode *l, *r;
	split(t, l, t, y1);
	split(t, t, r, y2 + 1);
	int ret = t ? t->ma : 0;
	merge(t, l, t);
	merge(t, t, r);
	return ret;
}

int query(int x1, int x2, int y1, int y2, int lo=0, int hi=-1, int node=0) {
	if (hi == -1) hi = N - 1;
	if (lo > x2 || hi < x1) return 0;
	if (x1 <= lo && hi <= x2) return qry(st[node], y1, y2);
	int mid = (lo + hi) / 2;
	return max(query(x1, x2, y1, y2, lo, mid, 2 * node + 1),
			query(x1, x2, y1, y2, mid + 1, hi, 2 * node + 2));
}

int main() {
	N = 100;
	update(2, 2, 5);
	update(2, 4, 30);
	update(3, 1, 10);
	update(0, 2, 20);
	cout << query(1, 3, 0, 3) << endl;
}
