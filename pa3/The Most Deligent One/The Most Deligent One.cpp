//https://baike.baidu.com/item/hash%E5%87%BD%E6%95%B0/10555888?fr=
//�ο��˰ٶȰٿƵ�ɢ�н���
//http://blog.csdn.net/djinglan/article/details/8812934
//�ο��˾����ַ����ļ���hash����
//https://www.cnblogs.com/ldy-miss/p/6099454.html
//ѡ����BKDRhash����
#include <stdio.h>
#include <string.h>
char empty[9] = "";//ȫ�ֱ���������Ĭ�Ϲ��캯���г�ʼ��������
class Node {
private:
	Node *next;
public:
	char name[9];//���ÿ���ڵ������
	int times;//��¼ÿ���������ֵĴ���
	Node();
	Node(char *_name);
	void insertafter(Node *p);
	Node *nextnode();
};
Node::Node() {//Ĭ�Ϲ��캯��
	strcpy(name, empty);
	next = nullptr;
	times = 1;//���������ʼ����Ϊ1
}
Node::Node(char *p) {
	strcpy(name, p);
	next = nullptr;
	times = 1;//���������ʼ����Ϊ1
}
void Node::insertafter(Node *p) {
	p->next = next;
	next = p;
}
Node* Node::nextnode() {
	return next;
}

unsigned int BKDRHash(char *str)//����ʹ���˲ο����������е�BKDRhash����
{
	unsigned int seed = 131313;//Ҳ���Գ���31��131��1313��13131��131313..
	unsigned int hash = 0;
	while (*str)
	{
		hash = hash*seed + (*str++);
	}
	return hash % 9973;//hash������������Ϊ9973(С��10000��)
}


Node node[10001];//����������Ѱ�ȷ��ʵĽڵ�
Node sup[10001];//������������Ľڵ�

int main()
{
	setvbuf(stdin,new char[1<<20],_IOFBF,1<<20);//ʹ����pa-book�еļӴ󻺳����Ĵ���
	setvbuf(stdout,new char[1<<20],_IOFBF,1<<20);
	int n;
	char str[9];
	scanf("%d", &n);
	int pos = 0;//������¼�����²���ڵ������
	int max = 0;//��¼�����ִ���
	char maxname[9] = "";//��¼����������������
	for (int i = 0; i < n; i++) {
		scanf("%s", str);
		int key = (int)BKDRHash(str);
		if (strcmp(node[key].name, empty) == 0) {//���ڵ����Ϊ�գ������ڸ��ڵ��ϸ�ֵ��������ȻΪ1���Ķ�
			strcpy(node[key].name, str);
			if (node[key].times > max) {
				max = node[key].times;
				strcpy(maxname, node[key].name);
			}
		}
		else {//���ڵ����Ѿ��ǿյ����,��Ҫ�Ӹ��ڵ㿪ʼ��һ�ȶ�
			if (strcmp(node[key].name, str) == 0) {//������ڵ�ֱ���ҵ�
				node[key].times++;
				if (node[key].times > max) {
					max = node[key].times;
					strcpy(maxname, node[key].name);
				}
			}
			else {//���ڵ�û�еĻ���Ҫ˳�Ÿ��ڵ�һֱ�ҵ�nextΪ��
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
				if (exist == false) {//���û���ҵ�ͬ���ģ�����һ���µĽڵ�
					strcpy(sup[pos].name, str);//�����½ڵ��ʱ�����ڱ�Ȼ��Ӧ�������ͷ�Ѿ�Ԫ�طǿգ���������ִ���һ����С��1�����ø������ֵ��Ϣ
					(*temp).insertafter(&sup[pos]);
					pos++;
				}
			}
		}
	}
	printf("%s %d", maxname, max);
	return 0;
}

