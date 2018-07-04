// li_chao.cpp
// Eric K. Zhang; Jul. 3, 2018

#include <bits/stdc++.h>
using namespace std;

typedef long long LL;
typedef pair<LL, LL> pll;
inline LL f(pll a, int x) {
	return a.first * x + a.second;
}

#define MAXLC 1000000
#define INF (1LL<<60)
pll line[MAXLC << 1];

void lc_init(int lo=0, int hi=MAXLC, int node=0) {
	if (lo > hi || line[node].second == INF)
		return;
	line[node] = { 0, INF };
	int mid = (lo + hi) / 2;
	lc_init(lo, mid - 1, 2 * node + 1);
	lc_init(mid + 1, hi, 2 * node + 2);
}

void add_line(pll ln, int lo=0, int hi=MAXLC, int node=0) {
	int mid = (lo + hi) / 2;
	bool l = f(ln, lo) < f(line[node], lo);
	bool m = f(ln, mid) < f(line[node], mid);
	bool h = f(ln, hi) < f(line[node], hi);
	if (m) swap(line[node], ln);
	if (lo == hi || ln.second == INF)
		return;
	else if (l != m)
		add_line(ln, lo, mid - 1, 2 * node + 1);
	else if (h != m)
		add_line(ln, mid + 1, hi, 2 * node + 2);
}

LL get(int x, int lo=0, int hi=MAXLC, int node=0) {
	int mid = (lo + hi) / 2;
	LL ret = f(line[node], x);
	if (x < mid)
		ret = min(ret, get(x, lo, mid - 1, 2 * node + 1));
	if (x > mid)
		ret = min(ret, get(x, mid + 1, hi, 2 * node + 2));
	return ret;
}

int main() {
	lc_init();
	add_line({ 2, -1 }); // y = 2x - 1
	add_line({ -1, 9 }); // y = 9 - x

	cout << get(0) << " = " << -1 << endl;
	cout << get(1) << " = " << 1 << endl;
	cout << get(3) << " = " << 5 << endl;
	cout << get(4) << " = " << 5 << endl;
	cout << get(5) << " = " << 4 << endl;
	cout << get(100) << " = " << -91 << endl;
}
