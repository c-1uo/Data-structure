//http://www.xuetangx.com/courses/course-v1:TsinghuaX+70240183x_1+sp/courseware/f21f5d119f0b4189b5fdaf9bcc45714d/70140e0ff13243db8c5910f4ce96552a/
//参考了mooc《计算几何》第八章windowing query中segment tree这一节的视频
//https://www.cnblogs.com/TheRoadToTheGold/p/6254255.html
//参考了上面这篇博客中对于区间修改中懒标记的介绍
#include <stdio.h>
#include <stdlib.h>
struct segment {//存储m行待处理的线段
	int left, right;
};
struct segnode {//构造segment tree节点
	int lb, rb;//每个节点都记录了一个线段区间的左右两个端点
	segnode* lc;
	segnode* rc;
	segnode* parent;
	bool op;//记录当前节点是否被整体翻转过，初始化都是默认未被翻转过。也就是懒标记
	int size;//记录当前节点的扑克牌总数
	int upsize;//记录当前扑克牌的正面朝上的总数
	segnode() { lc = NULL; rc = NULL; parent = NULL; op = false; }
	segnode(int a, int b) { lb = a, rb = b; lc = NULL; rc = NULL; parent = NULL; op = false; }
};
segnode* root = NULL;//初始化根节点
void buildSegtree(int lo, int mi, int hi, segment* elem, segnode* v) {
	if (lo == hi) {//当只有一个元素的时候为递归基
		v->lb = elem[lo].left;
		v->rb = elem[lo].right;
		v->size = elem[lo].right - elem[lo].left;
		v->upsize = elem[lo].right - elem[lo].left;//初始化的时候扑克牌全部为正的
		return;
	}
	else {
		int mil = (lo + mi) / 2;//不断取中位数
		int mir = (mi + 1 + hi) / 2;
		segnode* templeft = new segnode();
		v->lc = templeft;//完成正向和反向的连接
		templeft->parent = v;
		segnode* tempright = new segnode();
		v->rc = tempright;
		tempright->parent = v;
		buildSegtree(lo, mil, mi, elem, v->lc);
		buildSegtree(mi + 1, mir, hi, elem, v->rc);
		v->lb = v->lc->lb;//更新父亲节点的线段信息即扑克牌的数量信息
		v->rb = v->rc->rb;
		v->size = v->lc->size + v->rc->size;
		v->upsize = v->lc->upsize + v->rc->upsize;
		return;
	}
}
bool notcommon(segnode* v, segment* e) {//判断两条线段是否有交集
	if (e->right <= v->lb || e->left >= v->rb)
		return true;
	else
		return false;
}
bool isbelong(segnode* v, segment* e) {//判断待插入的线段是否能够完全包含待查询的节点中存储的elementary interval
	if (v->lb >= e->left&&v->rb <= e->right)
		return true;
	else
		return false;
}
void insert(segnode* v, segment* e) {//对应于'H'开头的命令，相当于把边插到树里了，插入的线段被分成若干条子线段存储在树的不同节点中
	if (notcommon(v, e))//没有交集直接返回
		return;
	if (isbelong(v, e)) {//命中子集的一部分时，更新当前节点的扑克牌信息
		v->op = !(v->op);
		v->upsize = v->size - v->upsize;
		return;
	}
	if (v->op != false) {//维护左孩子和右孩子的信息，向下维护一层。这里参考了代码最开头所给博客链接中懒标记的使用方法
		v->lc->op = !(v->lc->op);
		v->rc->op = !(v->rc->op);
		v->lc->upsize = v->lc->size - v->lc->upsize;
		v->rc->upsize = v->rc->size - v->rc->upsize;
		v->op = false;
	}
	insert(v->lc, e);
	insert(v->rc, e);
	v->upsize = v->lc->upsize + v->rc->upsize;//递归处理完左右子树之后要对当前节点的扑克牌信息更新
}
/*void insert(segnode* v, segment* e) {这是第一次超时的插入版本，因为对于每一个找到的子线段都立马在插入的时候对当前节点以下的所有节点都进行了信息更新
	if (isbelong(v, e)) {//退化为了蛮力算法
		maintaindown(v);
		stack[pos++] = v;
		return;
	}
	if (havecommon(v->lc, e))
		insert(v->lc, e);
	if (havecommon(v->rc, e))
		insert(v->rc, e);
}*/
int num = 0;
void query(segnode* v, segment* e) {//对应于'Q'开头的命令，相当于找出待查询的边在树中的那些正则子集
	if (notcommon(v, e))//没有交集的时候返回空
		return;
	if (isbelong(v, e)) {//正好命中
		num = num + v->upsize;
		return;
	}
	if (v->op != false) {//因为插入的时候对于正好能包含的树中节点的边就停止递归了，命中节点的下方的子节点的信息并没有更新，现在更新一层再交给递归
		v->lc->op = !(v->lc->op);
		v->rc->op = !(v->rc->op);
		v->lc->upsize = v->lc->size - v->lc->upsize;
		v->rc->upsize = v->rc->size - v->rc->upsize;
		v->op = false;
	}
	query(v->lc, e);
	query(v->rc, e);
	return;
}
int cmp(const void* a, const void* b) {
	int* c = (int*)a;
	int* d = (int*)b;
	if ((*c) - (*d) < 0)
		return -1;
	else
		return 1;
}
int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n, m;
	scanf("%d %d", &n, &m);
	segment* s = new segment[m];
	char* order = new char[m + 1];//用%s读取字符的时候要预留出截止符的空间
	int* vertex = new int[2 * m];//存放m条命令中所有的线段两个端点
	for (int i = 0; i < m; i++) {
		scanf("%s %d %d", &order[i], &((s + i)->left), &((s + i)->right));
	}
	for (int i = 0; i < m; i++)//因为语义上规定区间都是左开右闭的，所以对线段的左边都减一
		s[i].left--;
	for (int i = 0; i < m; i++) {
		vertex[2 * i] = s[i].left;
		vertex[2 * i + 1] = s[i].right;
	}
	qsort(vertex, 2 * m, sizeof(vertex[0]), cmp);//然后对2*m个顶点去重
	int* unique = new int[2 * m];
	int count = 0;
	for (int i = 0; i < 2 * m;) {
		unique[count++] = vertex[i];
		int j = i;
		while (j < 2 * m&&vertex[j] == vertex[i])
			j++;
		i = j;
	}
	segment* EI = new segment[count - 1];//得到基础的线段区间(elementary interval)，也都是左开右闭的
	for (int i = 0; i < count - 1; i++) {
		EI[i].left = unique[i];
		EI[i].right = unique[i + 1];
	}
	int midpoint = (0 + count - 2) / 2;
	root = new segnode();
	buildSegtree(0, midpoint, count - 2, EI, root);//建树
	for (int i = 0; i < m; i++) {
		if (order[i] == 'H') {
			insert(root, s + i);
		}
		else {
			num = 0;
			query(root, s + i);
			printf("%d\n", num);
		}
	}
	delete[] s;
	delete[] order;
	delete[] vertex;
	delete[] unique;
	delete[] EI;
	return 0;
}