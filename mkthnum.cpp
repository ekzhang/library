// mkthnum.cpp
// Eric K. Zhang; Mar. 22, 2018

#include <bits/stdc++.h>
using namespace std;

#define MAXN 100013
#define MAXW 262144
int N, M, K;
int A[MAXN];
int nums[MAXN];
vector<int> wt[MAXW];

void build_tree(int* b, int* e, int lo=0, int hi=-1, int node=0) {
	if (hi == -1) hi = K - 1;
	if (lo == hi || b >= e) return;
	int mid = (lo + hi) / 2;
	wt[node].push_back(0);
	for (auto it = b; it != e; ++it)
		wt[node].push_back(wt[node].back() + (*it <= mid));
	auto pivot = stable_partition(b, e, [mid](int x) { return x <= mid; });
	build_tree(b, pivot, lo, mid, 2 * node + 1);
	build_tree(pivot, e, mid + 1, hi, 2 * node + 2);
}

// kth order statistic in range [s, e)
int kth(int s, int e, int k, int lo=0, int hi=-1, int node=0) {
	if (hi == -1) hi = K - 1;
	if (lo == hi) return lo;
	int mid = (lo + hi) / 2;
	int sl = wt[node][s], el = wt[node][e];
	int inleft = el - sl;
	if (k < inleft) return kth(sl, el, k, lo, mid, 2 * node + 1);
	return kth(s - sl, e - el, k - inleft, mid + 1, hi, 2 * node + 2);
}

int main() {
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
		nums[i] = A[i];
	}

	// compression
	sort(nums, nums + N);
	K = unique(nums, nums + N) - nums;
	for (int i = 0; i < N; i++) {
		A[i] = lower_bound(nums, nums + K, A[i]) - nums;
	}

	build_tree(A, A + N, 0, K - 1);

	for (int q = 0; q < M; q++) {
		int i, j, k;
		cin >> i >> j >> k;
		--i; --k;
		cout << nums[kth(i, j, k)] << endl;
	}

	return 0;
}
