//https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/string_pm_kr/pm_kr.h.htm
//�ο��˴���ʾ���б���Karp-Rabin�㷨��ģ�壬����Ԥ�����hashֵ�ĸ���
//ɢ��ʱ����ѡȡ131��1000000007�������һ��ϰ����ϲο��˶������̵Ľ���
//ʹ������hash�����ֱ����hashֵ�������ܱ���ɢ�г�ͻʱ����Ƚ��ַ����ַ��Ĳ�������һ��Ҳ�ǲο����һ��ϰ����ϵ���ʾ

//https://www.cnblogs.com/zinthos/p/3899725.html
//�Ķ�����ƪ���ͣ�ѧϰ�˺�׺��������֪ʶ
//https://wenku.baidu.com/view/ed1be61e10a6f524ccbf85fd.html
//�Ķ��ˡ���׺���顪�����ַ������������ߡ���ƪ2009��Ĺ��Ҽ�ѵ�����ģ�ѧϰ�˺�׺�����Ӧ��
//ʹ�������������еĵ�6ҳ��17ҳ�ı����㷨�ͼ���height����Ĵ���
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 41011
#define R 29
#define checkM 1000000007//�����checkM��checkR��ȡֵ�ο��˶�������ϰ����ϵĽ���
#define checkR 131
#define DIGIT(S, i) ( (S)[i] - 'a'+1 )

#define maxn 40050

//ǰ�벿��Ϊɢ������
typedef long long HashCode;
int rankofright;
int m;//Ҫ����ַ������ֵ����ٴ���
struct Node {
	int cnt;//��¼�ڵ���ִ���
	int checknum;//��¼����һ��У���hash�������������У���õ�hashֵ
	Node* next;
	Node() { cnt = 0; checknum = -1; next = NULL; }
};
Node hashnode[M];//��������Ͱ
Node sup[M];//�����ã����ں������صĽڵ�
int pos;//��¼����������ȥ�ڵ�ĸ���

HashCode checkDm;//������¼�����õ�131��x-1�η���ģ��checkM��ֵ
HashCode prepareDm(size_t x) {//�൱�ڼ���27��x-1�η���ģ��M��ֵ
	HashCode Dm = 1;
	checkDm = 1;
	for (size_t i = 1; i < x; i++) {
		Dm = (Dm*R) % M;
		checkDm = (checkDm*checkR) % checkM;
	}
	return Dm;
}

void updateHash(HashCode& hashT, HashCode& checkhashT, char* T, size_t x, size_t k, HashCode Dm) {//����Ԥ������Ϣ�����ٸ���hashֵ
	hashT = (hashT - DIGIT(T, k - 1)*Dm) % M;
	hashT = (hashT*R + DIGIT(T, k + x - 1)) % M;
	if (hashT < 0) hashT += M;
	checkhashT = (checkhashT - DIGIT(T, k - 1)*checkDm) % checkM;
	checkhashT = (checkhashT*checkR + DIGIT(T, k + x - 1)) % checkM;
	if (checkhashT < 0) checkhashT += checkM;
}

bool workable1(char* T, int x, int len) {//��鳤��Ϊx�������Ӵ�
	pos = 0;
	rankofright = -1;
	for (int i = 0; i < M; i++) {//ɢ�б��ʼ������������Ͱ����Ϊ��
		hashnode[i].cnt = 0;
		hashnode[i].next = NULL;
		sup[i].cnt = 0;
		sup[i].next = NULL;
	}
	HashCode Dm, hashT = 0, checkhashT = 0;
	Dm = prepareDm(x);
	for (size_t i = 0; i < x; i++) {//��һ������Ϊx���Ӵ���hashֵ
		hashT = (hashT*R + DIGIT(T, i)) % M;
		checkhashT = (checkhashT*checkR + DIGIT(T, i)) % checkM;
	}
	hashnode[hashT].cnt++;
	hashnode[hashT].checknum = checkhashT;
	if (hashnode[hashT].cnt >= m)
		rankofright = 0;
	for (size_t k = 1; k < len - x + 1; k++) {
		updateHash(hashT, checkhashT, T, x, k, Dm);//����hashֵ
		if (hashnode[hashT].cnt == 0) {//���Ͽ�Ͱֱ�Ӳ���
			hashnode[hashT].cnt++;
			hashnode[hashT].checknum = checkhashT;
			if (hashnode[hashT].cnt >= m)
				rankofright = k;
		}
		else {//����Ͱ�ǿյ��������Ҫ�����½ڵ�
			bool exist = false;
			Node* p = hashnode + hashT;
			for (p; p != NULL; p = p->next) {
				if (p->checknum == checkhashT) {//�������У���hashֵҲ�������Ϊ�ַ�����ȫƥ�䣬��һ��ʵ�������з��յ�
					p->cnt++;
					rankofright = p->cnt >= m ? k : rankofright;
					exist = true;
					break;//һ��ƥ���������ѭ��
				}
			}
			if (exist == false) {//��û�п�ƥ�������Ҫ�����µĽڵ�
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

//��벿��Ϊ��׺��������
int r[maxn], sa[maxn];//r�Ǽ�¼���ַ���ת����27�������ֵ����飬sa�Ǻ�׺����

int cmp(int *r, int a, int b, int l) {
	return r[a] == r[b] && r[a + l] == r[b + l];
}

int wa[maxn], wb[maxn], wv[maxn], wt[maxn];
void da(int n, int m) {//��һ�������㷨�������calheight�������ο������ģ������㷨���Ե�6ҳ
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
void calheight(int n) {//�ο������ĵĵ�17ҳ
	int i, j, k = 0;
	for (i = 1; i <= n; i++) ranks[sa[i]] = i;
	for (i = 0; i < n; height[ranks[i++]] = k)
		for (k ? k-- : 0, j = sa[ranks[i] - 1]; r[i + k] == r[j + k]; k++);
	return;
}

int mostRightRank;//��¼���ҳ��ֵ������������ȵ����ֵ
int RightRank;
bool workable2(int lengthT, int k, int x) {//�����Ӵ�Ҫ�����ٳ���k�ε�������У���Ӵ�����Ϊxʱ���Ƿ����Ҫ��
	bool exist = false;
	int cnt = 1;
	mostRightRank = -1;//��ʼ��Ϊ-1����У��ʧ�ܵ����
	RightRank = sa[1];
	for (int i = 2; i <= lengthT; i++) {
		if (height[i] >= x) {
			cnt++;//sa[i]��s[i-1]�������ǰ׺���ȴ��ڵ���xʱ�������������Ӵ�������һ
			RightRank = RightRank >= sa[i] ? RightRank : sa[i];//���´�ʱ��ָ��λ�ã�ȡ�ϴ�ֵ
			if (cnt >= k) {
				exist = true;//һ������������Ҫ����У��ɹ�
				mostRightRank = mostRightRank >= RightRank ? mostRightRank : RightRank;//������ҵ�����������ָ��λ��
			}
		}
		else {
			cnt = 1;//lcp�ĳ��Ȳ�����Ҫ��ʱ��˵��ǰ׺������Ҫ����Ҫ���¼���
			RightRank = sa[i];
		}
	}
	return exist;
}

int binSearch2(int lengthT, int lo, int hi, int k) {//���Ӵ��ĳ��Ƚ��ж������������Ƿ��������������Ӵ����������Ǹ���������ʧ�ܻ᷵��0
	while (lo < hi) {
		int mi = (lo + hi) >> 1;
		workable2(lengthT, k, mi) == true ? lo = mi + 1 : hi = mi;
	}
	return --lo;
}

void preprocess(char* T, int lengthT) {//Ԥ����
	memset(r, 0, sizeof(r));//��ʼ��r����
	for (int i = 0; i < lengthT; i++)//���ַ�����ÿ����ĸ��Ӧ��27�����µ�һ������
		r[i] = T[i] - 'a' + 1;
}

int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);//ʹ����pa-book�е�setvbuf�����Ĵ���
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int tasknum;
	scanf("%d", &tasknum);
	for (int i = 0; i < tasknum; i++) {
		int randnum = rand() % 2;//��ÿ���������ѡ��ɢ�л��Ǻ�׺����ķ����������
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
			if (k == 1)//��������k=1���������Ϊheight�����е����ֵ���ᳬ��length-1.��У��length��һ�Ӵ����ȵ�ֵ��ʱ��ᴦ���У��ʧ�ܵ�������߽������������
				printf("%d 0\n", length);
			else {
				int finallen = binSearch2(length, 1, length + 1, k);//�ҵ�����Ҫ�����󴮳�
				if (finallen == 0)//����ʧ�ܵ����
					printf("none\n");
				else {//���ҳɹ������
					workable2(length, k, finallen);
					printf("%d %d\n", finallen, mostRightRank);
				}
			}
		}
	}
	return 0;
}