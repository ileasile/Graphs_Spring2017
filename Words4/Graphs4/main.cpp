#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <tuple>
#include <cstdlib>
#include <algorithm>
using namespace std;

//horisontal/vertical word structure
struct W {
	union {	int x, y; };
	union {	int x1, y1; };
	union {	int x2, y2;	};
	inline bool operator<(const W & other) const{
		// or y < other.y for vertical word
		return x < other.x;
	}
};

const int MAX_SIDE = 1000;
const int MAX_WLEN = 1000001;

int H, V, answer, vertex_count;
W h[MAX_SIDE];
W v[MAX_SIDE];
int parent [MAX_SIDE * MAX_SIDE];
vector <pair<int, int>> edges [MAX_WLEN + 1];

struct DSU {
	DSU(int size) {
		for (int i = 0; i < size; ++i) {
			parent[i] = i;
		}
	}

	inline int find(int v) {
		if (v == parent[v])
			return v;
		return parent[v] = find(parent[v]);
	}

	inline void unions(int a, int b) {
		parent[b] = a;
	}
};

void input_and_count_letters() {
	answer = 0;
	scanf("%d %d", &H, &V);
	for (int i = 0; i < H; ++i) {
		scanf("%d %d %d", &(h[i].x), &(h[i].y1), &(h[i].y2));
		answer += h[i].y2 - h[i].y1;
	}
	for (int i = 0; i < V; ++i) {
		scanf("%d %d %d", &(v[i].x1), &(v[i].x2), &(v[i].y));
		answer += v[i].x2 - v[i].x1;
	}
	answer += V + H;
}

void build_edges_list() {
	pair<int, int> vlast[MAX_SIDE];
	fill(vlast , vlast + V, make_pair(-1, -1));
	
	vertex_count = 0;
	for (auto & hi : h) {
		int j = 0;
		for (; j < V && v[j].y < hi.y1; ++j);

		int was_int = -1;
		for (; j < V && v[j].y <= hi.y2; ++j) {
			//if concentration point
			if (v[j].x1 <= hi.x && hi.x <= v[j].x2) {
				if (was_int != -1)
					edges[v[j].y - was_int - 1].push_back({vertex_count - 1, vertex_count });
				if (vlast[j].first != -1)
					edges[hi.x - vlast[j].second - 1].push_back({  vlast[j].first, vertex_count });
				vlast[j] = make_pair(vertex_count, hi.x);
				was_int = v[j].y;
				++vertex_count;
			}
		}
	}
	answer -= vertex_count;
}

void Kruskal_and_print_answer() {
	DSU su(vertex_count);
	int a, b;
	for (auto & ed : edges[0]) {
		a = su.find(ed.first);
		b = su.find(ed.second);

		if (a == b) {
			printf("-1");
			return;
		}
		else {
			su.unions(a, b);
		}
	}

	for (int w = 1; w < MAX_WLEN + 1; ++w) {
		for (auto & ed : edges[w]) {
			a = su.find(ed.first);
			b = su.find(ed.second);

			if (a == b) {
				answer -= w;
			}
			else {
				su.unions(a, b);
			}
		}
	}

	printf("%d", answer);
}

void task() {
	input_and_count_letters();

	sort(h, h + H);
	sort(v, v + V);
	build_edges_list();

	Kruskal_and_print_answer();
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
