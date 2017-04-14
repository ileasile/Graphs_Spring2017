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
	union {	int y1, x1; };
	union {	int y2, x2;	};
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
		a = find(a);
		b = find(b);
		if (a != b) {
			if (rank[a] < rank[b])
				swap(a, b);
			parent[b] = a;
			if (rank[a] == rank[b])
				++rank[a];
		}
	}
};

void task() {
	long long answer = 0;

	//input
	int H, V;
	scanf("%d %d", &H, &V);
	vector<W> h(H), v(V);
	for (int i = 0; i < H; ++i) {
		scanf("%d %d %d", &(h[i].x), &(h[i].y1), &(h[i].y2));
		answer += h[i].y2 - h[i].y1 + 1;
	}
	for (int i = 0; i < V; ++i) {
		scanf("%d %d %d", &(v[i].x1), &(v[i].x2), &(v[i].y));
		answer += v[i].x2 - v[i].x1 + 1;
	}
	// answer == letters number (counting intersections)

	//sorting words
	sort(h.begin(), h.end());
	sort(v.begin(), v.end());

	//building edges list
	vector<tuple<int, int, int>> edges;
	vector<pair<int, int>> vlast(V, { -1, -1 });
	int vertex_count = 0;
	for (int i = 0; i < H; ++i) {
		int was_int = -1;
		for (int j = 0; j < V; ++j) {
			//if concentration point
			if ( h[i].y1 <= v[j].y && v[j].y <= h[i].y2 &&
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

	//substract intersections
	answer -= vertex_count;

	//edges list sorting
	sort(edges.begin(), edges.end());

	//Kraskal algorithm
	DSU su(vertex_count);
	for (auto & ed : edges) {
		int w, i, j;
		tie(w, i, j) = ed;

		if (su.find(i) == su.find(j)) {
			if (w == 0) {
				printf("%d", -1);
				return;
			}
			answer -= w;
		}
		else {
			su.unions(i, j);
		}
	}

	printf("%lld", answer);
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
