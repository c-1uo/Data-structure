#include <cstdio>
#include <stdlib.h>
//整个关于凸包的背景知识的了解参考了一级提示中的网站：http:\\baike.baidu.com\view\707209.htm
//了解了凸包的5种算法逻辑http://blog.csdn.net/bone_ace/article/details/46239187
//点的结构体
struct point {
	int x;
	int y;
};
//比较函数,把各个x坐标下的y按照从小到大排序
int cmp(const void *a, const void *b) {
	point *c = (point *)a;
	point *d = (point *)b;
	if ((*c).y < (*d).y)
		return -1;
	else
		return 1;
}
//叉积，其中c是起始点
long long vec(point a, point b, point c) {
	long long result;
	result = (long long)(a.x - c.x)*(long long)(b.y - c.y) - (long long)(b.x - c.x)*(long long)(a.y - c.y);
	return result;
}

int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//参考了pa-book中的加速代码
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n;
	//scanf("%d", &n);
	//char empty;
	//scanf("%c", &empty);
	fread(&n, sizeof(int), 1, stdin);//参考了提示中的fread用法
	point *p;
	p = new point[n];
	for (int i = 0; i<n; i++) {
		fread(&p[i].x, sizeof(int), 1, stdin);
		fread(&p[i].y, sizeof(int), 1, stdin);
		//scanf("%d %d", &p[i].x, &p[i].y);
		//scanf("%c", &empty);
	}
	int ori = 0;
	int orix = p[0].x;
	int oriy = p[0].y;
	for (int i = 0; i < n; i++) {
		if (p[i].y < oriy) {
			ori = i;
			orix = p[i].x;
			oriy = p[i].y;
		}
		else if (p[i].y == oriy) {
			if (p[i].x < orix) {
				ori = i;
				orix = p[i].x;
				oriy = p[i].y;
			}
		}
	}//找到了凸包输出的起点，y坐标最小中x最小的那个
	 //排序，找到左下和右上的点
	int j = 0;
	while (p[j].x == p[0].x&&j<n) {
		j++;
	}
	qsort(&p[0], j, sizeof(p[0]), cmp);//那么此时p[0]就是最左下的那个元素
	int k = n - 1;
	while (p[k].x == p[n - 1].x&&k >= 0) {
		k--;
	}
	qsort(&p[k + 1], n - 1 - k, sizeof(p[0]), cmp);//p[n]就是最右上的那个元素
	point *q = new point[n];//q相当于一个栈，q立面是进行把凸包下半部分扫描进来
	int top = -1;
	q[0] = p[0]; ++top;
	q[1] = p[1]; ++top;
	for (int i = 2; i<n; i++) {
		while (vec(p[i], q[top], q[top - 1]) >= 0 && top >= 1) {//凹进去或者共线时候等价于将栈顶元素弹出，但保证栈里最底下两个元素不变
			--top;
		}
		q[++top] = p[i];
	}
	int mark;
	for (mark = 0; mark <= top; mark++) {
		if (q[mark].x == orix&&q[mark].y == oriy)
			break;
	}
	point *r = new point[n];//r中扫描上半部分，但是r里面的首末在q中已经包含进来了所以要去掉
	int pos = -1;
	r[0] = p[n - 1]; ++pos;
	r[1] = p[n - 2]; ++pos;
	for (int i = n - 3; i >= 0; i--) {
		while (vec(p[i], r[pos], r[pos - 1]) >= 0 && pos >= 1) {
			--pos;
		}
		r[++pos] = p[i];
	}
	for (int i = mark; i <= top; i++) {
	printf("%d %d\n", q[i].x, q[i].y);
	}
	for (int i = 1; i <= pos-1; i++) {//去掉重复的元素，首末两个
		printf("%d %d\n", r[i].x, r[i].y);
	}
	for (int i = 0; i <mark; i++) {
		printf("%d %d\n", q[i].x, q[i].y);
	}
	delete[] p;
	delete[] q;
	delete[] r;
	return 0;
}

