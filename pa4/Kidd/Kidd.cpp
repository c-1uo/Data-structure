//http://www.xuetangx.com/courses/course-v1:TsinghuaX+70240183x_1+sp/courseware/f21f5d119f0b4189b5fdaf9bcc45714d/70140e0ff13243db8c5910f4ce96552a/
//�ο���mooc�����㼸�Ρ��ڰ���windowing query��segment tree��һ�ڵ���Ƶ
//https://www.cnblogs.com/TheRoadToTheGold/p/6254255.html
//�ο���������ƪ�����ж��������޸�������ǵĽ���
#include <stdio.h>
#include <stdlib.h>
struct segment {//�洢m�д�������߶�
	int left, right;
};
struct segnode {//����segment tree�ڵ�
	int lb, rb;//ÿ���ڵ㶼��¼��һ���߶���������������˵�
	segnode* lc;
	segnode* rc;
	segnode* parent;
	bool op;//��¼��ǰ�ڵ��Ƿ����巭ת������ʼ������Ĭ��δ����ת����Ҳ���������
	int size;//��¼��ǰ�ڵ���˿�������
	int upsize;//��¼��ǰ�˿��Ƶ����泯�ϵ�����
	segnode() { lc = NULL; rc = NULL; parent = NULL; op = false; }
	segnode(int a, int b) { lb = a, rb = b; lc = NULL; rc = NULL; parent = NULL; op = false; }
};
segnode* root = NULL;//��ʼ�����ڵ�
void buildSegtree(int lo, int mi, int hi, segment* elem, segnode* v) {
	if (lo == hi) {//��ֻ��һ��Ԫ�ص�ʱ��Ϊ�ݹ��
		v->lb = elem[lo].left;
		v->rb = elem[lo].right;
		v->size = elem[lo].right - elem[lo].left;
		v->upsize = elem[lo].right - elem[lo].left;//��ʼ����ʱ���˿���ȫ��Ϊ����
		return;
	}
	else {
		int mil = (lo + mi) / 2;//����ȡ��λ��
		int mir = (mi + 1 + hi) / 2;
		segnode* templeft = new segnode();
		v->lc = templeft;//�������ͷ��������
		templeft->parent = v;
		segnode* tempright = new segnode();
		v->rc = tempright;
		tempright->parent = v;
		buildSegtree(lo, mil, mi, elem, v->lc);
		buildSegtree(mi + 1, mir, hi, elem, v->rc);
		v->lb = v->lc->lb;//���¸��׽ڵ���߶���Ϣ���˿��Ƶ�������Ϣ
		v->rb = v->rc->rb;
		v->size = v->lc->size + v->rc->size;
		v->upsize = v->lc->upsize + v->rc->upsize;
		return;
	}
}
bool notcommon(segnode* v, segment* e) {//�ж������߶��Ƿ��н���
	if (e->right <= v->lb || e->left >= v->rb)
		return true;
	else
		return false;
}
bool isbelong(segnode* v, segment* e) {//�жϴ�������߶��Ƿ��ܹ���ȫ��������ѯ�Ľڵ��д洢��elementary interval
	if (v->lb >= e->left&&v->rb <= e->right)
		return true;
	else
		return false;
}
void insert(segnode* v, segment* e) {//��Ӧ��'H'��ͷ������൱�ڰѱ߲嵽�����ˣ�������߶α��ֳ����������߶δ洢�����Ĳ�ͬ�ڵ���
	if (notcommon(v, e))//û�н���ֱ�ӷ���
		return;
	if (isbelong(v, e)) {//�����Ӽ���һ����ʱ�����µ�ǰ�ڵ���˿�����Ϣ
		v->op = !(v->op);
		v->upsize = v->size - v->upsize;
		return;
	}
	if (v->op != false) {//ά�����Ӻ��Һ��ӵ���Ϣ������ά��һ�㡣����ο��˴����ͷ������������������ǵ�ʹ�÷���
		v->lc->op = !(v->lc->op);
		v->rc->op = !(v->rc->op);
		v->lc->upsize = v->lc->size - v->lc->upsize;
		v->rc->upsize = v->rc->size - v->rc->upsize;
		v->op = false;
	}
	insert(v->lc, e);
	insert(v->rc, e);
	v->upsize = v->lc->upsize + v->rc->upsize;//�ݹ鴦������������֮��Ҫ�Ե�ǰ�ڵ���˿�����Ϣ����
}
/*void insert(segnode* v, segment* e) {���ǵ�һ�γ�ʱ�Ĳ���汾����Ϊ����ÿһ���ҵ������߶ζ������ڲ����ʱ��Ե�ǰ�ڵ����µ����нڵ㶼��������Ϣ����
	if (isbelong(v, e)) {//�˻�Ϊ�������㷨
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
void query(segnode* v, segment* e) {//��Ӧ��'Q'��ͷ������൱���ҳ�����ѯ�ı������е���Щ�����Ӽ�
	if (notcommon(v, e))//û�н�����ʱ�򷵻ؿ�
		return;
	if (isbelong(v, e)) {//��������
		num = num + v->upsize;
		return;
	}
	if (v->op != false) {//��Ϊ�����ʱ����������ܰ��������нڵ�ı߾�ֹͣ�ݹ��ˣ����нڵ���·����ӽڵ����Ϣ��û�и��£����ڸ���һ���ٽ����ݹ�
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
	char* order = new char[m + 1];//��%s��ȡ�ַ���ʱ��ҪԤ������ֹ���Ŀռ�
	int* vertex = new int[2 * m];//���m�����������е��߶������˵�
	for (int i = 0; i < m; i++) {
		scanf("%s %d %d", &order[i], &((s + i)->left), &((s + i)->right));
	}
	for (int i = 0; i < m; i++)//��Ϊ�����Ϲ涨���䶼�����ұյģ����Զ��߶ε���߶���һ
		s[i].left--;
	for (int i = 0; i < m; i++) {
		vertex[2 * i] = s[i].left;
		vertex[2 * i + 1] = s[i].right;
	}
	qsort(vertex, 2 * m, sizeof(vertex[0]), cmp);//Ȼ���2*m������ȥ��
	int* unique = new int[2 * m];
	int count = 0;
	for (int i = 0; i < 2 * m;) {
		unique[count++] = vertex[i];
		int j = i;
		while (j < 2 * m&&vertex[j] == vertex[i])
			j++;
		i = j;
	}
	segment* EI = new segment[count - 1];//�õ��������߶�����(elementary interval)��Ҳ�������ұյ�
	for (int i = 0; i < count - 1; i++) {
		EI[i].left = unique[i];
		EI[i].right = unique[i + 1];
	}
	int midpoint = (0 + count - 2) / 2;
	root = new segnode();
	buildSegtree(0, midpoint, count - 2, EI, root);//����
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