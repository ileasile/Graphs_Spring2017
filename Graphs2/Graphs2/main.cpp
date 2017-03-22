#define _CRT_SECURE_NO_WARNINGS
#define notset(p, i, j) (i < 0 || j < 0 || i >= N || j >= N || g[p][i][j])
#define set_used(x0, y0, x1, y1) (used[x0][y0][x1][y1 / 8] |= mask[y1 % 8])
#define not_used(x0, y0, x1, y1) (!(used[x0][y0][x1][y1 / 8] & mask[y1 % 8]))

#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <tuple>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;
struct vert {
	char x0, y0, x1, y1;
};
struct QueueElem {
	vert v;
	short d;
};

int N;
int x0i, x1i, y0i, y1i;
char x0s, x1s, y0s, y1s;
unsigned char used[128][128][128][13];
bool g[2][128][128];
unsigned char mask[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
queue<QueueElem> q;

inline void set_if_not_used(char & x0, char & y0, char & x1, char & y1, short & d) {
	if (not_used(x0, y0, x1, y1)) {
		set_used(x0, y0, x1, y1);
		q.push({ vert{ x0, y0, x1, y1 }, d });
	}
}

int main() {
#ifdef ILEASILE
	auto tm = clock();
#endif // ILEASILE

	ifstream in("input.txt");
	in >> N >> x0i >> y0i >> x1i >> y1i;
	x0s = x0i; x1s = x1i; y0s = y0i; y1s = y1i;
	for (int p = 0; p < 2; ++p)
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
				in >> g[p][i][j];
	in.close();

	auto vs = vert{ x0s, y0s, x1s, y1s };
	q.push({ vs, 0 });
	set_used(x0s, y0s, x1s, y1s);
	
	while (!q.empty()) {
		QueueElem qe;
		qe = q.front();
		q.pop();

		if (*((unsigned *)(&(qe.v))) == 0) {
			ofstream out("output.txt");
			out << qe.d;
#ifdef ILEASILE
			out << "\n\n============\n" << (double)(clock() - tm) / CLOCKS_PER_SEC;
#endif // ILEASILE
			out.close();
			return 0;
		}

		char lx0 = qe.v.x0 - 1, lx1 = qe.v.x1 - 1;
		char rx0 = qe.v.x0 + 1, rx1 = qe.v.x1 + 1;
		char uy0 = qe.v.y0 + 1, uy1 = qe.v.y1 + 1;
		char dy0 = qe.v.y0 - 1, dy1 = qe.v.y1 - 1;

		if (notset(0, lx0, qe.v.y0)) lx0 = qe.v.x0;
		if (notset(1, lx1, qe.v.y1)) lx1 = qe.v.x1;
		if (notset(0, rx0, qe.v.y0)) rx0 = qe.v.x0;
		if (notset(1, rx1, qe.v.y1)) rx1 = qe.v.x1;
		if (notset(0, qe.v.x0, uy0)) uy0 = qe.v.y0;
		if (notset(1, qe.v.x1, uy1)) uy1 = qe.v.y1;
		if (notset(0, qe.v.x0, dy0)) dy0 = qe.v.y0;
		if (notset(1, qe.v.x1, dy1)) dy1 = qe.v.y1;

		++qe.d;
		set_if_not_used(lx0, qe.v.y0, lx1, qe.v.y1, qe.d);
		set_if_not_used(rx0, qe.v.y0, rx1, qe.v.y1, qe.d);
		set_if_not_used(qe.v.x0, uy0, qe.v.x1, uy1, qe.d);
		set_if_not_used(qe.v.x0, dy0, qe.v.x1, dy1, qe.d);
	}

	return 0;
}
