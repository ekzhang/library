// lct.cpp
// Eric K. Zhang; Nov. 22, 2017

#include <bits/stdc++.h>
using namespace std;

#define MAXN 100013

// Link-cut node
struct node {
	node* p;
	node* c[2];
	int x, s;
	bool flip;

	bool r() { return !p || (p->c[0] != this && p->c[1] != this); }
	int d() { return r() ? -1 : p->c[1] == this; }

	void update() {
		s = x + (c[0] ? c[0]->s : 0) + (c[1] ? c[1]->s : 0);
	}

	void push() {
		if (flip) {
			swap(c[0], c[1]);
			if (c[0]) c[0]->flip ^= 1;
			if (c[1]) c[1]->flip ^= 1;
			flip = false;
		}
	}

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
			p->p->push();
			p->push();
			push();
			if (d() == p->d()) p->rot();
			else rot();
			rot();
		}
		if (!r()) {
			p->push();
			push();
			rot();
		}
		push();
	}

	void expose() {
		node* pre = nullptr;
		for (node* v = this; v; v = v->p) {
			v->splay();
			v->c[1] = pre;
			v->update();
			pre = v;
		}
		splay();
	}

	void make_root() {
		expose();
		flip ^= 1;
	}

} verts[MAXN];

inline node* n(int i) { return verts + i; }

node* find_root(node* x) {
	x->expose();
	while (x->c[0]) x = x->c[0];
	x->splay();
	return x;
}

bool connected(node* x, node* y) {
	return find_root(x) == find_root(y);
}

void link(node* x, node* y) {
	x->make_root();
	x->p = y;
}

void upd(node* x, int amt) {
	x->splay();
	x->x = amt;
	x->update();
}

int path_aggregate(node* x, node* y) {
	x->make_root();
	y->expose();
	return y->s;
}
