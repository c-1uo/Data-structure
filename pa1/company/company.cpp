// ��������.cpp : �������̨Ӧ�ó������ڵ㡣
//

//���δ��볢�Խ�һ�����������Ԫ�ص���������;��������Ż�����㷨�������ݹ�ģm�Ĵ�С�ֱ����
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
static int mark[10000001];     //���������ͨ��һ��mark������0��1�����Ա��״̬
static int code[10000001];
static int e[1000001];
int main() {
	unsigned int n, m;
    std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	cin >> n >> m;
	//������vector�������鵫��OJ��ʶ��
	//�ڽ��տո��ǰ���£�����������ݱ��浽��ά�ַ������У������������ַ������԰���������Ϊ6
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
			//mark[i] = 0;֮ǰ������0��1���Ա��������һֱ50%judge��ǰʮ������������Ե㱨�����ǿ����޸Ĳ���ר������һ���������ֱ��ͨ��code�����ֵ���ж�Ա���Ƿ�ע��
			code[i] = -1;//���ǵ�δע���Ա��codeΪ-1���˴��ο�����̳�е�������֮ǰ��ȫ����ʼ��Ϊ���ˣ��ں���c[i]=Q�����Ҫͨ��if�ж�һ�Σ��޸ĳ�-1�Ժ����Ч�ʸ��ߣ�����Ҫif�ж�Ա���Ƿ�ע��
			s[i] = 0;   //�˴���s������Ϊ�˰�˳�򱣴�Ա���ı�ţ�����ͨ��
			e[i] = 0;
		}

		int k = 0;
		for (int i = 0; i < m; i = i + 1) {
			cin >> c[i];
			if (c[i] == 'I')
			{
				int a = 0;
				cin >> d[i][1] >> d[i][2];
				a = code[d[i][1] - 1];      //�ȼ�¼һ��δ��ֵʱ���Ա����code
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
					code[s[i]] = -1;              //�˴��ο���pudn��̳��һ�������ߵĲ��衣֮ǰ�Լ��Ĵ˴�д����memset(mark,0,4*n);memset(s,0,4*n);memset(mark,0,4*n)��һֱ������������Ե����޷�ͨ��
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
	else {                                  //mС��1000ʱ�����Լ����70׼ȷ�ʵĵĴ��벿�֣������ݹ�ģ�����ʱ��׼ȷ�̶ȱ��޸ĺ�Ļ���һ������
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
						d[j][1] = 0;                //֮ǰ�Ѿ�ע���ȫ����0
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
						d[j][1] = 0;        //ע��ʱ��֮ǰ��Ա�����ֺ�code��0���ٰѱ�������name����
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
				d[i][1] = d[i][2] = 0;//�˲������׺��ԣ��������Ժ�Ҫ���ʵ�Ա������0
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
