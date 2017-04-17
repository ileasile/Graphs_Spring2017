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
public:
	DSU(int size) {
		parent.resize(size);
		for (int i = 0; i < size; ++i) {
			parent[i] = i;
		}
	}

	~DSU() {
		parent.clear();
	}

	int find(int v) {
		if (v == parent[v])
			return v;
		return parent[v] = find(parent[v]);
	}

	void unions(int a, int b) {
		parent[b] = a;
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
	vector<tuple<int, int, bool>> vlast(V, make_tuple( -1, -1,  false ));
	edges.reserve(2000000);
	vertex_count = 0;
	for (auto & hi : h) {
		int j = 0;
		for (; j < V && v[j].y < hi.y1; ++j);

		int was_int = -1;
		for (; j < V && v[j].y <= hi.y2; ++j) {
			//if concentration point
			if (v[j].x1 <= hi.x && hi.x <= v[j].x2) {
				int w1 = -1, w2 = -1;
				if (was_int != -1) {
					w1 = v[j].y - was_int - 1;
					edges.emplace_back(w1, vertex_count - 1, vertex_count);
				}
				if (get<0>(vlast[j]) != -1) {
					w2 = hi.x - get<1>(vlast[j]) - 1;
					edges.emplace_back(w2, get<0>(vlast[j]), vertex_count);
				}
				if (w1 == 0 && w2 == 0 && get<2>(vlast[j])) {
					printf("-1");
					throw 1;
				}
				vlast[j] = make_tuple(vertex_count, hi.x, w2 == 0);
				was_int = v[j].y;
				++vertex_count;
			}
		}
	}
	answer -= vertex_count;
}

void Kruskal_and_print_answer() {
	DSU su(vertex_count);
	auto ed = edges.begin();
	for (; ed != edges.end() && get<0>(*ed) == 0; ++ed) {
		get<1>(*ed) = su.find(get<1>(*ed));
		get<2>(*ed) = su.find(get<2>(*ed));

		if (get<1>(*ed) == get<2>(*ed)) {
			printf("-1");
			return;
		}
		else {
			su.unions(get<1>(*ed), get<2>(*ed));
		}
	}

	for (; ed != edges.end(); ++ed) {
		get<1>(*ed) = su.find(get<1>(*ed));
		get<2>(*ed) = su.find(get<2>(*ed));

		if (get<1>(*ed) == get<2>(*ed)) {
			answer -= get<0>(*ed);
		}
		else {
			su.unions(get<1>(*ed), get<2>(*ed));
		}
	}

	printf("%d", answer);
}

void task() {
	input_and_count_letters();

	sort(h.begin(), h.end());
	sort(v.begin(), v.end());
	try {
		build_edges_list();
	}
	catch (int){
		return;
	}
	h.clear(); v.clear();

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
