#include <stdio.h>
/*http://m.blog.csdn.net/wangbingcsu/article/details/49495309
在递归代码rebuild的思路上一开始非常复杂，处理不好叶子节点的判断，所给链接中这篇博客最底下的评论第三楼中看到了一个处理手法使用指针非常非常巧妙地写好了递归处理左右子树的表达式！用指针和左右子树的大小界定出来了各个子树的规模和位置。比手动if语句判断是否为叶节点要简洁太多*/
void rebuild(int *preorder, int *postorder, int *inorder, int n) {//传递指针和规模，既对齐确定范围，又便于设置递归基
	if (n == 1) {//当一个树只有自身一个根节点而没有子树的时候为递归基
		inorder[0] = preorder[0];
		return;
	}
	int i=0;
	while(postorder[i]!=preorder[1]){//找到后序遍历中根节点对应左子树的根节点的位置，记录下其的秩rank
		i++;
	}
	i++;//在中序遍历中左子树遍历完后，才是根节点的插入位置，所以要将秩加一
	inorder[i] = preorder[0];
	rebuild(preorder + 1, postorder, inorder, i);//根节点的左子树的起始位置相对初始位置向后移动了1，规模也变成了i
	rebuild(preorder + i + 1, postorder + i, inorder + i + 1, n - i - 1);//根节点的右子树的起始位置相对初始向后移动了i+1,规模也变成n-i-1
}
int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//参考了pa-book中设置缓存加速代码
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n;
	scanf("%d", &n);
	int *preorder = new int[n];//动态内存分配以节省空间
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

