#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <ctime>
#include <cstdlib>
using namespace std;

const short MAXN = 1000;
short N;
FILE *in, *out;

float p[MAXN][MAXN];
short b[MAXN][MAXN];
bool used[MAXN];
short num[MAXN];
short cnt;

void topo_sort() {
	cnt = N;
	used[0] = true;
	stack<pair<short, short>> st;
	st.push(make_pair(0, 0));

	while (!st.empty()) {
		auto & v = st.top();
		for (; v.second < N; ++v.second) {
			if (!used[v.second] && p[v.first][v.second] != 0) {
				st.push(make_pair(v.second, 0));
				used[v.second] = true;
				break;
			}
		}
		if (v.second == N) {
			num[--cnt] = v.first;
			st.pop();
		}
	}
}

short count(short i, short j) {
	if (b[i][j] == -1)
		return 2;
	else
		return count(i, b[i][j]) + count(b[i][j], j)  - 1;
}

void print_route(short i, short j) {
	if(b[i][j] != -1){
		print_route(i, b[i][j]);
		fprintf(out, "%hd ", b[i][j]);
		print_route(b[i][j], j);
	}
}

void task() {
	fscanf(in, "%hd", &N);
	for (short i = 0; i < N; ++i) {
		for(short j = 0; j < N; ++j){
			fscanf(in, "%f", p[i] + j);
			b[i][j] = -1;
		}
	}
	topo_sort();

	for (short i = 0; i < N; ++i) {
		auto ni = num[i];
		for (short k = i + 1; k < N; ++k) {
			auto nk = num[k];
			for (short j = k + 1; j < N; ++j) {
				auto nj = num[j];
				auto mlt = p[ni][nk] * p[nk][nj];
				auto & pij = p[ni][nj];
				if (pij < mlt) {
					pij = mlt;
					b[ni][nj] = nk;
				}
			}
		}
	}

	fprintf(out, "%f %hd\n", p[0][N - 1], count(0, N - 1));
	fprintf(out, "%hd ", 0);
	print_route(0, N - 1);
	fprintf(out, "%hd ", N - 1);
}

int main() {
#ifdef ILEASILE
	auto timer = clock();
#endif // ILEASILE
	in = fopen("input.txt", "r");
	out = fopen("output.txt", "w");
	task();
#ifdef ILEASILE
	fprintf(out, "\n\n ===============\n%f", (clock() - timer) / (double)CLOCKS_PER_SEC);
#endif // ILEASILE
	fclose(in);
	fclose(out);
	return 0;
}
