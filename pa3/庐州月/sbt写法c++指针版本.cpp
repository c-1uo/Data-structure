//https://wenku.baidu.com/view/364afa42a8956bec0975e3b1.html
//����AVL��ƽ�����ʵ�������е㸴�ӣ���ʹ��һ�ֲ����ӿ�ʵ�ָ�������Ҳ�ȶ��Ķ������������Ķ��˳������SBT���ģ�ѧϰ���������㷨�߼��Լ�ʵ�ֵ�α����
//http://m.blog.csdn.net/paradiserparadiser/article/details/10241485
//http://www.xuebuyuan.com/2225303.html
//�ο���������ƪ�����е�SBTģ��д������Ҫ��mantain()�ӿڵ�д�����Լ���д������Ҫ��search()�ӿ�
//http://blog.csdn.net/effective_coder/article/details/8736718
//�ο��˲����еļ��ֵ��͵�̰�Ĳ���
//http://m.blog.csdn.net/u013761036/article/details/38081287
//�ο���hdu4864��̰�Ĳ��ԣ�����������������̰���㷨��һ��������ѡ��һ�����ȿ��ǵ������������ɸѡ����ڶ��������е����Ž�
//http://blog.csdn.net/jianbagengmu/article/details/72742522
//ʹ���˲ο������н��ܵı�fread()���ײ��read()����
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
	int size;//��¼�Ե�ǰ�ڵ�Ϊ���������Ͻڵ���ܸ���(��������)��������ʱ���Ϊ0
	int key; //��¼��ǰ�ļ�ֵ
	SBTnode();
};
SBTnode *root = nullptr;
SBTnode::SBTnode() {
	lc = nullptr;
	rc = nullptr;
	size = 0;//�½��������Ľڵ����ڼ�ֵ��û�и�ֵ�����size��ʼ��Ϊ0
}
void lrotate(SBTnode* &e) {//����
	SBTnode *p = e->rc;
	e->rc = p->lc;
	p->lc = e;
	p->size = e->size;
	e->size = (e->lc != nullptr ? e->lc->size : 0) + (e->rc != nullptr ? e->rc->size : 0) + 1;
	e = p;
}

void rrotate(SBTnode* &e) {//����
	SBTnode *p = e->lc;
	e->lc = p->rc;
	p->rc = e;
	p->size = e->size;
	e->size = (e->lc != nullptr ? e->lc->size : 0) + (e->rc != nullptr ? e->rc->size : 0) + 1;
	e = p;
}

void maintain(SBTnode* &e, bool flag) {//ά��������һ��bool�����ﵽ�򻯵�Ч��
	if (flag == false) { //�������ʱ�������������������ֻ�ü���mantain������
		if ((e->lc != nullptr&&e->lc->lc != nullptr&&e->rc != nullptr&&e->lc->lc->size > e->rc->size) || (e->lc != nullptr&&e->lc->lc != nullptr&&e->rc == nullptr)) {//����������size�����Һ���size
			rrotate(e);
		}
		else if ((e->lc != nullptr&&e->lc->rc != nullptr&&e->rc != nullptr&&e->lc->rc->size > e->rc->size) || (e->lc != nullptr&&e->lc->rc != nullptr&&e->rc == nullptr)) {//����������size�����Һ���size
			lrotate(e->lc);
			rrotate(e);
		}
		else return;
	}
	else { //�ұ�,�����ά����ʱ��������Ҫ�Խڵ��Ƿ�Ϊ�ս����ж�
		if ((e->rc != nullptr&&e->rc->rc != nullptr&&e->lc != nullptr&&e->rc->rc->size > e->lc->size) || (e->rc != nullptr&&e->rc->rc != nullptr&&e->lc == nullptr)) { //�Һ��ӵ���������������
			lrotate(e);
		}
		else if ((e->rc != nullptr&&e->rc->lc != nullptr&&e->lc != nullptr&&e->rc->lc->size > e->lc->size) || (e->rc != nullptr&&e->rc->lc != nullptr&&e->lc == nullptr)) { //�Һ��ӵ���������������
			rrotate(e->rc);
			lrotate(e);
		}
		else return;
	}
	maintain(e->lc, 0);
	maintain(e->rc, 1);
}

void insert(SBTnode* &e, int value) {//����
	if (e == nullptr) { //�սڵ㣬��Ӧ������Ϊ��ʱֱ�Ӳ��뵽���ڵ�����
		e = new SBTnode;
		e->key = value;
		e->size = 1;
	}
	else {//����Ĺ��̺�BST�Ĳ�������һ��
		e->size++;
		if (value < e->key) insert(e->lc, value);
		else insert(e->rc, value);
		maintain(e, value >= e->key);//��ͬ����ͨBST���ǣ���������Ժ���Ҫά��һ����ά����������size
	}
}

int remove(SBTnode* &e, int value) {//ɾ��
	int k;
	e->size--;
	if (value == e->key || (value < e->key && e->lc == nullptr) || (value > e->key && e->rc == nullptr)) {//���л�����δ���е��ǵִ�ݹ��
		k = e->key;
		if (e->lc != nullptr && e->rc != nullptr) {
			e->key = remove(e->lc, e->key + 1);
		}
		else {
			e = e->lc != nullptr ? e->lc : e->rc;
		}
	}
	else if (value > e->key) {//�����������֧�ӿڵ�ʵ�ַ�����BST����һ��
		k = remove(e->rc, value);
	}
	else if (value < e->key) {
		k = remove(e->lc, value);
	}
	return k;
}

int money;//������¼ÿ�β��ҹ����еĲ�С�ڲ�ѯ�۸�����С���Ǹ�
void search(SBTnode* e, int value) {//����(׼ȷ��˵�ǲ��ҹ������ҵ��˲�С��value��������С���Ǹ�)
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
	int p;//p��¼���Ż���
	int v;//v��¼������
};
struct ma {
	int h;//h��¼���ϻ���
	int d;//d��¼��������������
};
int cmpbridge(const void *a, const void *b) {//�Ű���������Ҫ��������
	bridge *c = (bridge *)a;
	bridge *d = (bridge *)b;
	if ((*c).v > (*d).v)
		return -1;
	else
		return 1;
}
int cmpma(const void *a, const void *b) {//����Ҳ���������Ƚ�������
	ma *e = (ma *)a;
	ma *f = (ma *)b;
	if ((*e).d > (*f).d)
		return -1;
	else
		return 1;
}
int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//����ο���pa-book�е�setvbuf�Ӵ󻺳����Ĵ���
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	read(0, input, 1 << 25);//�Ȱ������������ַ���������������Ȼ��ͨ������������ּ������
	int n, m;//n��ʯ�ţ�m�ֲ���
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
	qsort(b, n, sizeof(b[0]), cmpbridge);//�Ű��������Ƚ�������
	qsort(mat, m, sizeof(mat[0]), cmpma);//����Ҳ�����������������Ƚ�������
	int j = 0;//������¼���ϱ��������еĸ���
	for (int i = 0; i < n; i++) {
		while (j < m&&mat[j].d >= b[i].v) {
			insert(root, mat[j].h);//�������������i���ŵ����в��϶��嵽����
			j++;
		}
		search(root, b[i].p);//�ҵ���С�������Ż��ѵ����ֲ���
		sum = sum + (long long)money;
		remove(root, money);//���ڲ���ֻ��ʹ��һ�Σ�ɾ����ѡ���Ĳ���
	}
	printf("%lld", sum);
	delete[] b;
	delete[] mat;
	return 0;
}