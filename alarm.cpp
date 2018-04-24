#include <iostream>
#include <cstdio>
using namespace std;
int binSearch(long int *a, long int *b, long int x, long int y, long int lo, long int hi) {
	if (lo == hi) {
		long int mi = (lo + hi) >> 1;
		if (b[mi] * x + a[mi] * y - a[mi] * b[mi] < 0)
			return 0;
		else
			return 1;
	}
	else {
		if (b[lo] * x + a[lo] * y - a[lo] * b[lo] < 0)
			return 0;
		else if (b[hi] * x + a[hi] * y - a[hi] * b[hi] >= 0)
			return hi + 1;
		else {
			while (lo < hi) {
				long int mi = (lo + hi) >> 1;
				if (b[mi] * x + a[mi] * y - a[mi] * b[mi] < 0)
					hi = mi;
				else
					lo = mi + 1;
			}
			return lo;
		}
	}
}

int main()
{
	long int n, m;
	/*cin >> n >> m;*/
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	scanf("%ld %ld", &n, &m);
	long int *a = new long int[n];
	long int *b = new long int[n];
	long int *x = new long int[m];
	long int *y = new long int[m];
	long int *z = new long int[m];
	for (long int i = 0; i < n; i++) {
		/*cin >> a[i] >> b[i];*/
		scanf("%ld %ld", &a[i], &b[i]);
	}
	//检查一下a,b
	/*for (int i = 0; i < n; i++) {
	cout << a[i] << " "<<b[i]<<endl;
	}*/
	for (long int i = 0; i < m; ++i) {
		/*cin >> x[i] >> y[i];*/
		scanf("%ld %ld", &x[i], &y[i]);
		//注释掉的是修改之前的顺序查找的版本
		/*long int k = 0;
		for (long int j = 0; j<n; j++) {
			if (b[j] * x[i] + a[j] * y[i] - a[j] * b[j] < 0) {
				k = j; break;
			}
		}
		if (k != 0)
			z[i] = k;
		if (k == 0)
			z[i] = n;*/
		z[i] = binSearch(a, b, x[i], y[i], 0, n - 1);
	}
	for (int i = 0; i < m; i++) {
		/*cout << z[i] << endl;*/
		printf("%ld\n", z[i]);
	}
	delete[] a;
	delete[] b;
	delete[] x;
	delete[] y;
	delete[] z;
	return 0;
}


