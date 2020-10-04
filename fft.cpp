// fft.cpp
// Eric K. Zhang; Nov. 20, 2017

#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Complex {
	T real, imag;
	Complex(T x=(T)0, T y=(T)0) : real(x), imag(y) {}
	Complex conj() { return Complex(real, -imag); }
	Complex operator+(const Complex& c) { return Complex(real + c.real, imag + c.imag); }
	Complex operator-(const Complex& c) { return Complex(real - c.real, imag - c.imag); }
	Complex operator*(const T& num) { return Complex(real * num, imag * num); }
	Complex operator/(const T& num) { return Complex(real / num, imag / num); }
	Complex operator*(const Complex& c) {
		return Complex(real * c.real - imag * c.imag, real * c.imag + imag * c.real);
	}
	Complex operator/(const Complex& c) {
		return *this * c.conj() / (c.x * c.x + c.y * c.y);
	}
};

typedef int itype;
typedef double dtype;

typedef Complex<dtype> ftype;
typedef vector<itype> poly;
const dtype PI = 4 * atan((dtype) 1);

void fft(ftype* A, int n, bool inv=false) {
	for (int i = 1, j = n / 2; i + 1 < n; i++) {
		if (i < j) swap(A[i], A[j]);
		int t = n / 2;
		while (j >= t) j -= t, t >>= 1;
		j += t;
	}
	for (int h = 2; h <= n; h <<= 1) {
		ftype wm(cos(2 * PI / h), sin(2 * PI / h));
		for (int i = 0; i < n; i += h) {
			ftype w(1);
			for (int j = i; j < i + h / 2; j++) {
				ftype x = A[j], y = w * A[j + h / 2];
				A[j + h / 2] = x - y;
				A[j] = x + y;
				w = w * wm;
			}
		}
	}
	if (inv) {
		reverse(A + 1, A + n);
		for (int i = 0; i < n; i++) {
			A[i] = A[i] / n;
		}
	}
}

poly pmul(poly p, poly q) {
	int dim = p.size() + q.size() - 1;
	while (__builtin_popcount(dim) != 1) ++dim;
	ftype* a = new ftype[dim];
	ftype* b = new ftype[dim];
	for (int i = 0; i < p.size(); i++)
		a[i] = p[i];
	for (int i = 0; i < q.size(); i++)
		b[i] = q[i];
	fft(a, dim);
	fft(b, dim);
	for (int i = 0; i < dim; i++)
		a[i] = a[i] * b[i];
	fft(a, dim, true);
	poly res(dim);
	for (int i = 0; i < dim; i++)
		res[i] = round(a[i].real);
	while (res.size() && !res.back())
		res.pop_back();
	delete[] a;
	delete[] b;
	return res;
}

poly ppow(poly p, int k, int mod) {
	poly ret = {1};
	for (int i = 0; (1 << i) <= k; i++) {
		if (k & (1 << i)) {
			ret = pmul(ret, p);
			for (int& c : ret)
				c %= mod;
		}
		p = pmul(p, p);
		for (int& c : p)
			c %= mod;
	}
	return ret;
}

string pprint(poly p) {
	string ret;
	bool leading = true;
	for (int i = p.size() - 1; i >= 0; i--) {
		if (!p[i]) continue;
		if (leading) ret += (p[i] < 0 ? "-" : "");
		else ret += (p[i] < 0 ? " - " : " + ");
		leading = false;
		if (abs(p[i]) != 1) ret += to_string(abs(p[i]));
		if (i) ret += (i == 1 ? "x" : "x^" + to_string(i));
	}
	return ret;
}

int main() {
	poly p = {3, -7, 5}, q = {-3, 9, -2};
	poly r = pmul(p, q);
	cout << "(" << pprint(p) << ")(" << pprint(q) << ") = " << pprint(r) << endl;
	cout << "(" << pprint(p) << ")^3 = " << pprint(ppow(p, 3, 1e4)) << endl;
}
