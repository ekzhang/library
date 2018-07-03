// convex_hull_trick.cpp
// Eric K. Zhang; Nov. 22, 2017

#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define MAXN 100013
int N, Q;
pii ar[MAXN];

vector<pii> envelope;
vector<double> hull;

double intersect(int m1, int b1, int m2, int b2) {
	return (b2 - b1) / ((double) m1 - m2);
}

double intersect(pii y1, pii y2) {
	return intersect(y1.first, y1.second, y2.first, y2.second);
}

int main() {
	// make cin, cout faster
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	freopen("cht.in", "r", stdin);

	cin >> N >> Q;
	for (int i = 0; i < N; i++) {
		cin >> ar[i].first >> ar[i].second;
	}

	// Ok, time for the CHT algorithm! We'll find the lower envelope
	sort(ar, ar + N, [](pii y1, pii y2) {
		return y1.first != y2.first ? y1.first > y2.first : y1.second < y2.second;
	});

	for (int i = 0; i < N; i++) {
		if (i != 0 && ar[i].first == ar[i - 1].first)
			continue; // repeated slope <-> parallel lines

		int sz;
		while ((sz = envelope.size()) >= 2) {
			if (intersect(envelope[sz - 2], ar[i]) <
					intersect(envelope[sz - 2], envelope[sz - 1])) {
				envelope.pop_back();
			}
			else break;
		}

		envelope.push_back(ar[i]);
	}

	for (int i = 0; i < envelope.size() - 1; i++) {
		hull.push_back(intersect(envelope[i], envelope[i + 1]));
	}

	for (int i = 0; i < Q; i++) {
		double x;
		cin >> x;
		int idx = lower_bound(begin(hull), end(hull), x) - begin(hull);
		cout << envelope[idx].first * x + envelope[idx].second << '\n';
	}

	cout.flush();
	return 0;
}
