// interview10.8.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include <iostream>
using namespace std;
int main()
{
	int n, m;
	static int size = 0; //��¼�Ѿ���������������
	static int pos = 0;  //��¼�����λ��
	static int j = 0;
	int k;
	cin >> n >> m;
	int *a = new int[n]; //������ܲɷõ���Ա������
	int *b = new int[n];
	int *c = new int[n];
	int *d = new int[n];
	for (int i = 0; i < n; i++) {
		cin >> a[i];
		size = size + 1;
		if (size == 1)
			b[0] = 0;
		else {
			pos = b[size - 2];//�ؼ���һ�����裬�ҵ�������Ǹ��ڵ�
			for (int s = 1; s < m - 1; s++) {
				pos = b[pos];
			}
			j = pos;
			if (b[j] == 0) {
				b[j] = i;
				b[i] = 0;
			}
			else {
				b[i] = b[j];
				b[j] = i;
			}
		}
		//����ÿ�β���
		/*for (int i = 0; i < size; i++) {
		cout << b[i] << " ";
		}
		cout << endl;*/
	}
	c[0] = a[n - 1];
	k = b[n - 1];
	for (int j = 1; j < n; j++) {
		c[j] = a[k];
		k = b[k];
	}
	d[0] = c[0];
	for (int i = 1; i < n; i++) {
		d[i] = c[n - i];
	}
	for (int i = 0; i < n; i++) {
		cout << d[i] << " ";
	}
	delete[] a;
	delete[] b;
	delete[] c;
	delete[] d;
	return 0;
}