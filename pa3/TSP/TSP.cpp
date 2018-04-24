//http://blog.csdn.net/jnu_simba/article/details/8866844
//�ο�������������������ڽӱ����ͼ�Ĵ洢�ṹ
//https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/graph/graph_dfs.h.htm
//�����е����������ǲ��տγ̽����Լ�����ʾ���б�����д
//�ο����ĵ���ʾ�еĲ��������еĳ��ж�������һ�仰������ͷ��β�����������ڵ�ʹ������ͼ��ͨ
//�����˽���06 - I5��Dijkstraʵ�ִ����д����������������õ��������������ÿ�����е��յ��׬Ǯ�����ֵ
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
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define INT_MAX 2147483647 
#define INT_MIN (-INT_MAX - 1)
typedef enum { UNDISCOVERED, DISCOVERED, VISITED }VStatus;
VStatus status[100005];//���������ʱ���¼�������еı����ʵ�״̬
struct Edge {
	int b, e, w;//�����յ��Ȩ��
	Edge(int _b, int _e, int _w) :b(_b), e(_e), w(_w) {}//�����±߲�����Ҫ�����Ĺ��캯��
	Edge() { b = 0; e = 0; w = 0; }//Ĭ�Ϲ��캯��
};
int cmp(const void *a, const void *b) {
	Edge *c = (Edge *)a;
	Edge *d = (Edge *)b;
	if ((*c).b < (*d).b)//�������������ͬ���з���ȥ�Ķ����С��С����ôdfs��ʱ���Ӵ�С
		return -1;
	else if ((*c).b == (*d).b) {
		if ((*c).e < (*d).e)
			return -1;
		else if ((*c).e == (*d).e) {
			if ((*c).w >(*d).w)//Ȩ�ذ��ս������б���֮��ȥ�ص�ʱ��ѡ��Ȩ������
				return -1;
			else
				return 1;
		}
		else
			return 1;
	}
	else
		return 1;
}
struct EdgeNode {//���ڽӱ���ʵ��,���Ǳ߱�ڵ㣬�ο���http://blog.csdn.net/jnu_simba/article/details/8866844
	EdgeNode *next;//ָ����һ���߱�ڵ�
	int num;//��¼�ھӳ��еı��
	int weight;//��¼�ߵ�Ȩ��
	EdgeNode() { next = nullptr; }
	EdgeNode(int x, int y, EdgeNode *e) { num = x; weight = y; next = e; }
};
struct VertexNode {//�����ڵ�
	EdgeNode *firstNbr;//��һ���ھ�
	VertexNode() { firstNbr = nullptr; }//Ĭ�Ϲ���ʱ��û�й���
};
VertexNode city[100005];
Edge a[1000050];
Edge b[1300000];//b�п���Ҫ�ӽ������������ıߣ����Ҫ�����ֵ����������ҵĳ�������
int in[100005];//��¼����ÿ�����е����
int out[100005];//��¼����ÿ�����еĳ���
int stack[100005];//������������ʱ���Ǹ�"ջ"
int stackpos;
void Tsort(int v) {//���ս�����DFS��ʵ��
	status[v] = DISCOVERED;
	for (EdgeNode *x = city[v].firstNbr; x != nullptr; x = x->next) {
		int u = x->num;
		switch (status[u]) {
		case UNDISCOVERED:
			Tsort(u); break;
		default:
			break;
		}
	}
	status[v] = VISITED;
	stack[stackpos++] = v;//��Ŵ���ھ��ȵݹ��Ƚ�ջ����˴���ھ���Ȼ��push
}
void DFS(int v,int &clock) {


}
int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//�ο���pa-book�е����󻺳����Ĵ���
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	read(0, input, 1 << 25);
	int n, m;
	//scanf("%d %d", &n, &m);//n���㣬m����
	n = getnum(); m = getnum();
	for (int i = 0; i < m; i++) {
		//scanf("%d %d %d", &a[i].b, &a[i].e, &a[i].w);
		a[i].b = getnum(); a[i].e = getnum(); a[i].w = getnum();
		out[a[i].b]++;//���ĳ��ȼ�һ
		in[a[i].e]++;//�յ����ȼ�һ
	}
	qsort(a, m, sizeof(a[0]), cmp);//��������һ�£�������ȥ��
	int k = 0;//��¼ȥ�غ��ж�������
	for (int i = 0; i < m; ) {//ѭ��������ʱ��k��¼��ȥ�غ�ߵ�����
		int j = i;
		b[k] = a[j]; k++;
		while (j < m&&a[j].b == a[i].b&&a[j].e == a[i].e) {
			j++;
		}
		i = j;
	}
	for (int i = 1; i <= n; i++) {//�ҵ�û����ȵĵ㶼�ͱ��Ϊ0�ĵ�������Ȩ������Ϊ0��������֤������ͼ������ͨ�ģ����ں�������С�ֵ����ҵ�������Ҫ�Զ����ͨͼ�е���׬Ǯ·�����бȽ�
		if (in[i] == 0) {
			b[k].b = 0; b[k].e = i; b[k].w = 0;
			k++;
		}
		if (out[i] == 0) {//�ҵ�û�г��ȵĵ㶼�ͱ��Ϊn+1�ĵ�������Ȩ��Ҳ����Ϊ0���ο���doc�ĵ��еĵ�һ����ʾ
			b[k].b = i; b[k].e = n + 1; b[k].w = 0;
			k++;
		}
	}
	qsort(b, k, sizeof(b[0]), cmp);//�Բ���ͼ�����б߽������������ͬ���յ㣬�յ㻹��ͬ�Ƚ�Ȩ�أ�����ȥ���ˣ���һ����ʵ����������յ���ͬ���������ʱһ����k����
	for (int i = 0; i < k; i++) {
		if (city[b[i].b].firstNbr == nullptr)
			city[b[i].b].firstNbr = new EdgeNode(b[i].e, b[i].w, nullptr);
		else {
			EdgeNode *temp = new EdgeNode(b[i].e, b[i].w, city[b[i].b].firstNbr);//���Խ����ھӻ�Խ��ǰ����˿�ʼ����ݹ��ʱ���ȱ��С���ھ���
			city[b[i].b].firstNbr = temp;
		}
	}
	Tsort(0);//��0�ų��п�ʼ�������򣬽��һ����0�ų����ڵ�һλ��n+1�ų��������һλ
	int *Tsorted = new int[n + 2];
	for (int i = 0; i < n + 2; i++) {
		Tsorted[i] = stack[--stackpos];
	}
	int *priority = new int[n + 2];//��������˽���06-I5��Dijkstraʵ�ִ����д����������������priority��ʾÿ�����е��յ��׬Ǯ�����ֵ��������Сֵ
	for (int i = 0; i < n + 2; i++) {
		priority[i] = INT_MIN;//��ʼ�����ǡ������
	}
	priority[Tsorted[n + 1]] = 0;
	for (int i = n; i > -1; i--) {//�������е��ھӣ����ϸ��µ�ǰ���е��յ���·������
		for (EdgeNode *x = city[Tsorted[i]].firstNbr; x != nullptr; x = x->next)
			priority[Tsorted[i]] = max(priority[Tsorted[i]], priority[x->num] + x->weight);
	}
	int *finalpath = new int[n + 2];//��¼�ֵ�����С��·��
	int q = 0;//��¼·����Ԫ�صĸ���
	finalpath[q++] = 0;
	for (int i = 0; i < n + 1;) {
		int temp;
		for (EdgeNode *x = city[i].firstNbr; x != nullptr; x = x->next) {//�������е��ھӣ��������������е����յ�׬Ǯ���ֵ�Ĳ�����ھ�֮���׬Ǯ������Ҫ���и���
			if (priority[i] == priority[x->num] + x->weight)//�ھ����ڽӱ��϶���������صģ����Ҫ�ҵ���С�ֵ�����Ǹ���������������ھ�
				temp = x->num;
		}//���յ�Ҳ��¼������ѭ����ֹ
		finalpath[q++] = temp;
		i = temp;
	}
	for (int i = 1; i < q - 1; i++)
		printf("%d ", finalpath[i]);
	//printf("%d", priority[0]);//���0�ų��е����׬Ǯ���ܹ����3-1-1TSP��һ��
	delete[] Tsorted;
	delete[] priority;
	delete[] finalpath;
	return 0;
}