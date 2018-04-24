//https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/string_pm_kmp/pm_kmp.cpp.htm
//https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/string_pm_kmp/pm_kmp_next.cpp.htm
//�ο��˴���ʾ���б��е�kmp�㷨�Լ�buildNext��ģ��
//http://blog.csdn.net/jingsuwen1/article/details/51931464
//�ο��˲����е���λѭ���������������˼·
#include <stdio.h>
#include <string.h>
char empty[1002] = "";//������ʼ��ÿ���ڵ��ڲ����ַ���Ϊ�մ�
struct Node {//�ö������ķ����Ž�ɢ�г�ͻ
	Node *next;
	char entry[1002];//���ÿ���켣�Ĵ���
	int index;//��¼ÿ�������ҵ����ܹ���λѭ�����ȵ���С���Ǹ���
	Node();
	void insertafter(Node *p);//�����½ڵ�
};
Node::Node() {//Ĭ�Ϲ��캯��
	strcpy(entry, empty);
	next = NULL;
	index = 0;//����������ȳ�ʼ��Ϊ����ʧ��ʱ�ķ���ֵ
}
void Node::insertafter(Node *p) {
	p->next = next;
	next = p;
}

int Hash(char *str){//������λѭ�����ַ����е���ĸ��֮�ͱ��ֲ�����һ���ʹ����hash����
	int hash = 0;
	int length = strlen(str);
	for (int i = 0; i < length; i++) {
		hash = hash + str[i] - 'a';
	}
	return hash % 9973;//hash������������Ϊ9973(С��10000��)
}

int* buildNext(char* P) {//����ģʽ��P��next���ο��˴���ʾ���б��е�ģ��
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
bool match(char* P, char* T) {//KMP�㷨���ο��˴���ʾ���б��е�ģ��
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
	if (i - j - n + m > 0)//��i-j>n-m��ʱ��Ϊƥ��ʧ��
		return false;
	else
		return true;
}

Node node[10001];//����������Ѱ�ȷ��ʵĽڵ�
Node sup[10001];//������������Ľڵ�
int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//ʹ����pa-book�еļӴ󻺳����Ĵ���
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n, m;
	scanf("%d %d", &n, &m);//�����ܳ�n,m���켣
	int pos = 0;//������¼�����²���ڵ������
	char str[1002];
	for (int i = 0; i < m; i++) {
		scanf("%s", str);
		int key = Hash(str);
		if (strcmp(node[key].entry, empty) == 0) {//Ͱ�ĵ�һ����λ���Ϊ�գ������ڸýڵ��ϸ�ֵ
			printf("0\n");
			strcpy(node[key].entry, str);
			node[key].index = i + 1;
		}
		else {//��һ����λ�Ѿ��ǿյ����,��Ҫ�Ӹýڵ㿪ʼ��ÿ����λ��һ�ȶ�
			Node *p = &node[key];
			bool exist = false;
			for (p; p != NULL; p = p->next) {
				char text[2005];
				int lengthT = (int)strlen(p->entry);//����һ���µ��ı���ΪT+T�����´���ƥ��ģʽ��P��ƥ��ɹ�˵��������λѭ���õ���
				for (int j = 0; j < lengthT; j++)
					text[j] = p->entry[j];
				for (int j = lengthT; j < 2 * lengthT; j++)
					text[j] = p->entry[j - lengthT];
				text[2 * lengthT] = '\0';
				if (match(str, text) == true) {
					exist = true;
					printf("%d\n", p->index);//�ɹ��򷵻��ı������Ǹ���
					break;
				}
			}
			if (exist == false) {//Ͱ�����в�λ���޷�ƥ��ʱ����Ҫ�¿���һ����λ
				printf("0\n");
				strcpy(sup[pos].entry, str);//�����½ڵ�
				sup[pos].index = i + 1;
				(node[key]).insertafter(&sup[pos]);
				pos++;
			}
		}
	}
	return 0;
}