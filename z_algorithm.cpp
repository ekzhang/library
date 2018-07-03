// z_algorithm.cpp
// Eric K. Zhang; Dec. 18, 2017

#include <bits/stdc++.h>
using namespace std;

vector<int> z_algorithm(string S) {
	int N = S.size(), L = 0, R = 0;
	vector<int> Z(N, 0);
	Z[0] = N;
	for (int i = 1; i < N; i++) {
		if (i < R)
			Z[i] = min(R - i, Z[i - L]);
		while (i + Z[i] < N && S[i + Z[i]] == S[Z[i]])
			++Z[i];
		if (i + Z[i] > R)
			L = i, R = i + Z[i];
	}
	return Z;
}

vector<int> search(string S, string P) {
	int N = S.size(), M = P.size();
	string combined = P + S;
	vector<int> Z = z_algorithm(combined);
	vector<int> matches;
	for (int i = 0; i < N; i++) {
		if (Z[M + i] >= M) {
			matches.push_back(i);
		}
	}
	return matches;
}

int main() {
	string S = "ababbababbabababbabababbababbaba";
	vector<int> Z = z_algorithm(S);
	for (int i = 0; i < S.size(); i++) {
		cout << "Index " << i << ", Z-value: " << Z[i] << endl;
		cout << S.substr(0, Z[i]) << '|' << S.substr(Z[i]) << endl;
		cout << S.substr(i, Z[i]) << '|' << S.substr(i + Z[i]) << endl;
		cout << endl;
	}
}
