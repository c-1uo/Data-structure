//https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/pq_complheap/pq_complheap_macro.h.htm
//使用了课程代码示例列表中的宏定义写法，并且建堆和下滤的函数名也是保持和讲义中一致
//参考了提示中的"注意根据内存限制设计合适的读入数据方案",结合fread能大块读取数据的特点进行了读入方案的设计。
#include <stdio.h>
#define  Parent(i)         ( ( i - 1 ) >> 1 )
#define  LChild(i)         ( 1 + ( ( i ) << 1 ) )
#define  RChild(i)         ( ( 1 + ( i ) ) << 1 )
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#define max(a,b)    (((a) > (b)) ? (a) : (b))
bool isLeaf(int n, int i) {//判断是否为叶子节点，左孩子一旦越界那么右孩子一定越界
	if ((i > -1) && (i < n) && (2 * i + 1 > n - 1))//首先当前节点要在堆中
		return true;
	else
		return false;

}
int maxinchild(int n, int i, int* elem) {//找出左右子堆的根节点中键码较大的那个
	if (RChild(i) >= n)//只有左孩子时
		return elem[LChild(i)];
	else//左右孩子都有的时候
		return max(elem[LChild(i)], elem[RChild(i)]);
}
void swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}
void percolateDown(int n, int i, int* elem) {//下滤，返回值是下滤的终点的秩
	while (isLeaf(n, i) == false && elem[i]<maxinchild(n,i,elem)) {//进入循环有两个条件，不是叶子并且满足孩子中有更小的
		if (elem[LChild(i)] == maxinchild(n, i, elem)) {
			swap(elem[i], elem[LChild(i)]);
			i = LChild(i);
		}
		else {
			swap(elem[i], elem[RChild(i)]);
			i = RChild(i);
		}
	}
	return;
}
void heapify(int n,int* elem) {//自下而上的下滤，建堆
	for (int i = (n - 1 - 1) / 2; i >= 0; i--) {
		percolateDown(n, i, elem);
	}
	return;
}
int main()
{
	//setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//这里参考了pa-book中的setvbuf加大缓冲区的代码
	//setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n, k;
	fread(&n, sizeof(int), 1, stdin);
	fread(&k, sizeof(int), 1, stdin);
	int* data = new int[k];//只维护这k个数字，建立一个大根堆
	fread(data, sizeof(int), k, stdin);
	heapify(k, data);
	int* left = new int[100000];//因为剩下的数据规模可能很大，为了减少从外存上读取数据的时间，可以一口气先读一大块进来，因为有内存限制，所以取100000
	int pos = 0;
	fread(left, sizeof(int), 100000, stdin);
	for (int i = k; i < n; i++) {
		int temp;
		if (pos < 100000)
			temp = left[pos++];
		else {//如果当前这一段读完了就再大块地读100000个整数进来
			fread(left, sizeof(int), 100000, stdin);
			pos = 0;
			temp = left[pos++];
		}
		if (temp < data[0]) {//新元素比堆中最大的元素要小，就把堆顶删了把新元素加进来做一次下滤维护堆的性质
			data[0] = temp;
			percolateDown(k, 0, data);
		}
	}
	printf("%d", data[0]);
	delete[] data;
	delete[] left;
	return 0;
}