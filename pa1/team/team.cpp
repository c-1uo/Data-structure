// team928版本.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;

//这是第一次代码的递归函数构造，把其中列p确定为初始列后选出第一项p[0]，其余列（包括自身）排除a后重新排序，输出重新排序后列q的首项。每次操作对应一次选人。但是复杂度达到了n的平方级别，有4个测试点超时
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

int a[500000], b[500000], c[500000], d[500001];//数组容量过大申明在全局存储区上，否则内存溢出编译器中断运行。放在main函数中时一直中断，因此查询了解决办法。
char teamname[500001];
int main()
{
	int n;//输入队员总数,1<=i<=500000
	int m;//选出的号码
	int t = 0;
	int c1 = 0, c2 = 0, c3 = 0;//记录操作选人到各个步骤时候分别选到了三个数组的哪一列
	/*cin >> n;*/
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	scanf("%d", &n);
	/*for (int i = 0; i < n + 1; i++) {   //初始化数组用来记录某个人是否被选择
		d[i] = 0;
	}*/
	memset(d, 0, 4*(n + 1));
	//构造三个数组
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

