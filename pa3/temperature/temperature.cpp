//http://www.xuetangx.com/courses/course-v1:TsinghuaX+70240183x_1+sp/courseware/4d2369f591f94ced806611c4dcaf79b2/2b4e06cee80b46e6ac888710607d77b6/
//ѧϰ��mooc�����С����㼸�Ρ��еĵ�����GEOMETRIC RANGE SEARCH��ȫ�����ݣ�ʹ�������е�range tree�������
//�ύ������������exitcode 8���⣬��΢��Ⱥ����Ԭ̩�����̽���ǳ���0��ԭ��
//�ο���ϰ�������8-20�Ϳα���240��241ҳ��BBST������ȡ��λ�����ķ���
#include <stdio.h>
#include "temperature.h"
#include <stdlib.h>
#define IsRoot(x) ( ! ( (x).parent ) )//����ʹ���˿γ̴����б�����ĺ궨��д��
#define IsLChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->lc ) )
#define IsRChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->rc ) )
struct station {
	int x;
	int y;
	int tem;
};
station STA[50005];
int n;
int searchSTArank(station* S, int e, int lo, int hi) {//����ÿһ��x���꣬Ҫ���Ѿ������x�����������ҵ���Ӧ����
	while (lo < hi) {
		int mi = (lo + hi) / 2;
		e < S[mi].x ? hi = mi : lo = mi + 1;
	}
	return --lo;
}
struct Xnode {//����range tree��xtree�ڵ�
	int value;
	int depth;//��¼�ڵ���ȣ������������lca
	int Ylength;//��¼ytree����һ�������ĳ���
	Xnode* lc;
	Xnode* rc;
	Xnode* parent;
	int cnt;//��¼��ǰ�ڵ�Ĺ�ģ
	station* stavec;//ÿ��x�����¶������һ����x������station������׵�ַ��Ҳ����ytree����ڵ�ַ
	Xnode(int x) { value = x; lc = NULL; rc = NULL; parent = NULL; stavec = NULL; Ylength = 0; }
};
station* mergeYvec(station* a, station* b, int la, int lb) {//ÿһ�����ڵ��ytree���ж����Ӵ���ytree�鲢����
	station* temp = new station[la + lb];
	int i = 0, j = 0, k = 0;
	while (j < la&&k < lb) {
		if ((*(a + j)).y < (*(b + k)).y) {
			*(temp + i) = *(a + j);
			i++;
			j++;
		}
		else {
			*(temp + i) = *(b + k);
			i++;
			k++;
		}
	}
	while (j < la) {
		*(temp + i) = *(a + j);
		i++;
		j++;
	}
	while (k < lb) {
		*(temp + i) = *(b + k);
		i++;
		k++;
	}
	return temp;
}
Xnode* root = NULL;//��ʼ�����ڵ�
void buildXtree(int lo, int mi, int hi, int* elem, Xnode* v) {
	if (lo == hi) {//��ֻ��һ��Ԫ�ص�ʱ��Ϊ�ݹ����������Ҷ�ӽڵ���ֱ�ӹ�����ytree
		int index = searchSTArank(STA, elem[lo], 0, n);
		int begin = index;
		while (begin > -1 && elem[lo] == STA[begin].x)
			begin--;
		int length = index - begin;
		v->stavec = new station[length];
		for (int i = 0; i < length; i++) {
			*(v->stavec + i) = STA[i + begin + 1];
		}
		v->Ylength = length;
		return;
	}
	else {
		int mil = (lo + mi) / 2;//����ȡ��λ��
		int mir = (mi + 1 + hi) / 2;
		Xnode* templeft = new Xnode(elem[mil]);
		v->lc = templeft;//�������ͷ��������
		templeft->parent = v;
		templeft->cnt = mi - lo + 1;//������ģ
		templeft->depth = v->depth + 1;//��ǰ�ڵ�����
		Xnode* tempright = new Xnode(elem[mir]);
		v->rc = tempright;
		tempright->parent = v;
		tempright->cnt = hi - mi;
		tempright->depth = v->depth + 1;
		buildXtree(lo, mil, mi, elem, v->lc);
		buildXtree(mi + 1, mir, hi, elem, v->rc);
		v->Ylength = v->lc->Ylength + v->rc->Ylength;//��������������ӽڵ��Ҫ�鲢���Ե�ytree�õ����ڵ��ytree
		v->stavec = mergeYvec(v->lc->stavec, v->rc->stavec, v->lc->Ylength, v->rc->Ylength);
		return;
	}
}

Xnode* search(int e, Xnode* v) {//�����BBST�е������ӿ�
	if (v->lc == NULL&&v->rc == NULL)
		return v;
	else
		return search(e, ((e <= v->value) ? v->lc : v->rc));
}
Xnode* lca(Xnode* a, Xnode* b) {//�ҵ���͹������ȣ���Ϊ�����ڵ������������1������Ҫ�ҵ������Ǹ��ȵ�����ͬ���
	Xnode* deeper;
	Xnode* lower;
	if (a->depth > b->depth) {
		deeper = a;
		lower = b;
	}
	else {
		deeper = b;
		lower = a;
	}
	int dif = deeper->depth - lower->depth;//��������������ȲȻ������Ƚ�parent��������Ƚ���
	for (int i = 0; i < dif; i++)
		deeper = deeper->parent;
	while (deeper != lower) {
		deeper = deeper->parent;
		lower = lower->parent;
	}
	return deeper;
}
int searchYleft(station* S, int e, int lo, int hi) {//����y�����ʱ�򣬷��ص��ǲ�С��y������С���Ǹ�����Ӧ��������
	while (lo < hi) {
		int mi = (lo + hi) / 2;
		e > S[mi].y ? lo = mi + 1 : hi = mi;
	}
	return lo;
}
int searchYright(station* S, int e, int lo, int hi) {//���ز�����y�����������Ǹ�����Ӧ��������
	while (lo < hi) {
		int mi = (lo + hi) / 2;
		e < S[mi].y ? hi = mi : lo = mi + 1;
	}
	return --lo;
}
long long sum;//�¶�֮��
long long num;//�۲�վ������
double average;//ƽ������
void singlenode(station* S, int y1, int y2, int n) {//����lca��·���Ͽ����ж������Ҫ��Ľڵ㣬��ÿ���ڵ��㷨�������
	int left = searchYleft(S, y1, 0, n);
	int right = searchYright(S, y2, 0, n);
	if (left == -1 || right == n || right < left) {//����ʧ�ܵ����
		sum = sum + 0;
		num = num + 0;
		return;
	}
	else {
		for (int i = left; i <= right; i++) {
			sum = sum + (long long)((S + i)->tem);//�ҵ�֮������ӵ��ܺ���
		}
		num = num + (long long)right - (long long)left + (long long)1;
		return;
	}
}
double calaverage(Xnode* a, Xnode* b, int y1, int y2) {//�ҵ�xtree�����еĽڵ��y��������±߽�ʱ
	Xnode* LCA = lca(a, b);//���ҵ�lca
	//�ȴ������������
	if (IsRoot(*a) || IsRoot(*b)) {//�����˻��ɸ��ڵ��ʱ��
		singlenode(a->stavec, y1, y2, a->Ylength);
	}
	else {
		if (a == b) {//�����нڵ��غ�ʱ
			singlenode(a->stavec, y1, y2, a->Ylength);
		}
		else {//a��b���غ�ʱ
			singlenode(a->stavec, y1, y2, a->Ylength);//a�����ȼ����ȥ
			for (a; a->depth >= LCA->depth + 2; a = a->parent) {//�Ѵ�lca��a·����ÿһ�����ʱ����������ӽ���
				if (IsLChild(*a)) {
					singlenode(a->parent->rc->stavec, y1, y2, a->parent->rc->Ylength);
				}
			}
			singlenode(b->stavec, y1, y2, b->Ylength);//b����Ҳ�����
			for (b; b->depth >= LCA->depth + 2; b = b->parent) {//�Ѵ�lca��b·����ÿһ���ҹ�ʱ����������ӽ���
				if (IsRChild(*b)) {
					singlenode(b->parent->lc->stavec, y1, y2, b->parent->lc->Ylength);
				}
			}
		}
	}
	if (num == 0)//��ĸΪ0�������������Ⱥ��ѯ����exitcode8�ĺ��壬��Ԭ̩�����̽��
		return 0;
	else {
		average = (double)sum / (double)num;
		return average;
	}
}

int cmp(const void* a, const void* b) {
	station* c = (station*)a;
	station* d = (station*)b;
	if ((*c).x < (*d).x)
		return -1;
	else if ((*c).x == (*d).x) {
		if ((*c).y < (*d).y)
			return -1;
		else
			return 1;
	}
	else
		return 1;
}
int searchRVec(int *c, int e, int lo, int hi) {//����xtree�еĲ��ң����ڱ�����a,b���߽��������漰��ǰ�����������ڵȼ۵�x�������ҵ����еĽڵ�Ȼ����ȥ�����ң���ֱ֤������
	while (lo < hi) {
		int mi = (lo + hi) / 2;
		e < c[mi] ? hi = mi : lo = mi + 1;
	}
	return --lo;
}//���ص��ǲ�����e���ȵ����ֵ,����������������x�����귶Χ������������������ʱ��Ҫ��ǰ��
int searchLVec(int* c, int e, int lo, int hi) {
	while (lo < hi) {
		int mi = (lo + hi) / 2;
		e > c[mi] ? lo = mi + 1 : hi = mi;
	}
	return lo;//���ز�С��e���ȵ���С��
}
int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//����ο���pa-book�еĴ���
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	n = GetNumOfStation();
	for (int i = 0; i < n; i++) {
		GetStationInfo(i, &STA[i].x, &STA[i].y, &STA[i].tem);
	}
	qsort(STA, n, sizeof(STA[0]), cmp);//��x�����������
	int* d = new int[n];//�����洢ȥ��֮���x����
	int posInd = 0;//��¼d��Ԫ�صĸ���
	for (int i = 0; i < n;) {//ȥ��
		d[posInd] = STA[i].x;
		int j = i;
		while (j < n&&STA[j].x == STA[i].x)
			j++;
		posInd++;
		i = j;
	}
	int rootrank = (0 + posInd - 1) / 2;
	root = new Xnode(d[rootrank]);
	root->cnt = posInd;
	root->depth = 0;
	buildXtree(0, rootrank, posInd - 1, d, root);//���xtree�Ľ���
	int x1, y1, x2, y2;//���ղ�ѯ���ĸ�����
	int state;
	do {//����ֱ��ʹ��������еĲ�ѯʾ������
		state = GetQuery(&x1, &y1, &x2, &y2);
		// ���� [x1, y1]-[x2, y2]��Χ���¶�ƽ��ֵ, �洢��queryResult
		int queryResult = 0;
		sum = 0, num = 0, average = 0;
		int lborder, rborder, tborder, bborder;//���ε��������±߽�
		if (x1 < x2) {
			lborder = x1;
			rborder = x2;
		}
		else {
			lborder = x2;
			rborder = x1;
		}
		if (y1 < y2) {
			bborder = y1;
			tborder = y2;
		}
		else {
			bborder = y2;
			tborder = y1;
		}
		int rank1 = searchLVec(d, lborder, 0, posInd);//���������в�ѯ
		int rank2 = searchRVec(d, rborder, 0, posInd);
		if (rank1 == -1 || rank2 == posInd || rank2 < rank1)
			queryResult = 0;
		else {
			Xnode* a = search(d[rank1], root);//�������ڽ�bbst������������
			Xnode* b = search(d[rank2], root);
			queryResult = (int)calaverage(a, b, bborder, tborder);
		}
		Response(queryResult);
	} while (state);
	delete[] d;
	return 0;
}