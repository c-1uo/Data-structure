//https://baike.baidu.com/item/hash%E5%87%BD%E6%95%B0/10555888?fr=
//参考了百度百科的散列介绍
//http://blog.csdn.net/djinglan/article/details/8812934
//参考了经典字符串的几种hash函数
//https://www.cnblogs.com/ldy-miss/p/6099454.html
//选择了BKDRhash函数
#include <stdio.h>
#include <string.h>
char empty[9] = "";//全局变量用来给默认构造函数中初始化数组用
class Node {
private:
	Node *next;
public:
	char name[9];//存放每个节点的姓名
	int times;//记录每个姓名出现的次数
	Node();
	Node(char *_name);
	void insertafter(Node *p);
	Node *nextnode();
};
Node::Node() {//默认构造函数
	strcpy(name, empty);
	next = nullptr;
	times = 1;//造出结点则初始次数为1
}
Node::Node(char *p) {
	strcpy(name, p);
	next = nullptr;
	times = 1;//造出结点则初始次数为1
}
void Node::insertafter(Node *p) {
	p->next = next;
	next = p;
}
Node* Node::nextnode() {
	return next;
}

unsigned int BKDRHash(char *str)//这里使用了参考资料中现有的BKDRhash函数
{
	unsigned int seed = 131313;//也可以乘以31、131、1313、13131、131313..
	unsigned int hash = 0;
	while (*str)
	{
		hash = hash*seed + (*str++);
	}
	return hash % 9973;//hash表长度最大的素数为9973(小于10000的)
}


Node node[10001];//用作类似于寻秩访问的节点
Node sup[10001];//用作后续插入的节点

int main()
{
	setvbuf(stdin,new char[1<<20],_IOFBF,1<<20);//使用了pa-book中的加大缓冲区的代码
	setvbuf(stdout,new char[1<<20],_IOFBF,1<<20);
	int n;
	char str[9];
	scanf("%d", &n);
	int pos = 0;//用来记录后续新插入节点的数量
	int max = 0;//记录最大出现次数
	char maxname[9] = "";//记录出现最多次数的人名
	for (int i = 0; i < n; i++) {
		scanf("%s", str);
		int key = (int)BKDRHash(str);
		if (strcmp(node[key].name, empty) == 0) {//根节点如果为空，则先在根节点上赋值，次数仍然为1不改动
			strcpy(node[key].name, str);
			if (node[key].times > max) {
				max = node[key].times;
				strcpy(maxname, node[key].name);
			}
		}
		else {//根节点上已经非空的情况,则要从根节点开始逐一比对
			if (strcmp(node[key].name, str) == 0) {//如果根节点直接找到
				node[key].times++;
				if (node[key].times > max) {
					max = node[key].times;
					strcpy(maxname, node[key].name);
				}
			}
			else {//根节点没有的话则要顺着根节点一直找到next为空
				Node* temp = &node[key];
				bool exist = false;
				while ((*temp).nextnode() != nullptr) {
					temp = (*temp).nextnode();
					if (strcmp((*temp).name, str) == 0) {
						exist = true;
						(*temp).times++;
						if ((*temp).times > max) {
							max = (*temp).times;
							strcpy(maxname, (*temp).name);
						}
						break;
					}
				}
				if (exist == false) {//如果没有找到同名的，插入一个新的节点
					strcpy(sup[pos].name, str);//挂载新节点的时候，由于必然相应的链表表头已经元素非空，因此最大出现次数一定不小于1，不用更新最大值信息
					(*temp).insertafter(&sup[pos]);
					pos++;
				}
			}
		}
	}
	printf("%s %d", maxname, max);
	return 0;
}

