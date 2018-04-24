//https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/string_pm_kmp/pm_kmp.cpp.htm
//https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/string_pm_kmp/pm_kmp_next.cpp.htm
//参考了代码示例列表中的kmp算法以及buildNext的模板
//http://blog.csdn.net/jingsuwen1/article/details/51931464
//参考了博客中的移位循环包含问题的两种思路
#include <stdio.h>
#include <string.h>
char empty[1002] = "";//用来初始化每个节点内部的字符串为空串
struct Node {//用独立链的方法排解散列冲突
	Node *next;
	char entry[1002];//存放每个轨迹的词条
	int index;//记录每个词条找到的能够移位循环祖先的最小的那个秩
	Node();
	void insertafter(Node *p);//插入新节点
};
Node::Node() {//默认构造函数
	strcpy(entry, empty);
	next = NULL;
	index = 0;//造出结点则把秩初始化为查找失败时的返回值
}
void Node::insertafter(Node *p) {
	p->next = next;
	next = p;
}

int Hash(char *str){//根据移位循环后字符串中的字母序之和保持不变这一性质构造的hash函数
	int hash = 0;
	int length = strlen(str);
	for (int i = 0; i < length; i++) {
		hash = hash + str[i] - 'a';
	}
	return hash % 9973;//hash表长度最大的素数为9973(小于10000的)
}

int* buildNext(char* P) {//构造模式串P的next表，参考了代码示例列表中的模板
	size_t m = strlen(P), j = 0;
	int* N = new int[m];
	int t = N[0] = -1;
	while (j < m - 1)
		if (0 > t || P[j] == P[t]) { 
			j++; t++;
			N[j] = t; 
		}
		else 
			t = N[t];
	return N;
}
bool match(char* P, char* T) {//KMP算法，参考了代码示例列表中的模板
	int* next = buildNext(P); 
	int n = (int)strlen(T), i = 0; 
	int m = (int)strlen(P), j = 0; 
	while (j < m  && i < n) 
		if (0 > j || T[i] == P[j])
		{
			i++;  j++;
		}
		else 
			j = next[j]; 
	delete[] next;
	if (i - j - n + m > 0)//当i-j>n-m的时候为匹配失败
		return false;
	else
		return true;
}

Node node[10001];//用作类似于寻秩访问的节点
Node sup[10001];//用作后续插入的节点
int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//使用了pa-book中的加大缓冲区的代码
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n, m;
	scanf("%d %d", &n, &m);//车轮周长n,m条轨迹
	int pos = 0;//用来记录后续新插入节点的数量
	char str[1002];
	for (int i = 0; i < m; i++) {
		scanf("%s", str);
		int key = Hash(str);
		if (strcmp(node[key].entry, empty) == 0) {//桶的第一个槽位如果为空，则先在该节点上赋值
			printf("0\n");
			strcpy(node[key].entry, str);
			node[key].index = i + 1;
		}
		else {//第一个槽位已经非空的情况,则要从该节点开始对每个槽位逐一比对
			Node *p = &node[key];
			bool exist = false;
			for (p; p != NULL; p = p->next) {
				char text[2005];
				int lengthT = (int)strlen(p->entry);//构造一个新的文本串为T+T，在新串中匹配模式串P，匹配成功说明可以移位循环得到。
				for (int j = 0; j < lengthT; j++)
					text[j] = p->entry[j];
				for (int j = lengthT; j < 2 * lengthT; j++)
					text[j] = p->entry[j - lengthT];
				text[2 * lengthT] = '\0';
				if (match(str, text) == true) {
					exist = true;
					printf("%d\n", p->index);//成功则返回文本串的那个秩
					break;
				}
			}
			if (exist == false) {//桶中所有槽位均无法匹配时则需要新开辟一个槽位
				printf("0\n");
				strcpy(sup[pos].entry, str);//挂载新节点
				sup[pos].index = i + 1;
				(node[key]).insertafter(&sup[pos]);
				pos++;
			}
		}
	}
	return 0;
}