// splay_tree.cpp
// Eric K. Zhang; Nov. 22, 2017

#include <bits/stdc++.h>
using namespace std;

struct node {
	node* p;
	node* c[2];
	int x, s;

	bool r() { return !p; }
	int d() { return r() ? -1 : p->c[1] == this; }
	void update() { s = 1 + (c[0] ? c[0]->s : 0) + (c[1] ? c[1]->s : 0); }

	static void connect(node* pa, node* ch, int dir) {
		if (ch) ch->p = pa;
		if (dir != -1) pa->c[dir] = ch;
	}

	void rot() {
		assert(!r());

		int x = d();
		node* pa = p;

		connect(pa->p, this, pa->d());
		connect(pa, c[!x], x);
		connect(this, pa, !x);

		pa->update();
		update();
	}

	void splay() {
		while (!r() && !p->r()) {
			if (d() == p->d()) p->rot();
			else rot();
			rot();
		}
		if (!r())
			rot();
	}
} verts[100013];
int t = 0;

inline node* n(int i) { return verts + i; }

node* find_by_order(node* n, int k) {
	// Note: finds, but DOES NOT SPLAY!
	n->push();
	int ls = n->c[0] ? n->c[0]->s : 0;
	if (k < ls) return find_by_order(n->c[0], k);
	if (k == ls) return n;
	return find_by_order(n->c[1], k - ls - 1);
}

pair<node*, node*> split(node* n, int idx) {
	// split into [0..idx) and [idx..end)
	n = find_by_order(n, idx);
	n->splay();
	node* v = n->c[0];
	if (v) v->p = nullptr;
	n->c[0] = nullptr;
	n->update();
	return {v, n};
}

node* merge(node* left, node* right) {
	if (!right) return left;
	right = find_by_order(right, 0);
	right->splay();
	right->c[0] = left;
	if (left) left->p = right;
	right->update();
	return right;
}

bool find_splay(node*& root, int value) {
	node* c;
	node* nex = root;
	do {
		c = nex;
		if (c->x > value) nex = c->c[0];
		else if (c->x < value) nex = c->c[1];
		else break;
	} while (nex);
	c->splay();
	root = c;
	return (root->x == value);
}

int query_splay(node*& root, int x) {
	// returns number of elements <= x
	if (!root) return 0;
	find_splay(root, x);
	int ret = root->c[0] ? root->c[0]->s : 0;
	if (root->x <= x) {
		ret++;
	}
	return ret;
}

int query_splay(node*& root, int x, int y) {
	// return in range [x .. y]
	int ans = query_splay(root, y);
	ans -= query_splay(root, x - 1);
	return ans;
}

void insert_splay(node*& root, int value) {
	if (!root) {
		root = n(t++);
		root->x = value;
		return;
	}

	node* c;
	node* nex = root;
	int d = -1;
	do {
		c = nex;
		if (c->x > value) {
			nex = c->c[0];
			d = 0;
		}
		else {
			nex = c->c[1];
			d = 1;
		}
	} while (nex);

	nex = n(t++);
	nex->x = value;
	nex->p = c;
	c->c[d] = nex;
	while (c) {
		c->s++;
		c = c->p;
	}
	nex->splay();
	root = nex;
}

bool delete_splay(node*& root, int value) {
	if (!find_splay(root, value)) {
		return false;
	}

	int numchildren = ((bool) root->c[0]) + ((bool) root->c[1]);
	if (!numchildren) {
		root = nullptr;
		return true;
	}

	if (numchildren == 1) {
		root = root->c[0] ? root->c[0] : root->c[1];
		root->p = nullptr;
		return true;
	}

	node* left = root->c[0];
	node* right = root->c[1];
	left->p = right->p = nullptr;

	while (right->c[0]) {
		right = right->c[0];
	}
	right->splay();
	root = right;
	root->c[0] = left;
	left->p = root;
	return true;
}

void inorder_splay(node* root, vector<int>& result) {
	if (!root) return;
	inorder_splay(root->c[0], result);
	result.push_back(root->x);
	inorder_splay(root->c[1], result);
}

#define MAXN 100013
int N;
node* tree;
int A[MAXN];

int main() {
	// naive treesort test
	// sort N numbers, delete minimum
	int N = 10;
	srand(0);
	for (int i = 0; i < N; i++) {
		A[i] = rand();
		insert_splay(tree, A[i]);
		cout << tree->x << endl;
	}

	sort(A, A + N);
	for (int i = 0; i < N; i++) {
		vector<int> ans;
		inorder_splay(tree, ans);
		for (int x : ans) {
			cout << x << ' ';
		}
		cout << endl;
		delete_splay(tree, A[i]);
	}

	return 0;
}
