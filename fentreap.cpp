// fentreap.cpp
// Eric K. Zhang; Jan. 1, 2018

#include <bits/stdc++.h>
using namespace std;

#define MAXN 100013
#define MAXLGN 18

int N;

struct tnode {
	tnode* l;
	tnode* r;
	int x, y;
	int val, sum;
	tnode() { y = rand() ^ (rand() << 16); }
	void update() { sum = val + (l ? l->sum : 0) + (r ? r->sum : 0); }
} vals[MAXN * MAXLGN];
int mem = 0;
tnode* fen[MAXN];

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

void update(int x, int y, int val) {
	for (x++; x < MAXN; x += x & -x) {
		upd(fen[x], y, val);
	}
}

int qry(tnode*& t, int y) {
	tnode* r;
	split(t, t, r, y + 1);
	int ret = t ? t->sum : 0;
	merge(t, t, r);
	return ret;
}

int query(int x, int y) {
	int ret = 0;
	for (x++; x; x -= x & -x) {
		ret += qry(fen[x], y);
	}
	return ret;
}

int main() {
	N = 100;
	update(0, 0, 5);
	update(2, 2, 30);
	update(3, 1, 10);
	update(0, 5, 10);
	cout << query(2, 4) << endl;
}
