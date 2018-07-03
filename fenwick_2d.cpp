// fenwick_2d.cpp
// Eric K. Zhang; Nov. 22, 2017

#include <bits/stdc++.h>
using namespace std;

#define MAX 1000

int N, M;
int fenwick[MAX + 1][MAX + 1];

void update(int x, int y, int val) {
	for (int a = x; a <= MAX; a += a & -a) {
		for (int b = y; b <= MAX; b += b & -b) {
			fenwick[a][b] += val;
		}
	}
}

int query(int x, int y) {
	int ans = 0;
	for (int a = x; a > 0; a -= a & -a) {
		for (int b = y; b > 0; b -= b & -b) {
			ans += fenwick[a][b];
		}
	}
	return ans;
}

int main() {
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		int A, B;
		cin >> A >> B;
		update(A, B, 1);
	}
	for (int i = 0; i < M; i++) {
		int A, B;
		cin >> A >> B;
		cout << query(A, B) << '\n';
	}
	cout.flush();
	return 0;
}
