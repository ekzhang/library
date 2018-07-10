// treap.cpp
// Eric K. Zhang; Nov. 22, 2017

#include <bits/stdc++.h>
using namespace std;

struct tnode {
	tnode *l, *r;
	int y = rand() ^ (rand() << 16);

	/* Data */
	int x;

	/* Aggregate values */
	int sz;

	tnode() { update(); }
	
	void update() {
		sz = 1 + (l ? l->sz : 0) + (r ? r->sz : 0);
	}

	void set(int x) { this->x = x; update(); }

} vals[100013];
int mem = 0;

void split(tnode* t, tnode*& l, tnode*& r, int k) {
	if (!t) { l = r = nullptr; return; }
	if (t->x < k)
		l = t, split(t->r, t->r, r, k);
	else
		r = t, split(t->l, l, t->l, k);
	t->update();
}

void split_at(tnode* t, tnode*& l, tnode*& r, int idx) {
	if (!t) { l = r = nullptr; return; }
	int tidx = t->l ? t->l->sz : 0;
	if (tidx < idx)
		l = t, split_at(t->r, t->r, r, idx - tidx - 1);
	else
		r = t, split_at(t->l, l, t->l, idx);
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

void insert(tnode*& t, int x) {
	tnode *l, *r;
	split(t, l, t, x);
	split(t, t, r, x + 1);
	t = &vals[mem++];
	t->set(x);
	merge(t, l, t);
	merge(t, t, r);
}

void remove(tnode*& t, int x) {
	tnode *l, *r;
	split(t, l, t, x);
	split(t, t, r, x + 1);
	merge(t, l, r);
}

tnode* find_by_order(tnode* t, int k) {
	int ls = t->l ? t->l->sz : 0;
	if (k < ls) return find_by_order(t->l, k);
	if (k == ls) return t;
	return find_by_order(t->r, k - 1 - ls);
}

void inorder(tnode* t, vector<int>& v) {
	if (!t) return;
	inorder(t->l, v);
	v.push_back(t->x);
	inorder(t->r, v);
}

string to_string(tnode* t) {
	vector<int> v;
	inorder(t, v);
	string ret = "[";
	for (int i = 0; i < v.size(); i++) {
		if (i) ret += ", ";
		ret += to_string(v[i]);
	}
	ret += "]";
	return ret;
}

int main() {
	tnode* t = nullptr;
	for (int i = 0; i < 20; i++) {
		vals[mem].x = i * i;
		merge(t, t, &vals[mem++]);
	}

	cout << "Original treap" << endl;
	cout << "t: " << to_string(t) << endl;
	cout << endl;

	tnode *l, *r;
	split(t, l, r, 81);

	cout << "After split at 81" << endl;
	cout << "l: " << to_string(l) << endl;
	cout << "r: " << to_string(r) << endl;
	cout << endl;

	merge(t, l, r);
	split_at(t, l, r, 5);

	cout << "After split at index 5" << endl;
	cout << "l: " << to_string(l) << endl;
	cout << "r: " << to_string(r) << endl;
	cout << endl;

	cout << "Testing find_by_order" << endl;
	for (int i = 0; i < l->sz; i++)
		cout << "Order " << i << " in l: " << find_by_order(l, i)->x << endl;
	cout << endl;
	for (int i = 0; i < r->sz; i++)
		cout << "Order " << i << " in r: " << find_by_order(r, i)->x << endl;
	cout << endl;

	merge(t, l, r);

	for (int i = 7; i <= 9; i++) {
		insert(t, i);
		cout << "After inserting " << i << endl;
		cout << "t: " << to_string(t) << endl;
		cout << endl;
	}

	remove(t, 121);
	cout << "After removing 121" << endl;
	cout << "t: " << to_string(t) << endl;
	cout << endl;

	return 0;
}
