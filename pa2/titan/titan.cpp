#include <cstdio>
using namespace std;
//先写一个满足基本操作的栈的类
class stack {
public:
	int *a;//用来存储进栈的元素
	int *b;//辅助数组用来记录当前栈里最大元素的值，每压入一个值就和之前的最大值比较，取更大的赋值
	int pos;
	int top();
	stack(int m);
	void push(int e);
	int pop();
	int getmax();//在栈的成员函数中提供一个查找最大值的接口。为了使得每次查找的时间从O(n)的复杂度降低为O(1),参考了《编程之美》一书中3.7队列中取最大值操作问题的解法，http://ishare.iask.sina.com.cn/f/18359238.html
    ~stack();
	bool empty();
};
//构造函数
stack::stack(int m) :pos(0) { 
	a = new int[m + 1];
	b = new int[m + 1];
}
//压栈
void stack::push(int e) {
	a[++pos] = e;
	if (pos == 1)
		b[pos] = e;
	else
		b[pos] = e > b[pos - 1] ? e : b[pos - 1];//压栈的过程中对辅助数组b中的元素进行更新，记录最大值
}
//弹出栈顶
int stack::pop(){
	int peek = a[pos];
	--pos;
	return peek;
}
//析构函数
stack::~stack() { 
	delete[] a; 
	delete[] b;
}
//取顶
int stack::top(){
	return a[pos];
}
//取最大值
int stack::getmax() {
	return b[pos];
}
//判断栈是否为空
bool stack::empty() {
	if (pos == 0)
		return true;
	else
		return false;
}

int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//参考pa-book中I/O缓存加速的代码
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n;//记录命令行数
	int abi;//记录能力值
	scanf("%d", &n);
	stack s1(n), s2(n);
	char empty;
	scanf("%c", &empty);
	char order;
	for (int i = 0; i < n; i++) {
		scanf("%c", &order);
		if (order == 'E') {
			scanf("%d", &abi);
			s1.push(abi);
			//实现求栈s1的最大值
		}
		else if (order == 'M') {
			if (s2.empty() == true) {
				while (s1.empty() == false) {
					s2.push(s1.pop());
				}
			}
			if (s1.empty() == true) {
				printf("%d\n", s2.getmax());
			}
			else {
				int max1 = s1.getmax();
				int max2 = s2.getmax();
				int max = max1 > max2 ? max1 : max2;
				printf("%d\n", max);//由于取最值没有队列的插入或者出列，因此只要比较构成队列的两个栈的最大值，并且取较大的那个返回
			}
		}	
		else {
			if (s2.empty() == true) {
				while (s1.empty()==false) {
					s2.push(s1.pop());
				}
			}
			printf("%d\n", s2.pop());//出队列相当于元素在两个栈中转移后，队首从栈底变成了栈顶，只需要pop出转以后的栈顶元素即可
		}
		scanf("%c", &empty);
	}
	return 0;
}

