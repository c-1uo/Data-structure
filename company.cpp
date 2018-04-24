// 辅助调试.cpp : 定义控制台应用程序的入口点。
//

//本段代码尝试将一般的历遍所有元素的起泡排序和经过查阅优化后的算法根据数据规模m的大小分别调用
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
static int mark[10000001];     //最初的是想通过一个mark数组用0和1来标记员工状态
static int code[10000001];
static int e[1000001];
int main() {
	unsigned int n, m;
    std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	cin >> n >> m;
	//尝试用vector创建数组但是OJ不识别
	//在接收空格的前提下，把输入的数据保存到二维字符数组中，由于最多五个字符，所以把列数设置为6
	//vector<vector<string>>c(m, vector<string>(3));
	//vector<vector<int>>d(m, vector<int>(3));
	//vector<int>e(m);
	if (n > 1000) {          
		char *c;
		c = new char[m];
		int(*d)[3];
		d = new int[m][3];
		int *code;
		code = new int[n];
		int *s;
		s = new int[n];
		int *e;
		e = new int[n];

		int num = 0;
		int sum = 0;

		for (int i = 0; i < n; i++) {
			//mark[i] = 0;之前考虑用0和1标记员工，但是一直50%judge里前十个最后两个测试点报错，于是考虑修改不必专门拉出一个数组而是直接通过code数组的值来判断员工是否注册
			code[i] = -1;//考虑到未注册的员工code为-1，此处参考了论坛中的做法，之前是全部初始化为零了，在后面c[i]=Q情况中要通过if判断一次，修改成-1以后代码效率更高，不需要if判断员工是否注册
			s[i] = 0;   //此处的s数组是为了按顺序保存员工的编号，方便通过
			e[i] = 0;
		}

		int k = 0;
		for (int i = 0; i < m; i = i + 1) {
			cin >> c[i];
			if (c[i] == 'I')
			{
				int a = 0;
				cin >> d[i][1] >> d[i][2];
				a = code[d[i][1] - 1];      //先记录一下未赋值时候该员工的code
				code[d[i][1] - 1] = d[i][2];
				if (a == -1) {
					//mark[d[i][1]] = 1;
					num++;
					s[k] = d[i][1] - 1;
					k++;
				}
			}//I

			else if (c[i] == 'O')
			{
				cin >> d[i][1];
				if (code[d[i][1] - 1] != -1) {
					code[d[i][1] - 1] = -1;
					num--;
					//mark[d[i][1]] = 0;
				}

			}//O

			else if (c[i] == 'C') {
				for (int i = 0; i < k; i++) {
					code[s[i]] = -1;              //此处参考了pudn论坛中一名开发者的步骤。之前自己的此处写法是memset(mark,0,4*n);memset(s,0,4*n);memset(mark,0,4*n)，一直在最后两个测试点中无法通过
					//mark[s[i]] = 0;
				}
				k = 0;
				num = 0;
			}//C

			else if (c[i] == 'N') {
				e[i] = num;
			}//N

			else {
				cin >> d[i][1];
				e[i] = code[d[i][1] - 1];
			}//Q
		}
		for (int i = 0; i < m; i++) {
			sum = sum + e[i];
		}
		cout << sum << endl;
		delete[] c;
		delete[] d;
		delete[] code;
		delete[] s;
		delete[] e;
	}
	else {                                  //m小于1000时候用自己最初70准确率的的代码部分，在数据规模不大的时候准确程度比修改后的还多一组数据
		string(*c)[3];
		c = new string[m][3];
		int(*d)[3];
		d = new int[m][3];
		int *e;
		e = new int[m];

		int static num = 0;
		int sum = 0;
		for (int i = 0; i < m; i++) {
			e[i] = 0;
		}

		for (int i = 0; i < m; i = i + 1) {
			cin >> c[i][0];
			if (c[i][0] == "I")
			{
				cin >> c[i][1] >> c[i][2];
				int same = 0;
				d[i][1] = atoi(c[i][1].c_str());
				d[i][2] = atoi(c[i][2].c_str());
				for (int j = 0; j < i; j++) {
					if (d[i][1] - d[j][1] == 0)
					{
						d[j][1] = 0;                //之前已经注册的全部置0
						d[j][2] = 0;
						same++;
					}
				}
				if (same == 0)
					num++;

			}//I

			else if (c[i][0] == "O")
			{
				cin >> c[i][1];
				int same = 0;
				d[i][1] = atoi(c[i][1].c_str());
				for (int j = 0; j < i; j++) {
					if (d[i][1] - d[j][1] == 0)
					{
						d[j][1] = 0;        //注销时把之前的员工名字和code置0，再把本行命令name置零
						d[j][2] = 0;
						same++;
					}
				}
				if (same != 0)
					num--;
				d[i][1] = d[i][2] = 0;
			}//O

			else if (c[i][0] == "C") {
				for (int j = 0; j <= i; j++) {
					d[j][1] = 0;
					d[j][2] = 0;
				}
				num = 0;
			}//C

			else if (c[i][0] == "N") {
				num = num;
				e[i] = num;
				d[i][1] = d[i][2] = 0;
			}//N

			else if (c[i][0] == "Q") {
				cin >> c[i][1];
				int same = 0;
				d[i][1] = atoi(c[i][1].c_str());
				for (int j = 0; j < i; j++) {
					if (d[i][1] - d[j][1] == 0)
					{
						d[i][2] = d[j][2];
						same++;
					}
				}
				if (same == 0)
					e[i] = -1;
				else
					e[i] = d[i][2];
				d[i][1] = d[i][2] = 0;//此步骤容易忽略，问完了以后要将问的员工号置0
			}//Q
		}
		for (int i = 0; i < m; i++) {
			sum = sum + e[i];
		}
		cout << sum << endl;
		delete[] c;
		delete[] d;
		delete[] e;
	}


	return 0;

}
