#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <unordered_map>
#include <bitset>
#include <ctime>
#include <tuple>
#include <cstdlib>
#include <algorithm>
#include <cmath>
using namespace std;

const double eps = 10e-4;

template<class T>
struct P {
	T x, y;
	inline const double sqr_len()const {
		return (double)x*x + (double)y*y;
	}

	template<class _T>
	inline bool operator==(const P<_T> & other) const {
		return abs(x - other.x) < eps && abs(y - other.y) < eps;
	}
	template<class _T>
	inline bool operator!=(const P<_T> & other) const {
		return abs(x - other.x) >= eps || abs(y - other.y) >= eps;
	}
	template<class _T>
	inline bool operator<(const P<_T> & other) const {
		return *this != other && (x < other.x || (x == other.x && y < other.y));
	}
	inline P<T> operator-(const P<T> & other) const {
		return P<T>{x - other.x, y - other.y};
	}
};

typedef P<int> Pt;
typedef P<double> Ptd;

struct Seg {
	Pt pt[2];
	inline Pt & operator[](int i) { return pt[i]; }
	inline const Pt & operator[](int i) const { return pt[i]; }
	inline Pt get_vector()const {
		return Pt{ pt[1].x - pt[0].x, pt[1].y - pt[0].y };
	}
};

struct Vertex {
	double weight;
	int num, prev;
	inline bool operator < (const Vertex & other) const {
		return weight > other.weight;
	}
};

inline void get_eq(const Seg & s, int & a, int & b, int & c) {
	a = s[1].y - s[0].y;
	b = s[0].x - s[1].x;
	c = s[1].x * s[0].y - s[0].x * s[1].y;
}

template <class T>
inline bool seg_has_pt_proj(const Seg & s, P<T> & p) {
	return	((s[0].x - eps <= p.x) && (p.x <= s[1].x + eps)) &&
		((s[0].y - eps <= p.y) && (p.y <= s[1].y + eps) ||
		(s[1].y - eps <= p.y) && (p.y <= s[0].y + eps));
}

inline bool seg_intersect(const Seg & s0, const Seg & s1, Ptd & intersection) {
	int a[2], b[2], c[2];
	get_eq(s0, a[0], b[0], c[0]);
	get_eq(s1, a[1], b[1], c[1]);
	int d = a[0] * b[1] - a[1] * b[0];
	if (d == 0)
		return false;
	int d1 = b[0] * c[1] - b[1] * c[0];
	int d2 = a[1] * c[0] - a[0] * c[1];

	double dd = d;

	intersection.x = d1 / dd;
	intersection.y = d2 / dd;
	return seg_has_pt_proj(s0, intersection) && seg_has_pt_proj(s1, intersection);
}

inline bool lies(const Seg & s, Pt & p) {
	return seg_has_pt_proj(s, p) &&
		((p.x - s[0].x)*(s[1].y - s[0].y) == (p.y - s[0].y)*(s[1].x - s[0].x));
}

inline double angle(const Ptd & v0, const Ptd & v1) {
	auto ac = (v0.x*v1.x + v0.y*v1.y) / sqrt(v0.sqr_len() * v1.sqr_len());
	ac = (ac > 1) ? 1 : (ac < -1 ? -1 : ac);
	return acos(ac);
}

int N;
const double INF = numeric_limits<double>::max();
vector<Seg> v;
vector<Ptd> vert;
vector<vector<int>> g;
Pt sti[2];
int stnum[2];
int vert_cnt;

void input() {
	scanf("%d", &N);
	v.resize(N);
	for (auto & e : v) {
		scanf("%d %d %d %d", &(e[0].x), &(e[0].y), &(e[1].x), &(e[1].y));
		if (e.pt[1] < e.pt[0])
			swap(e.pt[1], e.pt[0]);
	}
	for (int i = 0; i < 2; ++i)
		scanf("%d %d", &(sti[i].x), &(sti[i].y));
}

void build_graph() {
	map<Ptd, pair<int, vector<int>>> mp;

	vert_cnt = 0;
	for (int i = 0; i < N; ++i) {
		for (int j = i + 1; j < N; ++j) {
			Ptd isc;
			if (seg_intersect(v[i], v[j], isc)) {
				auto it = mp.find(isc);
				if (it != mp.end()) {
					if (it->second.second.front() == i)
						it->second.second.push_back(j);
				}
				else {
					mp[isc] = { vert_cnt++,{ i, j } };
				}
			}
		}
	}

	for (int cntr = 0; cntr < 2; ++cntr) {
		auto & pi = sti[cntr];
		Ptd p;
		p.x = pi.x; p.y = pi.y;
		auto it = mp.find(p);
		if (it == mp.end()) {
			stnum[cntr] = vert_cnt;
			for (int i = 0; i < N; ++i) {
				if (lies(v[i], pi)) {
					mp[p] = make_pair(vert_cnt++, vector<int>({ i }));
					break;
				}
			}

		}
		else {
			stnum[cntr] = it->second.first;
		}
	}

	vector<int> pr(N, -1);
	g.resize(vert_cnt);
	vert.resize(vert_cnt);
	for (auto & u : mp) {
		auto & num = u.second.first;
		vert[num] = u.first;
		auto & vec = u.second.second;
		for (auto & r : vec) {
			if (pr[r] != -1) {
				g[pr[r]].push_back(num);
				g[num].push_back(pr[r]);
			}
			pr[r] = num;
		}
	}
}

void dijkstra() {
	//Dijkstra initialization
	vector<map<int, double>> d(vert_cnt);
	vector<set<int>> used(vert_cnt, set<int>());
	priority_queue<Vertex> q;

	// Dijkstra (modified)
	d[stnum[0]][0] = 0;
	for (auto e : g[stnum[0]]) {
		d[e][stnum[0]] = 0;
		q.push({ 0, e, stnum[0] });
	}
	while (!q.empty()) {
		auto v = q.top();
		q.pop();
		if (!used[v.num].insert(v.prev).second)
			continue;
		Ptd first_vec = vert[v.num] - vert[v.prev];
		auto oldw = d[v.num][v.prev];
		for (auto e : g[v.num]) {
			if (used[e].find(v.num) != used[e].end())
				continue;
			Ptd second_vec = vert[e] - vert[v.num];

			auto it = d[e].insert(make_pair(v.num, INF)).first;
			auto w = angle(first_vec, second_vec) + oldw;
			if (it->second > w) {
				it->second = w;
				q.push({ w, e, v.num });
			}
		}
	}
	double mn = INF;
	for (auto el : d[stnum[1]]) {
		if (mn > el.second)
			mn = el.second;
	}

	if (mn == INF)
		printf("-1");
	else
		printf("%f\n", mn * 180 / M_PI);
}

void task() {
	input();
	build_graph();
	dijkstra();
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
