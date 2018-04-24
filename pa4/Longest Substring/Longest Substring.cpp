//https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/string_pm_kr/pm_kr.h.htm
//参考了代码示例列表中Karp-Rabin算法的模板，包括预处理和hash值的更新
//散列时基数选取131和1000000007是在最后一次习题课上参考了丁铭助教的建议
//使用两个hash函数分别计算hash值来尽可能避免散列冲突时逐个比较字符串字符的操作，这一步也是参考最后一次习题课上的提示

//https://www.cnblogs.com/zinthos/p/3899725.html
//阅读了这篇博客，学习了后缀数组的相关知识
//https://wenku.baidu.com/view/ed1be61e10a6f524ccbf85fd.html
//阅读了《后缀数组—处理字符串的有力工具》这篇2009年的国家集训队论文，学习了后缀数组的应用
//使用了上述论文中的第6页和17页的倍增算法和计算height数组的代码
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 41011
#define R 29
#define checkM 1000000007//这里的checkM和checkR的取值参考了丁铭助教习题课上的建议
#define checkR 131
#define DIGIT(S, i) ( (S)[i] - 'a'+1 )

#define maxn 40050

//前半部分为散列做法
typedef long long HashCode;
int rankofright;
int m;//要求的字符串出现的最少次数
struct Node {
	int cnt;//记录节点出现次数
	int checknum;//记录用另一个校验的hash函数计算出来的校验用的hash值
	Node* next;
	Node() { cnt = 0; checknum = -1; next = NULL; }
};
Node hashnode[M];//独立链的桶
Node sup[M];//辅助用，用于后续挂载的节点
int pos;//记录后续挂载上去节点的个数

HashCode checkDm;//用来记录辅助用的131的x-1次方后模余checkM的值
HashCode prepareDm(size_t x) {//相当于计算27的x-1次方后模余M的值
	HashCode Dm = 1;
	checkDm = 1;
	for (size_t i = 1; i < x; i++) {
		Dm = (Dm*R) % M;
		checkDm = (checkDm*checkR) % checkM;
	}
	return Dm;
}

void updateHash(HashCode& hashT, HashCode& checkhashT, char* T, size_t x, size_t k, HashCode Dm) {//利用预处理信息，快速更新hash值
	hashT = (hashT - DIGIT(T, k - 1)*Dm) % M;
	hashT = (hashT*R + DIGIT(T, k + x - 1)) % M;
	if (hashT < 0) hashT += M;
	checkhashT = (checkhashT - DIGIT(T, k - 1)*checkDm) % checkM;
	checkhashT = (checkhashT*checkR + DIGIT(T, k + x - 1)) % checkM;
	if (checkhashT < 0) checkhashT += checkM;
}

bool workable1(char* T, int x, int len) {//检查长度为x的所有子串
	pos = 0;
	rankofright = -1;
	for (int i = 0; i < M; i++) {//散列表初始化，独立链的桶均置为空
		hashnode[i].cnt = 0;
		hashnode[i].next = NULL;
		sup[i].cnt = 0;
		sup[i].next = NULL;
	}
	HashCode Dm, hashT = 0, checkhashT = 0;
	Dm = prepareDm(x);
	for (size_t i = 0; i < x; i++) {//第一个长度为x的子串的hash值
		hashT = (hashT*R + DIGIT(T, i)) % M;
		checkhashT = (checkhashT*checkR + DIGIT(T, i)) % checkM;
	}
	hashnode[hashT].cnt++;
	hashnode[hashT].checknum = checkhashT;
	if (hashnode[hashT].cnt >= m)
		rankofright = 0;
	for (size_t k = 1; k < len - x + 1; k++) {
		updateHash(hashT, checkhashT, T, x, k, Dm);//更新hash值
		if (hashnode[hashT].cnt == 0) {//遇上空桶直接插入
			hashnode[hashT].cnt++;
			hashnode[hashT].checknum = checkhashT;
			if (hashnode[hashT].cnt >= m)
				rankofright = k;
		}
		else {//遇上桶非空的情况则需要挂载新节点
			bool exist = false;
			Node* p = hashnode + hashT;
			for (p; p != NULL; p = p->next) {
				if (p->checknum == checkhashT) {//如果用于校验的hash值也相等则认为字符串完全匹配，这一步实际上是有风险的
					p->cnt++;
					rankofright = p->cnt >= m ? k : rankofright;
					exist = true;
					break;//一旦匹配可以跳出循环
				}
			}
			if (exist == false) {//若没有可匹配的则需要挂载新的节点
				sup[pos].cnt++;
				rankofright = sup[pos].cnt >= m ? k : rankofright;
				sup[pos].checknum = checkhashT;
				(sup + pos)->next = (hashnode + hashT)->next;
				(hashnode + hashT)->next = sup + pos;
				pos++;
			}
		}
	}
	if (rankofright != -1)
		return true;
	else
		return false;
}

int binSearch1(char* T, int lo, int hi, int len) {
	while (lo < hi) {
		int mi = (lo + hi) >> 1;
		workable1(T, mi, len) == true ? lo = mi + 1 : hi = mi;
	}
	return --lo;
}

//后半部分为后缀数组做法
int r[maxn], sa[maxn];//r是记录将字符串转化成27进制数字的数组，sa是后缀数组

int cmp(int *r, int a, int b, int l) {
	return r[a] == r[b] && r[a + l] == r[b + l];
}

int wa[maxn], wb[maxn], wv[maxn], wt[maxn];
void da(int n, int m) {//这一步倍增算法和下面的calheight函数均参考了论文，倍增算法来自第6页
	int i, j, p, *x = wa, *y = wb, *t;
	for (i = 0; i < m; i++) wt[i] = 0;
	for (i = 0; i < n; i++) wt[x[i] = r[i]]++;
	for (i = 1; i < m; i++) wt[i] += wt[i - 1];
	for (i = n - 1; i >= 0; i--) sa[--wt[x[i]]] = i;
	for (j = 1, p = 1; p < n; j *= 2, m = p) {
		for (p = 0, i = n - j; i < n; i++) y[p++] = i;
		for (i = 0; i < n; i++) if (sa[i] >= j) y[p++] = sa[i] - j;
		for (i = 0; i < n; i++) wv[i] = x[y[i]];
		for (i = 0; i < m; i++) wt[i] = 0;
		for (i = 0; i < n; i++) wt[wv[i]]++;
		for (i = 1; i < m; i++) wt[i] += wt[i - 1];
		for (i = n - 1; i >= 0; i--) sa[--wt[wv[i]]] = y[i];
		for (t = x, x = y, y = t, p = 1, x[sa[0]] = 0, i = 1; i < n; i++)
			x[sa[i]] = cmp(y, sa[i - 1], sa[i], j) ? p - 1 : p++;
	}
	return;
}

int ranks[maxn], height[maxn];
void calheight(int n) {//参考自论文的第17页
	int i, j, k = 0;
	for (i = 1; i <= n; i++) ranks[sa[i]] = i;
	for (i = 0; i < n; height[ranks[i++]] = k)
		for (k ? k-- : 0, j = sa[ranks[i] - 1]; r[i + k] == r[j + k]; k++);
	return;
}

int mostRightRank;//记录最右出现的满足条件的秩的最大值
int RightRank;
bool workable2(int lengthT, int k, int x) {//对于子串要求至少出现k次的条件，校验子串长度为x时候是否符合要求
	bool exist = false;
	int cnt = 1;
	mostRightRank = -1;//初始化为-1，即校验失败的情况
	RightRank = sa[1];
	for (int i = 2; i <= lengthT; i++) {
		if (height[i] >= x) {
			cnt++;//sa[i]和s[i-1]的最长公共前缀长度大于等于x时候，满足条件的子串数量加一
			RightRank = RightRank >= sa[i] ? RightRank : sa[i];//更新此时的指针位置，取较大值
			if (cnt >= k) {
				exist = true;//一旦总数满足了要求则校验成功
				mostRightRank = mostRightRank >= RightRank ? mostRightRank : RightRank;//更新最靠右的满足条件的指针位置
			}
		}
		else {
			cnt = 1;//lcp的长度不满足要求时候说明前缀不满足要求，需要重新计数
			RightRank = sa[i];
		}
	}
	return exist;
}

int binSearch2(int lengthT, int lo, int hi, int k) {//对子串的长度进行二分搜索，总是返回满足条件的子串长度最大的那个。若查找失败会返回0
	while (lo < hi) {
		int mi = (lo + hi) >> 1;
		workable2(lengthT, k, mi) == true ? lo = mi + 1 : hi = mi;
	}
	return --lo;
}

void preprocess(char* T, int lengthT) {//预处理
	memset(r, 0, sizeof(r));//初始化r数组
	for (int i = 0; i < lengthT; i++)//将字符串中每个字母对应于27进制下的一个数字
		r[i] = T[i] - 'a' + 1;
}

int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//使用了pa-book中的setvbuf函数的代码
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int tasknum;
	scanf("%d", &tasknum);
	for (int i = 0; i < tasknum; i++) {
		int randnum = rand() % 2;//对每次任务随机选择散列或是后缀数组的方法进行求解
		if (randnum == 0) {
			char temp[40002];
			scanf("%d %s", &m, temp);
			int lengthT = (int)strlen(temp);
			int lo = 1;
			int hi = lengthT + 1;
			int finallen = binSearch1(temp, lo, hi, lengthT);
			if (finallen == 0)
				printf("none\n");
			else {
				workable1(temp, finallen, lengthT);
				printf("%d %d\n", finallen, rankofright);
			}
		}
		else {
			char str[40050];
			int k;
			scanf("%d %s", &k, str);
			int length = strlen(str);
			preprocess(str, length);
			da(length + 1, 27);
			calheight(length);
			if (k == 1)//单独处理k=1的情况，因为height数组中的最大值不会超过length-1.在校验length这一子串长度的值的时候会处理成校验失败的情况。边界情况单独处理
				printf("%d 0\n", length);
			else {
				int finallen = binSearch2(length, 1, length + 1, k);//找到满足要求的最大串长
				if (finallen == 0)//查找失败的情况
					printf("none\n");
				else {//查找成功的情况
					workable2(length, k, finallen);
					printf("%d %d\n", finallen, mostRightRank);
				}
			}
		}
	}
	return 0;
}