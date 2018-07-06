// suffix_array.cpp
// Eric K. Zhang; Dec. 17, 2017

#include <bits/stdc++.h>
using namespace std;

vector<int> suffix_array(string S) {
	int N = S.size();
	vector<int> sa(N), classes(N);
	for (int i = 0; i < N; i++) {
		sa[i] = N - 1 - i;
		classes[i] = S[i];
	}
	stable_sort(sa.begin(), sa.end(), [&S](int i, int j) {
		return S[i] < S[j];
	});
	for (int len = 1; len < N; len *= 2) {
		vector<int> c(classes);
		for (int i = 0; i < N; i++) {
			bool same = i && sa[i - 1] + len < N
			              && c[sa[i]] == c[sa[i - 1]]
			              && c[sa[i] + len / 2] == c[sa[i - 1] + len / 2];
			classes[sa[i]] = same ? classes[sa[i - 1]] : i;
		}
		vector<int> cnt(N), s(sa);
		for (int i = 0; i < N; i++)
			cnt[i] = i;
		for (int i = 0; i < N; i++) {
			int s1 = s[i] - len;
			if (s1 >= 0)
				sa[cnt[classes[s1]]++] = s1;
		}
	}
	return sa;
}

vector<int> lcp_array(const vector<int>& sa, string S) {
	int N = S.size();
	vector<int> rank(N), lcp(N - 1);
	for (int i = 0; i < N; i++)
		rank[sa[i]] = i;

	int pre = 0;
	for (int i = 0; i < N; i++) {
		if (rank[i] < N - 1) {
			int j = sa[rank[i] + 1];
			while (max(i, j) + pre < S.size() && S[i + pre] == S[j + pre])
				++pre;
			lcp[rank[i]] = pre;
			if (pre > 0)
				--pre;
		}
	}
	return lcp;
}

int main() {
	const string S = "AGCGCCCTTGCGAGCAGTCGTATGCTTTCTCGAATTCCGAGCGGTTAAGCGTGAC";

	vector<int> sa = suffix_array(S);
	vector<int> lcp = lcp_array(sa, S);

	cout << S << endl;
	cout << string(S.size(), '=') << endl;
	for (int i = 0; i < S.size(); i++) {
		cout << setw(S.size()) << right << S.substr(0, sa[i]) << '|';
		cout << setw(S.size()) << left << S.substr(sa[i]);
		if (i < S.size() - 1)
			cout << ' ' << lcp[i];
		cout << endl;
	}

	return 0;
}
