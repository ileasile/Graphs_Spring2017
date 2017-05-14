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
	Point(short num) {
		if (l = num >= N2)
			num -= N2;
		x = num / N;
		y = num % N;
	}
	int get_num() {
		return x*N + y + (l ? N2 : 0);
	}
	inline bool correct() {
		return x >= 0 && y >= 0 && x < N && y < N;
	}
};

struct edge {
	short a, b;
	char cap, flow;
	edge(short a, short b, char cap, char flow) :a(a), b(b), cap(cap), flow(flow){}
};

vector<Point> sv, tv;
short n, s, t;
vector<short> d, ptr;
queue<short> q;
vector<edge> e;
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
	d.resize(n);
	ptr.resize(n);
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

	s = 2 * N2;
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
	queue<short> empty_queue;
	swap(q, empty_queue);
	q.push(s);
	d.assign(n, -1);
	d[s] = 0;
	while (!q.empty() && d[t] == -1) {
		int v = q.front();
		q.pop();
		for (auto id : g[v]) {
			int to = e[id].b;
			if (d[to] == -1 && e[id].flow < e[id].cap) {
				q.push(to);
				d[to] = d[v] + 1;
			}
		}
	}
	return d[t] != -1;
}

char dfs(int v) {
	if (v == t)  return 1;
	for (; ptr[v]<(int)g[v].size(); ++ptr[v]) {
		auto id = g[v][ptr[v]];
		auto to = e[id].b;
		if (d[to] != d[v] + 1)  continue;
		if ((e[id].cap - e[id].flow) && dfs(to)) {
			++e[id].flow;
			--e[id ^ 1].flow;
			return 1;
		}
	}
	return 0;
}

short dinic() {
	short flow = 0;
	for (;;) {
		if (!bfs())  break;
		ptr.assign(n, 0);
		while (dfs(s))
			++flow;
	}
	return flow;
}

void print_paths() {
	vector<vector<short>> paths(K);

	for (auto & id : g[s]) {
		auto trg = e[id].b;
		auto path_num = sv_rev[trg];
		auto & path = paths[path_num];
		short cv = trg;
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

void task() {
	input();
	buid_graph();
	if (dinic() != K)
		printf("-1");
	else {
		print_paths();
	}
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
