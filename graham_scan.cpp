// graham_scan.cpp
// Eric K. Zhang; Nov. 22, 2017

// Reads a number N (1 <= N <= 3 * 10^5), then N pairs
// of integers (X, Y). Outputs the convex hull of these
// points in O(N log N) time, without including middle
// points that are collinear.

#include <bits/stdc++.h>
using namespace std;

typedef long long LL;
typedef pair<int, int> point;

#define MAXN 300000

int N;
point points[MAXN];

LL ccw(point a, point b, point c) {
	return (LL) (b.first - a.first) * (c.second - a.second)
			- (LL) (b.second - a.second) * (c.first - a.first);
}

vector<point> graham_scan(vector<point> points) {
	int N = points.size();
	sort(points.begin(), points.end());
	vector<point> hull(N + 1);
	int idx = 0;
	for (int i = 0; i < N; i++) {
		while (idx >= 2 && ccw(hull[idx - 2], hull[idx - 1], points[i]) >= 0)
			idx--;
		hull[idx++] = points[i];
	}
	int half = idx;
	for (int i = N - 2; i >= 0; i--) {
		while (idx > half && ccw(hull[idx - 2], hull[idx - 1], points[i]) >= 0)
			idx--;
		hull[idx++] = points[i];
	}
	idx--;
	hull.resize(idx);
	return hull;
}

int main() {
	vector<point> points = {
		{44, 140}, {67, 153}, {69, 128}, {21, 111},
		{95, 149}, {132, 119}, {103, 123}, {68, 95},
		{33, 77}, {6, 51}, {53, 24}, {85, 47},
		{115, 96}, {138, 73}, {129, 31}, {110, 11}
	};

	vector<point> hull = graham_scan(points);
	for (int i = 0; i < hull.size(); i++) {
		cout << '(' << hull[i].first << ", " << hull[i].second << ')' << endl;
	}

	return 0;
}