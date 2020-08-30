// kinetic_tournament.cpp
// Eric K. Zhang; Aug. 29, 2020
//
// This is an implementation of a _kinetic tournament_, which I originally
// learned about from Daniel Zhang in this Codeforces blog comment:
// https://codeforces.com/blog/entry/68534#comment-530381
//
// The functionality of the data structure is a mix between a line container,
// i.e., "convex hull trick", and a segment tree.
//
// Suppose that you have an array containing pairs of nonnegative integers,
// A[i] and B[i]. You also have a global parameter T, corresponding to the
// "temperature" of the data structure. Your goal is to support the following
// queries on this data:
//
//   - update(i, a, b): set A[i] = a and B[i] = b
//   - query(s, e): return min{s <= i <= e} A[i] * T + B[i]
//   - heaten(new_temp): set T = new_temp
//       [precondition: new_temp >= current value of T]
//
// (For simplicity, we set A[i] = 0 and B[i] = LLONG_MAX for uninitialized
// entries, which should not change the query results.)
//
// This allows you to essentially do arbitrary lower convex hull queries on a
// collection of lines, as well as any contiguous subcollection of those lines.
// This is more powerful than standard convex hull tricks and related data
// structures (Li-Chao Segment Tree) for three reasons:
//
//   - You can arbitrarily remove/edit lines, not just add them.
//   - Dynamic access to any subinterval of lines, which lets you avoid costly
//     merge small-to-large operations in some cases.
//   - Easy to reason about and implement from scratch, unlike dynamic CHT.
//
// The tradeoff is that you can only query sequential values (temperature is
// only allowed to increase) for amortization reasons, but this happens to be
// a fairly common case in many problems.
//
// Time complexity:
//
//   - query: O(log n)
//   - update: O(log n)
//   - heaten: O(log^2 n)  [amortized]
//
// Verification: FBHC 2020, Round 2, Problem D "Log Drivin' Hirin'"

#include <bits/stdc++.h>
using namespace std;

template <typename T = int64_t>
class kinetic_tournament {
	const T INF = numeric_limits<T>::max();
	typedef pair<T, T> line;

	size_t n;         // size of the underlying array
	T temp;           // current temperature
	vector<line> st;  // tournament tree
	vector<T> melt;   // melting temperature of each subtree

	inline T eval(const line& ln, T t) {
		return ln.first * t + ln.second;
	}

	inline bool cmp(const line& line1, const line& line2) {
		auto x = eval(line1, temp);
		auto y = eval(line2, temp);
		if (x != y) return x < y;
		return line1.first < line2.first;
	}

	T next_isect(const line& line1, const line& line2) {
		if (line1.first > line2.first) {
			T delta = eval(line2, temp) - eval(line1, temp);
			T delta_slope = line1.first - line2.first;
			assert(delta > 0);
			T mint = temp + (delta - 1) / delta_slope + 1;
			return mint > temp ? mint : INF;  // prevent overflow
		}
		return INF;
	}

	void recompute(size_t lo, size_t hi, size_t node) {
		if (lo == hi || melt[node] > temp) return;

		size_t mid = (lo + hi) / 2;
		recompute(lo, mid, 2 * node + 1);
		recompute(mid + 1, hi, 2 * node + 2);

		auto line1 = st[2 * node + 1];
		auto line2 = st[2 * node + 2];
		if (!cmp(line1, line2))
			swap(line1, line2);
		st[node] = line1;

		melt[node] = min(melt[2 * node + 1], melt[2 * node + 2]);
		if (line1 != line2) {
			T t = next_isect(line1, line2);
			assert(t > temp);
			melt[node] = min(melt[node], t);
		}
	}

	void update(size_t i, T a, T b, size_t lo, size_t hi, size_t node) {
		if (i < lo || i > hi) return;
		if (lo == hi) {
			st[node] = {a, b};
			return;
		}
		size_t mid = (lo + hi) / 2;
		update(i, a, b, lo, mid, 2 * node + 1);
		update(i, a, b, mid + 1, hi, 2 * node + 2);
		melt[node] = 0;
		recompute(lo, hi, node);
	}

	T query(size_t s, size_t e, size_t lo, size_t hi, size_t node) {
		if (hi < s || lo > e) return INF;
		if (s <= lo && hi <= e) return eval(st[node], temp);
		size_t mid = (lo + hi) / 2;
		return min(query(s, e, lo, mid, 2 * node + 1),
			query(s, e, mid + 1, hi, 2 * node + 2));
	}

public:
	// Constructor for a kinetic tournament, takes in the size n of the
	// underlying arrays a[..], b[..] as input.
	kinetic_tournament(size_t size) : n(size), temp(0) {
		assert(size > 0);
		size_t seg_size = ((size_t) 2) << (64 - __builtin_clzll(n - 1));
		st.resize(seg_size, {0, INF});
		melt.resize(seg_size, INF);
	}

	// Sets A[i] = a, B[i] = b.
	void update(size_t i, T a, T b) {
		update(i, a, b, 0, n - 1, 0);
	}

	// Returns min{s <= i <= e} A[i] * T + B[i].
	T query(size_t s, size_t e) {
		return query(s, e, 0, n - 1, 0);
	}

	// Increases the internal temperature to new_temp.
	void heaten(T new_temp) {
		assert(new_temp >= temp);
		temp = new_temp;
		recompute(0, n - 1, 0);
	}
};
