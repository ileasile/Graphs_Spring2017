#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

const short MAXN = 1024;
short N;

float p[MAXN][MAXN];
float prob[MAXN];
short b[MAXN];
bool used[MAXN];
short num[MAXN];
short cnt;

template<class T, int SIZE>
class Stack {
	T data[SIZE];
	T * ptr = data - 1;
public:
	inline void push(T && val) {
		*(++ptr) = val;
	}
	inline bool empty() {
		return ptr == data - 1;
	}
	inline T & top() {
		return *ptr;
	}
	inline void pop() {
		--ptr;
	}
};

void topo_sort() {
	cnt = N;
	used[0] = true;
	Stack<pair<short, short>, MAXN> st;
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

short count_route() {
	short cnt = 0;
	short i = 0;
	while (i != N - 1) {
		++cnt;
		i = b[i];
	}
	return cnt + 1;
}

void print_route() {
	short i = 0;
	while (i != N - 1) {
		printf("%hd ", i);
		i = b[i];
	}
	printf("%hd ", i);
}

void task() {
	scanf("%hd", &N);
	for (short i = 0; i < N; ++i) {
		for(short j = 0; j < N; ++j){
			scanf("%f", p[i] + j);
		}
		b[i] = -1;
	}
	topo_sort();

	short i = N - 1;
	for (; num[i] != N - 1; --i);
	short fin = i;
	prob[num[i]] = 1;

	for (--i; i >= 0; --i) {
		auto ni = num[i];
		auto & pni = prob[ni] = 0;
		for (short j = i + 1; j <= fin; ++j) {
			auto nj = num[j];
			auto mlt = p[ni][nj] * prob[nj];
			if (pni < mlt) {
				pni = mlt;
				b[ni] = nj;
			}
		}
	}

	printf("%f %hd\n", prob[0], count_route());
	print_route();
}

int main() {
#ifdef ILEASILE
	auto timer = clock();
#endif // ILEASILE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	task();
#ifdef ILEASILE
	printf("\n\n===============\n%f", (clock() - timer) / (double)CLOCKS_PER_SEC);
#endif // ILEASILE
	fclose(stdin);
	fclose(stdout);
	return 0;
}
