// rectangle_union.cpp
// Eric K. Zhang; Jul. 6, 2018

#include <bits/stdc++.h>
using namespace std;

typedef long long LL;

struct rect {
	int x1, y1, x2, y2;
};

class footprint_segtree {
	const int N;
	const vector<int>& weights;
	vector<int> mi, cnt, lazy;
	int total;

	void init(int lo, int hi, int node) {
		if (lo == hi) {
			cnt[node] = weights[lo];
			total += cnt[node];
			return;
		}
		int mid = (lo + hi) / 2;
		init(lo, mid, 2 * node + 1);
		init(mid + 1, hi, 2 * node + 2);
		cnt[node] = cnt[2 * node + 1] + cnt[2 * node + 2];
	}

	void push(int lo, int hi, int node) {
		if (lazy[node]) {
			mi[node] += lazy[node];
			if (lo != hi) {
				lazy[2 * node + 1] += lazy[node];
				lazy[2 * node + 2] += lazy[node];
			}
			lazy[node] = 0;
		}
	}

	void update_range(int s, int e, int x, int lo, int hi, int node) {
		push(lo, hi, node);
		if (lo > e || hi < s)
			return;
		if (s <= lo && hi <= e) {
			lazy[node] = x;
			push(lo, hi, node);
			return;
		}
		int mid = (lo + hi) / 2;
		update_range(s, e, x, lo, mid, 2 * node + 1);
		update_range(s, e, x, mid + 1, hi, 2 * node + 2);

		mi[node] = min(mi[2 * node + 1], mi[2 * node + 2]);
		cnt[node] = 0;
		if (mi[2 * node + 1] == mi[node])
			cnt[node] += cnt[2 * node + 1];
		if (mi[2 * node + 2] == mi[node])
			cnt[node] += cnt[2 * node + 2];
	}

public:
	footprint_segtree(const vector<int>& weights)
		: N(weights.size()), weights(weights) {
		mi.resize(4 * N);
		cnt.resize(4 * N);
		lazy.resize(4 * N);
		total = 0;
		init(0, N - 1, 0);
	}

	void update_range(int s, int e, int x) {
		update_range(s, e, x, 0, N - 1, 0);
	}

	int query() {
		return total - (mi[0] ? 0 : cnt[0]);
	}
};

LL rectangle_union(const vector<rect>& rects) {
	// Coordinate Compression
	vector<int> ys;
	for (const rect& r : rects) {
		ys.push_back(r.y1);
		ys.push_back(r.y2);
	}
	sort(ys.begin(), ys.end());
	ys.resize(unique(ys.begin(), ys.end()) - ys.begin());

	vector<int> lengths(ys.size() - 1);
	for (int i = 0; i + 1 < ys.size(); i++)
		lengths[i] = ys[i + 1] - ys[i];
	footprint_segtree st(lengths);

	// Sweepline Preparation
	vector<pair<int, pair<int, int> > > events;
	for (int i = 0; i < rects.size(); i++) {
		const rect& r = rects[i];
		events.push_back({ r.x1, { i, 1 } });
		events.push_back({ r.x2, { i, -1 } });
	}
	sort(events.begin(), events.end());

	// Sweepline
	int pre = INT_MIN;
	LL ret = 0;
	for (auto& e : events) {
		ret += (LL) st.query() * (e.first - pre);
		pre = e.first;

		const rect& r = rects[e.second.first];
		int change = e.second.second;
		int y1 = lower_bound(ys.begin(), ys.end(), r.y1) - ys.begin();
		int y2 = lower_bound(ys.begin(), ys.end(), r.y2) - ys.begin();
		st.update_range(y1, y2 - 1, change);
	}

	return ret;
}

int main() {
	int N;
	cin >> N;
	vector<rect> rects;
	for (int i = 0; i < N; i++) {
		int x1, y1, x2, y2;
		cin >> x1 >> y1 >> x2 >> y2;
		rects.push_back(rect { x1, y1, x2, y2 });
	}

	cout << rectangle_union(rects) << endl;
	return 0;
}
