#include <cstdio>
#include <stdlib.h>
//��������͹���ı���֪ʶ���˽�ο���һ����ʾ�е���վ��http:\\baike.baidu.com\view\707209.htm
//�˽���͹����5���㷨�߼�http://blog.csdn.net/bone_ace/article/details/46239187
//������graham�㷨
static int ori;//��¼grahamɨ�����ʼ��
static int orix;
static int oriy;
//����
struct point {
public:
	int x;
	int y;
};
//���������c����ʼ��
long long vec(point a, point b, point c) {
	long long result;
	result = (long long)(a.x - c.x)*(long long)(b.y - c.y) - (long long)(b.x - c.x)*(long long)(a.y - c.y);
	return result;
}
int cmp(const void *a, const void *b) {//���ʹ�ý�����mergesort������ʱ��ﵽ800ms�����ǳ����ÿ��еĿ��ţ�Ч���Ժ�һЩ����500ms
	point *c = (point *)a;
	point *d = (point *)b;
	long long g;//�˴�Ϊ64λ���֣���Ϊ��Ŀ�еķ�Χxλ�����ɴﵽ2��25�η�����˳˷��������Խ��
	g = (long long)((*c).x - orix)*(long long)((*d).y - oriy) - (long long)((*d).x - orix)*(long long)((*c).y - oriy);//��һ����ʼһֱû������ֵǰ��long long��pa-book��11���4��ָ����ֵ��������ٸ���ֵ
	if (g > 0)
		return -1;//ca��cb��˳ʱ�뷽�򣬶�Ӧ����������б��a<b
	else if (g < 0)
		return 1;//ca��cb����ʱ�뷽��a>b
	else if (g == 0) {
		if ((long long)((*c).x - orix)*(long long)((*c).x - orix) + (long long)((*c).y - oriy)*(long long)((*c).y - oriy) < (long long)((*d).x - orix)*(long long)((*d).x - orix) + (long long)((*d).y - oriy)*(long long)((*d).y - oriy))
			return -1;//ca��cb����,��a��ø���
		else
			return 1;
	}
}
int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//�ο���pa-book�еļ��ٴ���
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n;
	fread(&n, sizeof(int), 1, stdin);//�ο�����ʾ�е�fread�÷�
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
	}//�ҵ���͹���������㣬y������С��x��С���Ǹ�
	qsort(p, n, sizeof(p[0]), cmp);//��ɰ��ռ��Ǵ�С��������
	point *q = new point[n];//q�൱��һ��ջ
	int top = -1;
	q[0] = p[0]; ++top;
	q[1] = p[1]; ++top;
	for (int i = 2; i<n; i++) {
		while (vec(p[i], q[top], q[top-1]) >= 0 && top >= 1) {//����ȥ���߹���ʱ��ȼ��ڽ�ջ��Ԫ�ص���������֤ջ�����������Ԫ�ز���
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

