#include <cstdio>
int main() {
	setvbuf ( stdin , new char [1 << 20] , _IOFBF, 1 << 20);
    setvbuf ( stdout , new char [1 << 20] , _IOFBF, 1 << 20);
	int n, m;
	scanf("%d %d", &n, &m);
	int *A = new int[n + 1];
	int *B = new int[n + 1];
	int *S = new int[m + 1];
	int *num = new int[3 * n];
	char *way = new char[3 * n];
	int k = 0;/*记录第一次循环的次数*/
	bool able = true;
	A[0] = 0;
	B[0] = 0;
	S[0] = 0;
	int topa = 1, topb = 0, tops = 0;
	for (int i = 1; i < n + 1; i++) {
		scanf("%d", &A[i]);
	}
	while (topa < (n + 1)) {
		bool a = (A[topa] == B[topb] + 1);
		bool b = (S[tops] == B[topb] + 1);
		bool c = (A[topa] != B[topb] + 1) && (S[tops] != B[topb] + 1);
		if (a==true) {
			B[topb + 1] = A[topa];
			k++;
			num[k] = A[topa];
			way[k] = 'P';
			topb++;
			topa++;
		}
		if (b==true) {
			B[topb + 1] = S[tops];
			k++;
			num[k] = S[tops];
			way[k] = 'O';
			topb++;
			tops--;
		}
		if(c==true) {
			if (tops == m) {
				able = false;
				break;
			}
			else {
				S[tops + 1] = A[topa];
				k++;
				num[k] = A[topa];
				way[k] = 'I';
				tops++;
				topa++;
			}
		}
	}
	//校验一下第一轮循环结果
	/*for (int i = 1; i < topb + 1; i++) {
		printf("%d ", B[i]);
	}
	printf("\n");
	for (int i = 1; i < tops + 1; i++) {
		printf("%d ", S[i]);
	}
	printf("\n");*/
	if (able == false) {
		printf("No");
	}
	else {
		bool sorted = true;
		int temp;
		for (int i = 1; i < tops; i++) {
			if (S[i] < S[i + 1]) {
				temp = S[i];
				S[i] = S[i + 1];
				S[i + 1] = temp;
				sorted = false;
			}
		}
		if (((S[tops] == B[topb] + 1) && (sorted == true))||(tops==0) == true) {
			for (int i = k + 1; i <= k + tops; i++) {
				num[i] = S[1 + tops - i + k];
				way[i] = 'O';
			}
			for (int i = 1; i <= k + tops; i++) {
				printf("%d %c\r\n", num[i], way[i]);
			}
		}
		else
			printf("No");
	}
	delete[] A;
	delete[] B;
	delete[] S;
	delete[] num;
	delete[] way;
	return 0;
}
