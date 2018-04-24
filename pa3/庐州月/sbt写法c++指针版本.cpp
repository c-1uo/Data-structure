//https://wenku.baidu.com/view/364afa42a8956bec0975e3b1.html
//由于AVL的平衡策略实现起来有点复杂，想使用一种操作接口实现更简明但也稳定的二叉搜索树，阅读了陈启峰的SBT论文，学习了整个的算法逻辑以及实现的伪代码
//http://m.blog.csdn.net/paradiserparadiser/article/details/10241485
//http://www.xuebuyuan.com/2225303.html
//参考了上述两篇博客中的SBT模板写法，主要是mantain()接口的写法，自己重写了所需要的search()接口
//http://blog.csdn.net/effective_coder/article/details/8736718
//参考了博客中的几种典型的贪心策略
//http://m.blog.csdn.net/u013761036/article/details/38081287
//参考了hdu4864的贪心策略，对于有两个条件的贪心算法，一般是先挑选出一个优先考虑的条件，再逐个筛选满足第二个条件中的最优解
//http://blog.csdn.net/jianbagengmu/article/details/72742522
//使用了参考博客中介绍的比fread()更底层的read()函数
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
char input[1 << 25];
inline int getnum() {
	static int x;
	static char ch;
	static int pos;
	x = 0, ch = input[pos++];
	while (ch < '0' || '9' < ch)
		ch = input[pos++];
	while ('0' <= ch && ch <= '9') {
		x = x * 10 + ch - '0';
		ch = input[pos++];
	}
	return x;
}
struct SBTnode {
	SBTnode *lc;
	SBTnode *rc;
	int size;//记录以当前节点为根的子树上节点的总个数(包含自身)，空树的时候记为0
	int key; //记录当前的键值
	SBTnode();
};
SBTnode *root = nullptr;
SBTnode::SBTnode() {
	lc = nullptr;
	rc = nullptr;
	size = 0;//新建立出来的节点由于键值还没有赋值，因此size初始化为0
}
void lrotate(SBTnode* &e) {//左旋
	SBTnode *p = e->rc;
	e->rc = p->lc;
	p->lc = e;
	p->size = e->size;
	e->size = (e->lc != nullptr ? e->lc->size : 0) + (e->rc != nullptr ? e->rc->size : 0) + 1;
	e = p;
}

void rrotate(SBTnode* &e) {//右旋
	SBTnode *p = e->lc;
	e->lc = p->rc;
	p->rc = e;
	p->size = e->size;
	e->size = (e->lc != nullptr ? e->lc->size : 0) + (e->rc != nullptr ? e->rc->size : 0) + 1;
	e = p;
}

void maintain(SBTnode* &e, bool flag) {//维护，加入一个bool变量达到简化的效果
	if (flag == false) { //当插入的时候深入的是左子树，则只用继续mantain左子树
		if ((e->lc != nullptr&&e->lc->lc != nullptr&&e->rc != nullptr&&e->lc->lc->size > e->rc->size) || (e->lc != nullptr&&e->lc->lc != nullptr&&e->rc == nullptr)) {//左孩子左子树size大于右孩子size
			rrotate(e);
		}
		else if ((e->lc != nullptr&&e->lc->rc != nullptr&&e->rc != nullptr&&e->lc->rc->size > e->rc->size) || (e->lc != nullptr&&e->lc->rc != nullptr&&e->rc == nullptr)) {//左孩子右子树size大于右孩子size
			lrotate(e->lc);
			rrotate(e);
		}
		else return;
	}
	else { //右边,这里的维护的时候首先需要对节点是否为空进行判断
		if ((e->rc != nullptr&&e->rc->rc != nullptr&&e->lc != nullptr&&e->rc->rc->size > e->lc->size) || (e->rc != nullptr&&e->rc->rc != nullptr&&e->lc == nullptr)) { //右孩子的右子树大于左孩子
			lrotate(e);
		}
		else if ((e->rc != nullptr&&e->rc->lc != nullptr&&e->lc != nullptr&&e->rc->lc->size > e->lc->size) || (e->rc != nullptr&&e->rc->lc != nullptr&&e->lc == nullptr)) { //右孩子的左子树大于左孩子
			rrotate(e->rc);
			lrotate(e);
		}
		else return;
	}
	maintain(e->lc, 0);
	maintain(e->rc, 1);
}

void insert(SBTnode* &e, int value) {//插入
	if (e == nullptr) { //空节点，对应整棵树为空时直接插入到根节点的情况
		e = new SBTnode;
		e->key = value;
		e->size = 1;
	}
	else {//插入的过程和BST的操作基本一致
		e->size++;
		if (value < e->key) insert(e->lc, value);
		else insert(e->rc, value);
		maintain(e, value >= e->key);//不同于普通BST的是，插入完成以后需要维护一次来维持整棵树的size
	}
}

int remove(SBTnode* &e, int value) {//删除
	int k;
	e->size--;
	if (value == e->key || (value < e->key && e->lc == nullptr) || (value > e->key && e->rc == nullptr)) {//命中或者是未命中但是抵达递归基
		k = e->key;
		if (e->lc != nullptr && e->rc != nullptr) {
			e->key = remove(e->lc, e->key + 1);
		}
		else {
			e = e->lc != nullptr ? e->lc : e->rc;
		}
	}
	else if (value > e->key) {//后面的两个分支接口的实现方法和BST基本一致
		k = remove(e->rc, value);
	}
	else if (value < e->key) {
		k = remove(e->lc, value);
	}
	return k;
}

int money;//用来记录每次查找过程中的不小于查询价格中最小的那个
void search(SBTnode* e, int value) {//查找(准确地说是查找过程中找到了不小于value的数中最小的那个)
	if (e == nullptr)
		return;
	if (e->key >= value) {
		money = e->key;
		search(e->lc, value);
	}
	else
		search(e->rc, value);
}

struct bridge {
	int p;//p记录修桥花费
	int v;//v记录美化度
};
struct ma {
	int h;//h记录材料花费
	int d;//d记录材料提升美化度
};
int cmpbridge(const void *a, const void *b) {//桥按照美化度要求降序排列
	bridge *c = (bridge *)a;
	bridge *d = (bridge *)b;
	if ((*c).v > (*d).v)
		return -1;
	else
		return 1;
}
int cmpma(const void *a, const void *b) {//材料也按照美化度降序排列
	ma *e = (ma *)a;
	ma *f = (ma *)b;
	if ((*e).d > (*f).d)
		return -1;
	else
		return 1;
}
int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//这里参考了pa-book中的setvbuf加大缓冲区的代码
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	read(0, input, 1 << 25);//先把所有数据以字符串读到缓冲区，然后通过函数逐个数字计算出来
	int n, m;//n座石桥，m种材料
	long long sum = 0;
	//scanf("%d %d", &n, &m);
	n = getnum(); m = getnum();
	bridge *b = new bridge[n];
	ma *mat = new ma[m];
	for (int i = 0; i < n; i++) {
		//scanf("%d %d", &b[i].p, &b[i].v);
		b[i].p = getnum(); b[i].v = getnum();
	}
	for (int i = 0; i < m; i++) {
		//scanf("%d %d", &mat[i].h, &mat[i].d);
		mat[i].h = getnum(); mat[i].d = getnum();
	}
	qsort(b, n, sizeof(b[0]), cmpbridge);//桥按照美化度降序排列
	qsort(mat, m, sizeof(mat[0]), cmpma);//材料也按照能提升的美化度降序排列
	int j = 0;//用来记录材料被插入树中的个数
	for (int i = 0; i < n; i++) {
		while (j < m&&mat[j].d >= b[i].v) {
			insert(root, mat[j].h);//把美化度满足第i座桥的所有材料都插到树中
			j++;
		}
		search(root, b[i].p);//找到最小的满足桥花费的那种材料
		sum = sum + (long long)money;
		remove(root, money);//由于材料只能使用一次，删除被选出的材料
	}
	printf("%lld", sum);
	delete[] b;
	delete[] mat;
	return 0;
}