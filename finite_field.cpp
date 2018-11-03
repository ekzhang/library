// finite_field.cpp
// Eric K. Zhang; Nov. 3, 2018

#include <bits/stdc++.h>
using namespace std;

typedef long long LL;

template<int p> struct FF {
	LL val;

	FF(const LL x=0) { val = (x % p + p) % p; }

	bool operator==(const FF<p>& other) const { return val == other.val; }
	bool operator!=(const FF<p>& other) const { return val != other.val; }

	FF operator+() const { return val; }
	FF operator-() const { return -val; }

	FF& operator+=(const FF<p>& other) { val = (val + other.val) % p; return *this; }
	FF& operator-=(const FF<p>& other) { *this += -other; return *this; }
	FF& operator*=(const FF<p>& other) { val = (val * other.val) % p; return *this; }
	FF& operator/=(const FF<p>& other) { *this *= other.inv(); return *this; }

	FF operator+(const FF<p>& other) const { return FF(*this) += other; }
	FF operator-(const FF<p>& other) const { return FF(*this) -= other; }
	FF operator*(const FF<p>& other) const { return FF(*this) *= other; }
	FF operator/(const FF<p>& other) const { return FF(*this) /= other; }

	static FF<p> pow(const FF<p>& a, LL b) {
		if (!b) return 1;
		return pow(a * a, b >> 1) * (b & 1 ? a : 1);
	}

	FF<p> pow(const LL b) const { return pow(*this, b); }
	FF<p> inv() const { return pow(p - 2); }
};

template<int p> FF<p> operator+(const LL lhs, const FF<p>& rhs) { return FF<p>(lhs) += rhs; }
template<int p> FF<p> operator-(const LL lhs, const FF<p>& rhs) { return FF<p>(lhs) -= rhs; }
template<int p> FF<p> operator*(const LL lhs, const FF<p>& rhs) { return FF<p>(lhs) *= rhs; }
template<int p> FF<p> operator/(const LL lhs, const FF<p>& rhs) { return FF<p>(lhs) /= rhs; }

typedef FF<1000000007> num;

int main() {
	num a = 23;
	num b = 1 / a;
	assert(a * b == 1);
	cout << a.pow(100).val << endl; // => 398079999
	cout << (a - 30).val << endl; // => 1000000000
}
