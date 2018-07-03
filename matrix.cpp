// matrix.cpp
// Eric K. Zhang; Dec. 22, 2017

#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vec;
typedef vector<vec> mat;

const int mod = 1e9 + 7;

mat zeros(int n, int m) {
	return mat(n, vec(m));
}

mat id(int n) {
	mat ret = zeros(n, n);
	for (int i = 0; i < n; i++)
		ret[i][i] = 1;
	return ret;
}

mat add(mat a, const mat& b) {
	int n = a.size(), m = a[0].size();
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			a[i][j] = (a[i][j] + b[i][j]) % mod;
	return a;
}

mat mul(const mat& a, const mat& b) {
	int n = a.size(), m = a[0].size(), k = b[0].size();
	mat ret = zeros(n, k);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < k; j++)
			for (int p = 0; p < m; p++)
				ret[i][j] = (ret[i][j] + (long long) a[i][p] * b[p][j]) % mod;
	return ret;
}

mat pow(const mat& a, int p) {
	if (p == 0) return id(a.size());
	mat ret = pow(mul(a, a), p >> 1);
	if (p % 2) ret = mul(ret, a);
	return ret;
}

int main() {
	mat fibonacci = {{1, 1}, {1, 0}};
	int k = 25;
	mat a = pow(fibonacci, k);
	cout << "F_" << k << " = " << a[1][0] << endl;
}
