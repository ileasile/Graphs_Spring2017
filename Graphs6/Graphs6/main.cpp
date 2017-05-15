#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;

char N;
short K, N2;

struct Point {
	char x, y, l;
	Point() {
		x = y = l = 0;
	}
	inline int get_num() {
		return x*N + y + (l ? N2 : 0);
	}
	inline bool correct() {
		return x >= 0 && y >= 0 && x < N && y < N;
	}
};

struct Edge {
	short a, b;
	char cap, flow;
	Edge(short a, short b, char cap, char flow) :a(a), b(b), cap(cap), flow(flow){}
};

vector<Point> sv, tv;
short n, s, t;
vector<int> pred;
vector<Edge> e;
map<short, short> sv_rev;
vector<vector<int>> g;

void input() {
	scanf("%hhd %hd", &N, &K);
	sv.resize(K);
	tv.resize(K);
	for (auto & l : sv) { l.l = 0; scanf("%hhd", &(l.x)); }
	for (auto & l : sv) scanf("%hhd", &(l.y));
	for (auto & l : tv) { l.l = 1; scanf("%hhd", &(l.x)); }
	for (auto & l : tv) scanf("%hhd", &(l.y));

	N2 = short(N)*N;
	n = N2 * 2 + 2;
	g.resize(n);
	pred.resize(n);
}

void add_edge(short a, short b) {
	g[a].push_back((int)e.size());
	e.emplace_back( a, b, 1, 0 );
	g[b].push_back((int)e.size());
	e.emplace_back( b, a, 0, 0 );
}

void buid_graph() {
	Point pt;
	for (pt.x = 0; pt.x < N; ++pt.x) {
		for (pt.y = 0; pt.y < N; ++pt.y) {
			Point to_pt = pt;
			to_pt.l = 1;
			add_edge(pt.get_num(), to_pt.get_num());
		}
	}

	pt.l = 1;
	char sh[4][2] = { { 1, 0 },{ -1, 0 },{ 0, 1 },{ 0, -1 } };
	for (pt.x = 0; pt.x < N; ++pt.x) {
		for (pt.y = 0; pt.y < N; ++pt.y) {
			Point pt_zero = pt;
			pt_zero.l = 0;
			for (int k = 0; k < 4; ++k) {
				Point to_pt = pt_zero;
				to_pt.x += sh[k][0];
				to_pt.y += sh[k][1];
				if (to_pt.correct())
					add_edge(pt.get_num(), to_pt.get_num());
			}
		}
	}

	s = N2 << 1;
	t = s + 1;

	int i = -1;
	for (auto & svert : sv) {
		auto num = svert.get_num();
		sv_rev[num] = ++i;
		add_edge(s, num);
	}
	for (auto & tvert : tv) {
		add_edge(tvert.get_num(), t);
	}
}

bool bfs() {
	queue<short> q;
	q.push(s);
	pred.assign(n, -1);
	pred[s] = 0;
	while (!q.empty() && pred[t] == -1) {
		int v = q.front();
		q.pop();
		for (auto id : g[v]) {
			int to = e[id].b;
			if (pred[to] == -1 && e[id].flow < e[id].cap) {
				q.push(to);
				pred[to] = id;
			}
		}
	}
	return pred[t] != -1;
}

void increase_flow() {
	int id;
	short b = t;
	do {
		id = pred[b];
		b = e[id].a;
		++e[id].flow;
		--e[id ^ 1].flow;
	} while (b != s);
}

void edmonds_karp() {
	for (; bfs(); --K)
		increase_flow();
}

void print_paths() {
	vector<vector<short>> paths(sv.size());

	for (auto & id : g[s]) {
		short cv = e[id].b;
		auto & path = paths[sv_rev[cv]];
		while (cv != t) {
			if(cv < N2)
				path.push_back(cv);
			for (auto id : g[cv]) {
				if (e[id].flow == 1) {
					cv = e[id].b;
					break;
				}
			}
		}
	}

	for (auto & p : paths) {
		for (auto vert : p) {
			printf("%d ", vert);
		}
		printf("\n");
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	input();
	buid_graph();
	edmonds_karp();
	if (K)
		printf("-1");
	else
		print_paths();

	fclose(stdin);
	fclose(stdout);
	return 0;
}
