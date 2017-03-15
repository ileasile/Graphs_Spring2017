#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

int N;
int x0i, x1i, y0i, y1i;
char x0s, x1s, y0s, y1s;
bool g[2][100][100];
unsigned char used[100][100][100][13];

typedef tuple<char, char, char, char> vert;

inline bool isset(char p, char i, char j) {
	return i >= 0 && j >= 0 && i < N  && j < N && g[p][i][j];
}

int main() {
	ifstream in("input.txt");
	in >> N >> x0i >> y0i >> x1i >> y1i;
	x0s = x0i; x1s = x1i; y0s = y0i; y1s = y1i;
	for (int p = 0; p < 2; ++p) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				in >> g[p][i][j];
				g[p][i][j] = !g[p][i][j];
			}
		}
	}
	in.close(); 

	for (int i = 0; i < N; ++i) 
		for (int j = 0; j < N; ++j) 
			for (int k = 0; k < N; ++k) 
				for (int l = 0; l < (N >> 3)+1; ++l) 
					used[i][j][k][l] = 0;

	unsigned char mask[8];
	mask[0] = 1;
	for (int i = 1; i < 8; ++i) mask[i] = mask[i - 1] << 1;

	queue<pair<vert, short>> q;
	//map<vert, int> d;

	auto vs = vert(x0s, y0s, x1s, y1s);
	auto fin = vert(0, 0, 0, 0);
	q.emplace(vs, 0);
	used[x0s][y0s][x1s][y1s / 8] |= mask[y1s % 8];

	while (!q.empty()) {
		char x0, x1, y0, y1;
		vert v;
		short d_v;
		tie(v, d_v) = q.front();
		tie(x0, y0, x1, y1) = v;
		q.pop();

		if (v == fin) {
			ofstream out("output.txt");
			out << d_v;
			out.close();
			return 0;
		}

		char lx0 = x0, ly0 = y0, lx1 = x1, ly1 = y1;
		char rx0 = x0, ry0 = y0, rx1 = x1, ry1 = y1;
		char ux0 = x0, uy0 = y0, ux1 = x1, uy1 = y1;
		char dx0 = x0, dy0 = y0, dx1 = x1, dy1 = y1;

		if (isset(0, x0 - 1, y0)) lx0 = x0 - 1;
		if (isset(1, x1 - 1, y1)) lx1 = x1 - 1;
		if (isset(0, x0 + 1, y0)) rx0 = x0 + 1;
		if (isset(1, x1 + 1, y1)) rx1 = x1 + 1;
		if (isset(0, x0, y0 + 1)) uy0 = y0 + 1;
		if (isset(1, x1, y1 + 1)) uy1 = y1 + 1;
		if (isset(0, x0, y0 - 1)) dy0 = y0 - 1;
		if (isset(1, x1, y1 - 1)) dy1 = y1 - 1;

		auto lv = vert(lx0, ly0, lx1, ly1);
		auto rv = vert(rx0, ry0, rx1, ry1);
		auto uv = vert(ux0, uy0, ux1, uy1);
		auto dv = vert(dx0, dy0, dx1, dy1);

		++d_v;
		if (!(used[lx0][ly0][lx1][ly1 / 8] & mask[ly1 % 8])) { used[lx0][ly0][lx1][ly1 / 8] |= mask[ly1 % 8]; q.emplace(lv, d_v); }
		if (!(used[rx0][ry0][rx1][ry1 / 8] & mask[ry1 % 8])) { used[rx0][ry0][rx1][ry1 / 8] |= mask[ry1 % 8]; q.emplace(rv, d_v); }
		if (!(used[ux0][uy0][ux1][uy1 / 8] & mask[uy1 % 8])) { used[ux0][uy0][ux1][uy1 / 8] |= mask[uy1 % 8]; q.emplace(uv, d_v); }
		if (!(used[dx0][dy0][dx1][dy1 / 8] & mask[dy1 % 8])) { used[dx0][dy0][dx1][dy1 / 8] |= mask[dy1 % 8]; q.emplace(dv, d_v); }
	}

	return 0;
}