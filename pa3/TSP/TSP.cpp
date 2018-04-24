//http://blog.csdn.net/jnu_simba/article/details/8866844
//参考了上述博客中如何用邻接表完成图的存储结构
//https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/graph/graph_dfs.h.htm
//代码中的拓扑排序是参照课程讲义以及代码示例列表中书写
//参考了文档提示中的并不是所有的城市都相连这一句话，引入头和尾这两个辅助节点使得整个图连通
//仿照了讲义06 - I5中Dijkstra实现代码的写法，按照拓扑排序得到的序列逆向求出每个城市到终点的赚钱的最大值
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
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define INT_MAX 2147483647 
#define INT_MIN (-INT_MAX - 1)
typedef enum { UNDISCOVERED, DISCOVERED, VISITED }VStatus;
VStatus status[100005];//拓扑排序的时候记录各个城市的被访问的状态
struct Edge {
	int b, e, w;//起点和终点和权重
	Edge(int _b, int _e, int _w) :b(_b), e(_e), w(_w) {}//构造新边并且需要参数的构造函数
	Edge() { b = 0; e = 0; w = 0; }//默认构造函数
};
int cmp(const void *a, const void *b) {
	Edge *c = (Edge *)a;
	Edge *d = (Edge *)b;
	if ((*c).b < (*d).b)//这里如果按照相同城市发出去的顶点从小到小，那么dfs的时候会从大到小
		return -1;
	else if ((*c).b == (*d).b) {
		if ((*c).e < (*d).e)
			return -1;
		else if ((*c).e == (*d).e) {
			if ((*c).w >(*d).w)//权重按照降序排列便于之后去重的时候选择权重最大的
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
struct EdgeNode {//用邻接表来实现,这是边表节点，参考了http://blog.csdn.net/jnu_simba/article/details/8866844
	EdgeNode *next;//指向下一个边表节点
	int num;//记录邻居城市的编号
	int weight;//记录边的权重
	EdgeNode() { next = nullptr; }
	EdgeNode(int x, int y, EdgeNode *e) { num = x; weight = y; next = e; }
};
struct VertexNode {//顶点表节点
	EdgeNode *firstNbr;//第一个邻居
	VertexNode() { firstNbr = nullptr; }//默认构造时候没有挂载
};
VertexNode city[100005];
Edge a[1000050];
Edge b[1300000];//b中可能要加进来额外数量的边，因此要比最大值多出两倍左右的城市数量
int in[100005];//记录输入每个城市的入度
int out[100005];//记录输入每个城市的出度
int stack[100005];//用作拓扑排序时的那个"栈"
int stackpos;
void Tsort(int v) {//参照讲义中DFS的实现
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
	stack[stackpos++] = v;//编号大的邻居先递归先进栈，因此大的邻居自然后push
}
void DFS(int v,int &clock) {


}
int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//参考了pa-book中的扩大缓冲区的代码
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	read(0, input, 1 << 25);
	int n, m;
	//scanf("%d %d", &n, &m);//n个点，m条边
	n = getnum(); m = getnum();
	for (int i = 0; i < m; i++) {
		//scanf("%d %d %d", &a[i].b, &a[i].e, &a[i].w);
		a[i].b = getnum(); a[i].e = getnum(); a[i].w = getnum();
		out[a[i].b]++;//起点的出度加一
		in[a[i].e]++;//终点的入度加一
	}
	qsort(a, m, sizeof(a[0]), cmp);//先排序处理一下，接下来去重
	int k = 0;//记录去重后有多少条边
	for (int i = 0; i < m; ) {//循环结束的时候k记录了去重后边的条数
		int j = i;
		b[k] = a[j]; k++;
		while (j < m&&a[j].b == a[i].b&&a[j].e == a[i].e) {
			j++;
		}
		i = j;
	}
	for (int i = 1; i <= n; i++) {//找到没有入度的点都和编号为0的点相连，权重设置为0，这样保证了整个图都是连通的，便于后续的最小字典序找到，否则还要对多个连通图中的最赚钱路径进行比较
		if (in[i] == 0) {
			b[k].b = 0; b[k].e = i; b[k].w = 0;
			k++;
		}
		if (out[i] == 0) {//找到没有出度的点都和编号为n+1的点相连，权重也设置为0，参考了doc文档中的第一行提示
			b[k].b = i; b[k].e = n + 1; b[k].w = 0;
			k++;
		}
	}
	qsort(b, k, sizeof(b[0]), cmp);//对插入图的所有边进行排序，起点相同比终点，终点还相同比较权重，由于去重了，这一步其实不会有起点终点相同的情况。此时一共有k条边
	for (int i = 0; i < k; i++) {
		if (city[b[i].b].firstNbr == nullptr)
			city[b[i].b].firstNbr = new EdgeNode(b[i].e, b[i].w, nullptr);
		else {
			EdgeNode *temp = new EdgeNode(b[i].e, b[i].w, city[b[i].b].firstNbr);//编号越大的邻居会越靠前，因此开始进入递归的时候会比编号小的邻居早
			city[b[i].b].firstNbr = temp;
		}
	}
	Tsort(0);//从0号城市开始拓扑排序，结果一定是0号城市在第一位，n+1号城市在最后一位
	int *Tsorted = new int[n + 2];
	for (int i = 0; i < n + 2; i++) {
		Tsorted[i] = stack[--stackpos];
	}
	int *priority = new int[n + 2];//这里仿照了讲义06-I5中Dijkstra实现代码的写法，区别在于这里priority表示每个城市到终点的赚钱的最大值而不是最小值
	for (int i = 0; i < n + 2; i++) {
		priority[i] = INT_MIN;//初始化都是“负无穷”
	}
	priority[Tsorted[n + 1]] = 0;
	for (int i = n; i > -1; i--) {//遍历所有的邻居，不断更新当前城市到终点的最长路径长度
		for (EdgeNode *x = city[Tsorted[i]].firstNbr; x != nullptr; x = x->next)
			priority[Tsorted[i]] = max(priority[Tsorted[i]], priority[x->num] + x->weight);
	}
	int *finalpath = new int[n + 2];//记录字典序最小的路径
	int q = 0;//记录路径中元素的个数
	finalpath[q++] = 0;
	for (int i = 0; i < n + 1;) {
		int temp;
		for (EdgeNode *x = city[i].firstNbr; x != nullptr; x = x->next) {//遍历所有的邻居，若满足两个城市到达终点赚钱最大值的差等于邻居之间的赚钱数，则要进行更新
			if (priority[i] == priority[x->num] + x->weight)//邻居在邻接表上都是逆序挂载的，因此要找到最小字典序的那个必须访问完所有邻居
				temp = x->num;
		}//在终点也记录下来后循环终止
		finalpath[q++] = temp;
		i = temp;
	}
	for (int i = 1; i < q - 1; i++)
		printf("%d ", finalpath[i]);
	//printf("%d", priority[0]);//输出0号城市的最大赚钱数能够解决3-1-1TSP这一题
	delete[] Tsorted;
	delete[] priority;
	delete[] finalpath;
	return 0;
}