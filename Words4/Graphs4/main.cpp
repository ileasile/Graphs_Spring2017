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
	bool operator<(const W & other) const{
		// or y < other.y for vertical word
		return x < other.x;
	}
};

class DSU {
	vector<int> parent;
	vector<int> rank;
public:
	DSU(int size) {
		parent.resize(size);
		rank.resize(size, 0);
		for (int i = 0; i < size; ++i) {
			parent[i] = i;
		}
	}

	~DSU() {
		parent.clear();
		rank.clear();
	}

	int find(int v) {
		if (v == parent[v])
			return v;
		return parent[v] = find(parent[v]);
	}

	void unions(int a, int b) {
		if (rank[a] < rank[b])
			swap(a, b);
		parent[b] = a;
		if (rank[a] == rank[b])
			++rank[a];
	}
};

int H, V, answer, vertex_count;
vector<W> h, v;
vector<tuple<int, int, int>> edges;

void input_and_count_letters() {
	answer = 0;
	scanf("%d %d", &H, &V);
	h.resize(H);
	v.resize(V);
	for (auto & hi : h) {
		scanf("%d %d %d", &(hi.x), &(hi.y1), &(hi.y2));
		answer += hi.y2 - hi.y1 + 1;
	}
	for (auto & vi : v) {
		scanf("%d %d %d", &(vi.x1), &(vi.x2), &(vi.y));
		answer += vi.x2 - vi.x1 + 1;
	}
}

void build_edges_list() {
	vector<pair<int, int>> vlast(V, { -1, -1 });
	vertex_count = 0;
	for (int i = 0; i < H; ++i) {
		int was_int = -1;
		for (int j = 0; j < V; ++j) {
			//if concentration point
			if (h[i].y1 <= v[j].y && v[j].y <= h[i].y2 &&
				v[j].x1 <= h[i].x && h[i].x <= v[j].x2) {

				if (was_int != -1)
					edges.emplace_back(v[j].y - was_int - 1, vertex_count - 1, vertex_count);
				if (vlast[j].first != -1)
					edges.emplace_back(h[i].x - vlast[j].second - 1, vlast[j].first, vertex_count);
				vlast[j] = make_pair(vertex_count, h[i].x);
				was_int = v[j].y;
				++vertex_count;
			}
		}
	}
	answer -= vertex_count;
}

void Kruskal_and_print_answer() {
	DSU su(vertex_count);
	for (auto & ed : edges) {
		int w, i, j;
		tie(w, i, j) = ed;
		i = su.find(i);
		j = su.find(j);

		if (i == j) {
			if (w == 0) {
				printf("-1");
				return;
			}
			answer -= w;
		}
		else {
			su.unions(i, j);
		}
	}

	printf("%d", answer);
}

void task() {
	input_and_count_letters();
	sort(h.begin(), h.end());
	sort(v.begin(), v.end());
	build_edges_list();
	sort(edges.begin(), edges.end());
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
