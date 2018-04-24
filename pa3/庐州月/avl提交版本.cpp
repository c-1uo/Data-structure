//https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/index.htm
//使用了代码示例列表（也就是讲义）中的宏定义、bintree的写法以及avl中搜索、插入删除的代码。自己做了补充使其能处理重复元素并且新写了一个需要的搜索接口
//http://blog.csdn.net/effective_coder/article/details/8736718
//参考了博客中的几种典型的贪心策略
//http://m.blog.csdn.net/u013761036/article/details/38081287
//参考了hdu4864的贪心策略，对于有两个条件的贪心算法，一般是先挑选出一个优先考虑的条件，再逐个筛选满足第二个条件中的最优解
//http://blog.csdn.net/jianbagengmu/article/details/72742522
//使用了参考博客中介绍的比fread()更底层的read()函数，在TSP一题中使用了同样的方法
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>//read()函数头文件
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p) ? (p)->height : -1)
#define Balanced(x) ( stature( (x).lc ) == stature( (x).rc ) )
#define BalFac(x) ( stature( (x).lc ) - stature( (x).rc ) ) 
#define AvlBalanced(x) ( ( -2 < BalFac(x) ) && ( BalFac(x) < 2 ) ) 
#define IsRoot(x) ( ! ( (x).parent ) )
#define IsLChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->lc ) )
#define IsRChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->rc ) )
#define HasParent(x) ( ! IsRoot(x) )
#define HasLChild(x) ( (x).lc )
#define HasRChild(x) ( (x).rc )
#define HasChild(x) ( HasLChild(x) || HasRChild(x) ) 
#define HasBothChild(x) ( HasLChild(x) && HasRChild(x) ) 
#define IsLeaf(x) ( ! HasChild(x) )
#define FromParentTo(x) ( IsRoot(x) ? _root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )
#define tallerChild(x) ( \
	stature( (x)->lc ) > stature( (x)->rc ) ? (x)->lc : ( /*左高*/ \
	stature( (x)->lc ) < stature( (x)->rc ) ? (x)->rc : ( /*右高*/ \
	IsLChild( * (x) ) ? (x)->lc : (x)->rc /*等高：与父亲x同侧者（zIg-zIg或zAg-zAg）优先*/ \
	) \
	) \
	)
template <typename T> void swap(T& a, T& b)//交换函数模板
{
	T temp = a;
	a = b;
	b = temp;
}

char input[1 << 25];//字符串数组用来先一次性以字符形式读入所有的数字
inline int getnum() {//处理这个字符数组，转化成相应的数字
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

template <typename T> struct BinNode {//节点类模板
	T data;
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;
	int height;
	int cnt;//为了处理重复元素，在每个二叉树节点处都记录下当前键码出现的次数
	BinNode() { parent = NULL; lc = NULL; rc = NULL; height = 0; cnt = 1; }//默认构造函数初始化的时候当前键码次数都初始化为1次
	BinNode(T e, BinNodePosi(T) p) { data = e; parent = p; lc = NULL; rc = NULL; height = 0; cnt = 1; }
	BinNodePosi(T) succ();
};

template <typename T> BinNodePosi(T) BinNode<T>::succ() {//找到当前二叉树节点的中序遍历意义上的后继
	BinNodePosi(T) s = this; 
	if (rc) {
		s = rc;
		while (HasLChild(*s)) s = s->lc; 
	}
	else {
		while (IsRChild(*s)) s = s->parent;
		s = s->parent;
	}
	return s;
}


template <typename T> class AVL {
protected:
	int _size; BinNodePosi(T) _root;
	virtual int updateHeight(BinNodePosi(T) x);
	void updateHeightAbove(BinNodePosi(T) x);
	BinNodePosi(T) _hot;
	BinNodePosi(T) connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3);
	BinNodePosi(T) rotateAt(BinNodePosi(T) v);
public:
	T temp;//后续搜索接口是返回当前树中不小于待搜索元素最小的那个，temp用来存储搜索结果
	AVL() : _size(0), _root(NULL) { }
	int size() const { return _size; }
	bool empty() const { return !_root; }
	BinNodePosi(T) root() const { return _root; }
	BinNodePosi(T) & search(const T& e);
	BinNodePosi(T) insert(const T& e);
	bool remove(const T& e);
	void searchlarger(BinNodePosi(T) e, T value);
	void Searchlarger(T value);//新写的搜索接口，返回全树中不小于待搜索元素value最小的那个，方便找到最省钱的那种材料
};

template <typename T> int AVL<T>::updateHeight(BinNodePosi(T) x)//更新节点高度
{
	return x->height = 1 + max(stature(x->lc), stature(x->rc));
}

template <typename T> void AVL<T>::updateHeightAbove(BinNodePosi(T) x)
{
	while (x) { updateHeight(x); x = x->parent; }
}

template <typename T> BinNodePosi(T) AVL<T>::connect34(//3+4重构
	BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
	BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3
) {
	a->lc = T0; if (T0) T0->parent = a;
	a->rc = T1; if (T1) T1->parent = a; updateHeight(a);
	c->lc = T2; if (T2) T2->parent = c;
	c->rc = T3; if (T3) T3->parent = c; updateHeight(c);
	b->lc = a; a->parent = b;
	b->rc = c; c->parent = b; updateHeight(b);
	return b;
}

template <typename T> BinNodePosi(T) AVL<T>::rotateAt(BinNodePosi(T) v) { //旋转
	BinNodePosi(T) p = v->parent; BinNodePosi(T) g = p->parent; 
	if (IsLChild(*p))
		if (IsLChild(*v)) {
			p->parent = g->parent;
			return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
		}
		else {
			v->parent = g->parent;
			return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
		}
	else 
		if (IsRChild(*v)) {
			p->parent = g->parent;
			return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
		}
		else {
			v->parent = g->parent;
			return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
		}
}

template <typename T> 
static BinNodePosi(T) & searchIn(BinNodePosi(T) & v, const T& e, BinNodePosi(T) & hot) {//标准BST的搜索接口
	if (!v || (e == v->data)) return v; 
	hot = v;
	return searchIn(((e < v->data) ? v->lc : v->rc), e, hot); 
}
template <typename T> BinNodePosi(T) & AVL<T>::search(const T& e)
{
	return searchIn(_root, e, _hot = NULL);
} 

template <typename T> BinNodePosi(T) AVL<T>::insert(const T& e) {//插入接口，和常规的略有不同
	BinNodePosi(T) & x = search(e);
	if (x) {//确认目标节点如果之前存在那么数量加一，不需要新插入节点了
		x->cnt++;
		return x;
	} 
	else {//如果搜索失败才需要真实地插入一个新的节点
		BinNodePosi(T) xx = x = new BinNode<T>(e, _hot); _size++; 
		for (BinNodePosi(T) g = _hot; g; g = g->parent) {
			if (!AvlBalanced(*g)) { 
				BinNodePosi(T) &q = FromParentTo(*g);//这里之前一直遇见空指针野指针的错误提示，经过袁泰凌助教在微信群中提示规定运算顺序得到解决
				q = rotateAt(tallerChild(tallerChild(g))); 
				break; 
			}
			else
				updateHeight(g);
		}
		return xx;
	}
} 

template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T) & x, BinNodePosi(T) & hot) {
	BinNodePosi(T) w = x; 
	BinNodePosi(T) succ = NULL; 
	if (!HasLChild(*x)) 
		succ = x = x->rc;
	else if (!HasRChild(*x)) 
		succ = x = x->lc; 
	else { 
		w = w->succ();
		swap(x->data, w->data); 
		swap(x->cnt, w->cnt);//由于交换了当前待删除节点和其后继的位置，因此不光要交换两个节点的data，还需要交换记录各自键码出现次数的cnt
		BinNodePosi(T) u = w->parent;
		((u == x) ? u->rc : u->lc) = succ = w->rc; 
	}
	hot = w->parent; 
	if (succ) succ->parent = hot; 
	return succ; 
} 

template <typename T> bool AVL<T>::remove(const T& e) { //从AVL树中删除关键码e
	BinNodePosi(T) & x = search(e); if (!x) return false; 
	else {
		if (x->cnt > 1) {//先判断待删除的元素在树中出现的次数，即出现次数大于1次只需要对cnt进行减一
			x->cnt--;
			return true;
		}
		else {//只有待删除元素的出现次数正好为1次的时候才需要实质地删除
			removeAt(x, _hot); _size--; 
			for (BinNodePosi(T) g = _hot; g; g = g->parent) {
				if (!AvlBalanced(*g)) {
					BinNodePosi(T) &q = FromParentTo(*g);//这里fromparentto的问题和上面插入函数中一样
					q = rotateAt(tallerChild(tallerChild(g)));
				} 
				updateHeight(g); 
			} 
			return true;
		}
	}
} 

template <typename T> void AVL<T>::searchlarger(BinNodePosi(T) e, T value) {//查找(准确地说是查找过程中隐性地找到了不小于value的数中最小的那个)
	if (!e)
		return;
	if (e->data >= value) {
		temp = e->data;//不断更新temp的数值，每次都更新为当前不小于待查找元素的那个值
		searchlarger(e->lc, value);
	}
	else
		searchlarger(e->rc, value);
}
template <typename T> void AVL<T>::Searchlarger(T value) {
	searchlarger(_root, value);
	return;
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
	//fread(input,sizeof(char), 1 << 25,stdin);//先把所有数据以字符串读到缓冲区，然后通过函数逐个数字计算出来
	read(0, input, 1 << 25);//fread()是封装过后的read()，理论上这样会更快一些
	int n, m;//n座石桥，m种材料
	long long sum = 0;
	AVL<int> AVLtree;
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
			//insert(root, mat[j].h);//把美化度满足第i座桥的所有材料都插到树中
			AVLtree.insert(mat[j].h);
			j++;
		}
		//search(root, b[i].p);//找到最小的满足桥花费的那种材料
		AVLtree.Searchlarger(b[i].p);
		int another = AVLtree.temp;
		sum = sum + (long long)another;
		AVLtree.remove(another);//由于材料只能使用一次，删除被选出的材料
	}
	printf("%lld", sum);
	delete[] b;
	delete[] mat;
	return 0;
}