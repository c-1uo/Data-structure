#include <stdio.h>
/*http://m.blog.csdn.net/wangbingcsu/article/details/49495309
�ڵݹ����rebuild��˼·��һ��ʼ�ǳ����ӣ�������Ҷ�ӽڵ���жϣ�������������ƪ��������µ����۵���¥�п�����һ�������ַ�ʹ��ָ��ǳ��ǳ������д���˵ݹ鴦�����������ı��ʽ����ָ������������Ĵ�С�綨�����˸��������Ĺ�ģ��λ�á����ֶ�if����ж��Ƿ�ΪҶ�ڵ�Ҫ���̫��*/
void rebuild(int *preorder, int *postorder, int *inorder, int n) {//����ָ��͹�ģ���ȶ���ȷ����Χ���ֱ������õݹ��
	if (n == 1) {//��һ����ֻ������һ�����ڵ��û��������ʱ��Ϊ�ݹ��
		inorder[0] = preorder[0];
		return;
	}
	int i=0;
	while(postorder[i]!=preorder[1]){//�ҵ���������и��ڵ��Ӧ�������ĸ��ڵ��λ�ã���¼�������rank
		i++;
	}
	i++;//�����������������������󣬲��Ǹ��ڵ�Ĳ���λ�ã�����Ҫ���ȼ�һ
	inorder[i] = preorder[0];
	rebuild(preorder + 1, postorder, inorder, i);//���ڵ������������ʼλ����Գ�ʼλ������ƶ���1����ģҲ�����i
	rebuild(preorder + i + 1, postorder + i, inorder + i + 1, n - i - 1);//���ڵ������������ʼλ����Գ�ʼ����ƶ���i+1,��ģҲ���n-i-1
}
int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//�ο���pa-book�����û�����ٴ���
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n;
	scanf("%d", &n);
	int *preorder = new int[n];//��̬�ڴ�����Խ�ʡ�ռ�
	int *postorder = new int[n];
	int *inorder = new int[n];
	for (int i = 0; i < n; i++) {
		scanf("%d", &preorder[i]);
	}
	for (int i = 0; i < n; i++) {
		scanf("%d", &postorder[i]);
	}
	rebuild(preorder, postorder, inorder, n);
	for (int i = 0; i < n; i++) {
		printf("%d ", inorder[i]);
	}
	delete[] preorder;
	delete[] postorder;
	delete[] inorder;
	return 0;
}

