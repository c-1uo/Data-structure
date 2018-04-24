#include <cstdio>
using namespace std;
//��дһ���������������ջ����
class stack {
public:
	int *a;//�����洢��ջ��Ԫ��
	int *b;//��������������¼��ǰջ�����Ԫ�ص�ֵ��ÿѹ��һ��ֵ�ͺ�֮ǰ�����ֵ�Ƚϣ�ȡ����ĸ�ֵ
	int pos;
	int top();
	stack(int m);
	void push(int e);
	int pop();
	int getmax();//��ջ�ĳ�Ա�������ṩһ���������ֵ�Ľӿڡ�Ϊ��ʹ��ÿ�β��ҵ�ʱ���O(n)�ĸ��ӶȽ���ΪO(1),�ο��ˡ����֮����һ����3.7������ȡ���ֵ��������Ľⷨ��http://ishare.iask.sina.com.cn/f/18359238.html
    ~stack();
	bool empty();
};
//���캯��
stack::stack(int m) :pos(0) { 
	a = new int[m + 1];
	b = new int[m + 1];
}
//ѹջ
void stack::push(int e) {
	a[++pos] = e;
	if (pos == 1)
		b[pos] = e;
	else
		b[pos] = e > b[pos - 1] ? e : b[pos - 1];//ѹջ�Ĺ����жԸ�������b�е�Ԫ�ؽ��и��£���¼���ֵ
}
//����ջ��
int stack::pop(){
	int peek = a[pos];
	--pos;
	return peek;
}
//��������
stack::~stack() { 
	delete[] a; 
	delete[] b;
}
//ȡ��
int stack::top(){
	return a[pos];
}
//ȡ���ֵ
int stack::getmax() {
	return b[pos];
}
//�ж�ջ�Ƿ�Ϊ��
bool stack::empty() {
	if (pos == 0)
		return true;
	else
		return false;
}

int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//�ο�pa-book��I/O������ٵĴ���
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n;//��¼��������
	int abi;//��¼����ֵ
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
			//ʵ����ջs1�����ֵ
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
				printf("%d\n", max);//����ȡ��ֵû�ж��еĲ�����߳��У����ֻҪ�ȽϹ��ɶ��е�����ջ�����ֵ������ȡ�ϴ���Ǹ�����
			}
		}	
		else {
			if (s2.empty() == true) {
				while (s1.empty()==false) {
					s2.push(s1.pop());
				}
			}
			printf("%d\n", s2.pop());//�������൱��Ԫ��������ջ��ת�ƺ󣬶��״�ջ�ױ����ջ����ֻ��Ҫpop��ת�Ժ��ջ��Ԫ�ؼ���
		}
		scanf("%c", &empty);
	}
	return 0;
}

