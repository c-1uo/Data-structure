//http://www.xuetangx.com/courses/course-v1:TsinghuaX+70240183x_1+sp/courseware/4d2369f591f94ced806611c4dcaf79b2/2b4e06cee80b46e6ac888710607d77b6/
//学习了mooc课堂中《计算几何》中的第七章GEOMETRIC RANGE SEARCH的全部内容，使用了其中的range tree来解答本题
//提交过程中碰到了exitcode 8问题，在微信群中由袁泰凌助教解答是除以0的原因
//参考了习题解析的8-20和课本的240和241页建BBST（不断取中位数）的方法
#include <stdio.h>
#include "temperature.h"
#include <stdlib.h>
#define IsRoot(x) ( ! ( (x).parent ) )//这里使用了课程代码列表里面的宏定义写法
#define IsLChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->lc ) )
#define IsRChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->rc ) )
struct station {
	int x;
	int y;
	int tem;
};
station STA[50005];
int n;
int searchSTArank(station* S, int e, int lo, int hi) {//对于每一个x坐标，要在已经排序的x坐标向量中找到对应的秩
	while (lo < hi) {
		int mi = (lo + hi) / 2;
		e < S[mi].x ? hi = mi : lo = mi + 1;
	}
	return --lo;
}
struct Xnode {//构造range tree中xtree节点
	int value;
	int depth;//记录节点深度，方便后续查找lca
	int Ylength;//记录ytree中那一段向量的长度
	Xnode* lc;
	Xnode* rc;
	Xnode* parent;
	int cnt;//记录当前节点的规模
	station* stavec;//每个x坐标下都存放了一个该x坐标下station数组的首地址，也就是ytree的入口地址
	Xnode(int x) { value = x; lc = NULL; rc = NULL; parent = NULL; stavec = NULL; Ylength = 0; }
};
station* mergeYvec(station* a, station* b, int la, int lb) {//每一个父节点的ytree序列都有子代的ytree归并而来
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
Xnode* root = NULL;//初始化根节点
void buildXtree(int lo, int mi, int hi, int* elem, Xnode* v) {
	if (lo == hi) {//当只有一个元素的时候为递归基，并且在叶子节点上直接挂载上ytree
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
		int mil = (lo + mi) / 2;//不断取中位数
		int mir = (mi + 1 + hi) / 2;
		Xnode* templeft = new Xnode(elem[mil]);
		v->lc = templeft;//完成正向和反向的连接
		templeft->parent = v;
		templeft->cnt = mi - lo + 1;//子树规模
		templeft->depth = v->depth + 1;//当前节点的深度
		Xnode* tempright = new Xnode(elem[mir]);
		v->rc = tempright;
		tempright->parent = v;
		tempright->cnt = hi - mi;
		tempright->depth = v->depth + 1;
		buildXtree(lo, mil, mi, elem, v->lc);
		buildXtree(mi + 1, mir, hi, elem, v->rc);
		v->Ylength = v->lc->Ylength + v->rc->Ylength;//构造出左右两个子节点后还要归并各自的ytree得到父节点的ytree
		v->stavec = mergeYvec(v->lc->stavec, v->rc->stavec, v->lc->Ylength, v->rc->Ylength);
		return;
	}
}

Xnode* search(int e, Xnode* v) {//在这棵BBST中的搜索接口
	if (v->lc == NULL&&v->rc == NULL)
		return v;
	else
		return search(e, ((e <= v->value) ? v->lc : v->rc));
}
Xnode* lca(Xnode* a, Xnode* b) {//找到最低公共祖先，因为两个节点的深度最多会相差1，于是要找到长的那个先到达相同深度
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
	int dif = deeper->depth - lower->depth;//计算出两个点的深度差，然后逐个比较parent，从自身比较起
	for (int i = 0; i < dif; i++)
		deeper = deeper->parent;
	while (deeper != lower) {
		deeper = deeper->parent;
		lower = lower->parent;
	}
	return deeper;
}
int searchYleft(station* S, int e, int lo, int hi) {//查找y坐标的时候，返回的是不小于y坐标最小的那个，对应于左括号
	while (lo < hi) {
		int mi = (lo + hi) / 2;
		e > S[mi].y ? lo = mi + 1 : hi = mi;
	}
	return lo;
}
int searchYright(station* S, int e, int lo, int hi) {//返回不大于y坐标中最大的那个，对应于右括号
	while (lo < hi) {
		int mi = (lo + hi) / 2;
		e < S[mi].y ? hi = mi : lo = mi + 1;
	}
	return --lo;
}
long long sum;//温度之和
long long num;//观测站的数量
double average;//平均气温
void singlenode(station* S, int y1, int y2, int n) {//到达lca的路径上可能有多个满足要求的节点，对每个节点算法都是如此
	int left = searchYleft(S, y1, 0, n);
	int right = searchYright(S, y2, 0, n);
	if (left == -1 || right == n || right < left) {//查找失败的情况
		sum = sum + 0;
		num = num + 0;
		return;
	}
	else {
		for (int i = left; i <= right; i++) {
			sum = sum + (long long)((S + i)->tem);//找到之后逐个加到总和中
		}
		num = num + (long long)right - (long long)left + (long long)1;
		return;
	}
}
double calaverage(Xnode* a, Xnode* b, int y1, int y2) {//找到xtree中命中的节点和y坐标的上下边界时
	Xnode* LCA = lca(a, b);//先找到lca
	//先处理向左的子树
	if (IsRoot(*a) || IsRoot(*b)) {//考虑退化成根节点的时候
		singlenode(a->stavec, y1, y2, a->Ylength);
	}
	else {
		if (a == b) {//当命中节点重合时
			singlenode(a->stavec, y1, y2, a->Ylength);
		}
		else {//a和b不重合时
			singlenode(a->stavec, y1, y2, a->Ylength);//a本身先计算进去
			for (a; a->depth >= LCA->depth + 2; a = a->parent) {//把从lca到a路径上每一次左拐时候的右子树加进来
				if (IsLChild(*a)) {
					singlenode(a->parent->rc->stavec, y1, y2, a->parent->rc->Ylength);
				}
			}
			singlenode(b->stavec, y1, y2, b->Ylength);//b本身也算进来
			for (b; b->depth >= LCA->depth + 2; b = b->parent) {//把从lca到b路径上每一次右拐时候的左子树加进来
				if (IsRChild(*b)) {
					singlenode(b->parent->lc->stavec, y1, y2, b->parent->lc->Ylength);
				}
			}
		}
	}
	if (num == 0)//分母为0的情况，这里在群里询问了exitcode8的含义，由袁泰凌助教解答
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
int searchRVec(int *c, int e, int lo, int hi) {//对于xtree中的查找，对于闭区间a,b，边界条件还涉及到前驱，可以先在等价的x向量中找到命中的节点然后再去树里找，保证直接命中
	while (lo < hi) {
		int mi = (lo + hi) / 2;
		e < c[mi] ? hi = mi : lo = mi + 1;
	}
	return --lo;
}//返回的是不大于e的秩的最大值,在向量中先搜索好x的坐标范围，避免在树中搜索的时候还要找前驱
int searchLVec(int* c, int e, int lo, int hi) {
	while (lo < hi) {
		int mi = (lo + hi) / 2;
		e > c[mi] ? lo = mi + 1 : hi = mi;
	}
	return lo;//返回不小于e的秩的最小者
}
int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//这里参考了pa-book中的代码
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	n = GetNumOfStation();
	for (int i = 0; i < n; i++) {
		GetStationInfo(i, &STA[i].x, &STA[i].y, &STA[i].tem);
	}
	qsort(STA, n, sizeof(STA[0]), cmp);//对x坐标进行排序
	int* d = new int[n];//用来存储去重之后的x坐标
	int posInd = 0;//记录d中元素的个数
	for (int i = 0; i < n;) {//去重
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
	buildXtree(0, rootrank, posInd - 1, d, root);//完成xtree的建立
	int x1, y1, x2, y2;//接收查询的四个坐标
	int state;
	do {//这里直接使用了题干中的查询示例代码
		state = GetQuery(&x1, &y1, &x2, &y2);
		// 计算 [x1, y1]-[x2, y2]范围内温度平均值, 存储到queryResult
		int queryResult = 0;
		sum = 0, num = 0, average = 0;
		int lborder, rborder, tborder, bborder;//矩形的左右上下边界
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
		int rank1 = searchLVec(d, lborder, 0, posInd);//现在向量中查询
		int rank2 = searchRVec(d, rborder, 0, posInd);
		if (rank1 == -1 || rank2 == posInd || rank2 < rank1)
			queryResult = 0;
		else {
			Xnode* a = search(d[rank1], root);//命中了在进bbst中做后续操作
			Xnode* b = search(d[rank2], root);
			queryResult = (int)calaverage(a, b, bborder, tborder);
		}
		Response(queryResult);
	} while (state);
	delete[] d;
	return 0;
}