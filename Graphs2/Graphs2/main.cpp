#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <tuple>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

int N;
int x0i, x1i, y0i, y1i;
char x0s, x1s, y0s, y1s;
bool g[2][128][128];
unsigned char used[128][128][128][13];


struct vert {
	char x0, y0, x1, y1;
};
struct QueueElem {
	vert v;
	short d;
};
inline bool notset(char p, char i, char j) {
	return i < 0 || j < 0 || i >= N || j >= N || g[p][i][j];
}

int main() {
#ifdef ILEASILE
	auto tm = clock();
#endif // ILEASILE

	ifstream in("input.txt");
	in >> N >> x0i >> y0i >> x1i >> y1i;
	x0s = x0i; x1s = x1i; y0s = y0i; y1s = y1i;
	for (int p = 0; p < 2; ++p) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				in >> g[p][i][j];
			}
		}
	}
	in.close();

	unsigned char mask[8];
	mask[0] = 1;
	for (int i = 1; i < 8; ++i) mask[i] = mask[i - 1] << 1;

	queue<QueueElem> q;

	auto vs = vert{ x0s, y0s, x1s, y1s };
	q.push({ vs, 0 });
	used[x0s][y0s][x1s][y1s / 8] |= mask[y1s % 8];

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
		if (!(used[lx0][qe.v.y0][lx1][qe.v.y1 / 8] & mask[qe.v.y1 % 8])) {
			used[lx0][qe.v.y0][lx1][qe.v.y1 / 8] |= mask[qe.v.y1 % 8];
			q.push({ vert{ lx0, qe.v.y0, lx1, qe.v.y1 }, qe.d });
		}
		if (!(used[rx0][qe.v.y0][rx1][qe.v.y1 / 8] & mask[qe.v.y1 % 8])) {
			used[rx0][qe.v.y0][rx1][qe.v.y1 / 8] |= mask[qe.v.y1 % 8];
			q.push({ vert{ rx0, qe.v.y0, rx1, qe.v.y1 }, qe.d });
		}
		if (!(used[qe.v.x0][uy0][qe.v.x1][uy1 / 8] & mask[uy1 % 8])) {
			used[qe.v.x0][uy0][qe.v.x1][uy1 / 8] |= mask[uy1 % 8];
			q.push({ vert{ qe.v.x0, uy0, qe.v.x1, uy1 }, qe.d });
		}
		if (!(used[qe.v.x0][dy0][qe.v.x1][dy1 / 8] & mask[dy1 % 8])) {
			used[qe.v.x0][dy0][qe.v.x1][dy1 / 8] |= mask[dy1 % 8];
			q.push({ vert{ qe.v.x0, dy0, qe.v.x1, dy1 }, qe.d });
		}
	}

	return 0;
}
