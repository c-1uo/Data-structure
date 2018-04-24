#include <cstdio>
#include <stdlib.h>
//��������͹���ı���֪ʶ���˽�ο���һ����ʾ�е���վ��http:\\baike.baidu.com\view\707209.htm
//�˽���͹����5���㷨�߼�http://blog.csdn.net/bone_ace/article/details/46239187
//��Ľṹ��
struct point {
	int x;
	int y;
};
//�ȽϺ���,�Ѹ���x�����µ�y���մ�С��������
int cmp(const void *a, const void *b) {
	point *c = (point *)a;
	point *d = (point *)b;
	if ((*c).y < (*d).y)
		return -1;
	else
		return 1;
}
//���������c����ʼ��
long long vec(point a, point b, point c) {
	long long result;
	result = (long long)(a.x - c.x)*(long long)(b.y - c.y) - (long long)(b.x - c.x)*(long long)(a.y - c.y);
	return result;
}

int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//�ο���pa-book�еļ��ٴ���
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n;
	//scanf("%d", &n);
	//char empty;
	//scanf("%c", &empty);
	fread(&n, sizeof(int), 1, stdin);//�ο�����ʾ�е�fread�÷�
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
	}//�ҵ���͹���������㣬y������С��x��С���Ǹ�
	 //�����ҵ����º����ϵĵ�
	int j = 0;
	while (p[j].x == p[0].x&&j<n) {
		j++;
	}
	qsort(&p[0], j, sizeof(p[0]), cmp);//��ô��ʱp[0]���������µ��Ǹ�Ԫ��
	int k = n - 1;
	while (p[k].x == p[n - 1].x&&k >= 0) {
		k--;
	}
	qsort(&p[k + 1], n - 1 - k, sizeof(p[0]), cmp);//p[n]���������ϵ��Ǹ�Ԫ��
	point *q = new point[n];//q�൱��һ��ջ��q�����ǽ��а�͹���°벿��ɨ�����
	int top = -1;
	q[0] = p[0]; ++top;
	q[1] = p[1]; ++top;
	for (int i = 2; i<n; i++) {
		while (vec(p[i], q[top], q[top - 1]) >= 0 && top >= 1) {//����ȥ���߹���ʱ��ȼ��ڽ�ջ��Ԫ�ص���������֤ջ�����������Ԫ�ز���
			--top;
		}
		q[++top] = p[i];
	}
	int mark;
	for (mark = 0; mark <= top; mark++) {
		if (q[mark].x == orix&&q[mark].y == oriy)
			break;
	}
	point *r = new point[n];//r��ɨ���ϰ벿�֣�����r�������ĩ��q���Ѿ���������������Ҫȥ��
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
	for (int i = 1; i <= pos-1; i++) {//ȥ���ظ���Ԫ�أ���ĩ����
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

