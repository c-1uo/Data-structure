#include <cstdio>
#include <stdlib.h>
//整个关于凸包的背景知识的了解参考了一级提示中的网站：http:\\baike.baidu.com\view\707209.htm
//了解了凸包的5种算法逻辑http://blog.csdn.net/bone_ace/article/details/46239187
//尤其是graham算法
static int ori;//记录graham扫描的起始点
static int orix;
static int oriy;
//点类
struct point {
public:
	int x;
	int y;
};
//叉积，其中c是起始点
long long vec(point a, point b, point c) {
	long long result;
	result = (long long)(a.x - c.x)*(long long)(b.y - c.y) - (long long)(b.x - c.x)*(long long)(a.y - c.y);
	return result;
}
int cmp(const void *a, const void *b) {//起初使用讲义中mergesort，但是时间达到800ms，于是尝试用库中的快排，效果稍好一些到达500ms
	point *c = (point *)a;
	point *d = (point *)b;
	long long g;//此处为64位数字，因为题目中的范围x位数最多可达到2的25次方，因此乘法可能造成越界
	g = (long long)((*c).x - orix)*(long long)((*d).y - oriy) - (long long)((*d).x - orix)*(long long)((*c).y - oriy);//这一步开始一直没有在右值前加long long，pa-book第11面第4条指出左值会溢出后再给右值
	if (g > 0)
		return -1;//ca在cb的顺时针方向，对应的排序中倾斜角a<b
	else if (g < 0)
		return 1;//ca在cb的逆时针方向，a>b
	else if (g == 0) {
		if ((long long)((*c).x - orix)*(long long)((*c).x - orix) + (long long)((*c).y - oriy)*(long long)((*c).y - oriy) < (long long)((*d).x - orix)*(long long)((*d).x - orix) + (long long)((*d).y - oriy)*(long long)((*d).y - oriy))
			return -1;//ca和cb共线,且a离得更近
		else
			return 1;
	}
}
int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//参考了pa-book中的加速代码
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n;
	fread(&n, sizeof(int), 1, stdin);//参考了提示中的fread用法
	point *p;
	p = new point[n];
	for (int i = 0; i<n; i++) {
		fread(&p[i].x, sizeof(int), 1, stdin);
		fread(&p[i].y, sizeof(int), 1, stdin);
	}
	ori = 0;
	orix = p[0].x;
	oriy = p[0].y;
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
	qsort(p, n, sizeof(p[0]), cmp);//完成按照极角从小到大排序
	point *q = new point[n];//q相当于一个栈
	int top = -1;
	q[0] = p[0]; ++top;
	q[1] = p[1]; ++top;
	for (int i = 2; i<n; i++) {
		while (vec(p[i], q[top], q[top-1]) >= 0 && top >= 1) {//凹进去或者共线时候等价于将栈顶元素弹出，但保证栈里最底下两个元素不变
			--top;
		}
		q[++top] = p[i];
	}
	for (int i = 0; i <= top; i++) {
		printf("%d %d\n", q[i].x, q[i].y);
	}
	delete[] p;
	delete[] q;
	return 0;
}

