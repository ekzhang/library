// li_chao_dynamic.cpp
// Eric K. Zhang; Jul. 3, 2018

#include <bits/stdc++.h>
using namespace std;

typedef long long LL;

#define MAXLC 1000000000
#define INF (1LL<<60)

inline LL f(LL m, LL b, int x) {
	return m * x + b;
}

struct lc_node {
	LL m = 0, b = INF;
	lc_node *l = nullptr, *r = nullptr;

	~lc_node() { delete l; delete r; }

	void add_line(LL nm, LL nb, int lo=0, int hi=MAXLC) {
		int mid = (lo + hi) / 2;
		bool bl = f(nm, nb, lo) < f(m, b, lo);
		bool bm = f(nm, nb, mid) < f(m, b, mid);
		bool bh = f(nm, nb, hi) < f(m, b, hi);
		if (bm) {
			swap(nm, m);
			swap(nb, b);
		}
		if (lo == hi || nb == INF)
			return;
		else if (bl != bm) {
			if (!l) l = new lc_node;
			l->add_line(nm, nb, lo, mid - 1);
		}
		else if (bh != bm) {
			if (!r) r = new lc_node;
			r->add_line(nm, nb, mid + 1, hi);
		}
	}

	LL get(int x, int lo=0, int hi=MAXLC) {
		int mid = (lo + hi) / 2;
		LL ret = f(m, b, x);
		if (l && x < mid)
			ret = min(ret, l->get(x, lo, mid - 1));
		if (r && x > mid)
			ret = min(ret, r->get(x, mid + 1, hi));
		return ret;
	}

	void clear() {
		delete l; delete r;
		m = 0, b = INF, l = nullptr, r = nullptr;
	}

} lc;

int main() {
	lc.add_line(2, -1); // y = 2x - 1
	lc.add_line(-1, 9); // y = 9 - x

	cout << lc.get(0) << " = " << -1 << endl;
	cout << lc.get(1) << " = " << 1 << endl;
	cout << lc.get(3) << " = " << 5 << endl;
	cout << lc.get(4) << " = " << 5 << endl;
	cout << lc.get(5) << " = " << 4 << endl;
	cout << lc.get(100) << " = " << -91 << endl;
}
