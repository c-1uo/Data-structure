// team928�汾.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;

//���ǵ�һ�δ���ĵݹ麯�����죬��������pȷ��Ϊ��ʼ�к�ѡ����һ��p[0]�������У����������ų�a������������������������q�����ÿ�β�����Ӧһ��ѡ�ˡ����Ǹ��Ӷȴﵽ��n��ƽ��������4�����Ե㳬ʱ
/*int newnum(int s, int p[500000], int q[500000], int r[500000], int num) {
s = p[0];
for (int j = 0; j < num; j++) {
if (q[j] == p[0])
for (int k = j; k < num; k++) { q[k] = q[k + 1]; }
}
for (int j = 0; j < num; j++) {
if (r[j] == p[0])
for (int k = j; k < num; k++) { r[k] = r[k + 1]; }
}
for (int i = 0; i < num; i++) {
p[i] = p[i + 1];
}
return q[0];
};*/

int a[500000], b[500000], c[500000], d[500001];//������������������ȫ�ִ洢���ϣ������ڴ�����������ж����С�����main������ʱһֱ�жϣ���˲�ѯ�˽���취��
char teamname[500001];
int main()
{
	int n;//�����Ա����,1<=i<=500000
	int m;//ѡ���ĺ���
	int t = 0;
	int c1 = 0, c2 = 0, c3 = 0;//��¼����ѡ�˵���������ʱ��ֱ�ѡ���������������һ��
	/*cin >> n;*/
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	scanf("%d", &n);
	/*for (int i = 0; i < n + 1; i++) {   //��ʼ������������¼ĳ�����Ƿ�ѡ��
		d[i] = 0;
	}*/
	memset(d, 0, 4*(n + 1));
	//������������
	for (int j = 0; j < n; j++) {
		/*cin >> a[j];*/
		scanf("%d", &a[j]);
	}
	for (int j = 0; j < n; j++) {
		scanf("%d", &b[j]);
	}
	for (int j = 0; j < n; j++) {
		scanf("%d", &c[j]);
	}
	/*cin >> m;*/
	
	scanf("%d", &m);
	for (int i = 0; i < n; i++) {
		if (i % 3 == 0) {
			while (d[a[c1]] != 0) { c1++; }
			teamname[a[c1]] = 'A';
			d[a[c1]] = 1;
		}
		else if (i % 3 == 1) {
			while (d[b[c2]] != 0) { c2++; }
			teamname[b[c2]] = 'B';
			d[b[c2]] = 1;
		}
		else {
			while (d[c[c3]] != 0) { c3++; }
			teamname[c[c3]] = 'C';
			d[c[c3]] = 1;
		}
	}
	/*cout << teamname[m] << endl;*/
	printf("%c\n", teamname[m]);
	return 0;
}

