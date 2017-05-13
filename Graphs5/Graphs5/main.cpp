#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <ctime>
#include <tuple>
#include <cstdlib>
#include <algorithm>
#include <cmath>
using namespace std;

const double eps = 10e-4;

template<class T>
struct Point {
	T x, y;
	inline const double sqr_len()const {
		return (double)x*x + (double)y*y;
	}

	template<class _T>
	inline bool operator==(const Point<_T> & other) const {
		return abs(x - other.x) < eps && abs(y - other.y) < eps;
	}
	template<class _T>
	inline bool operator!=(const Point<_T> & other) const {
		return abs(x - other.x) >= eps || abs(y - other.y) >= eps;
	}
	template<class _T>
	inline bool operator<(const Point<_T> & other) const {
		return *this != other && (x < other.x || (x == other.x && y < other.y));
	}
	inline Point<T> operator-(const Point<T> & other) const {
		return Point<T>{x - other.x, y - other.y};
	}
};

typedef Point<int> IntegerPoint;
typedef Point<double> DoublePoint;

struct Line {
	int a, b, c;
};

struct Segment {
	IntegerPoint pt[2];
	inline IntegerPoint & operator[](int i) { return pt[i]; }
	inline const IntegerPoint & operator[](int i) const { return pt[i]; }
	inline IntegerPoint get_vector()const {
		return IntegerPoint{ pt[1].x - pt[0].x, pt[1].y - pt[0].y };
	}

	template <class T>
	inline bool in_bounding_box(const Point<T> & p) const{
		return	(((*this)[0].x - eps <= p.x) && (p.x <= (*this)[1].x + eps)) &&
			(((*this)[0].y - eps <= p.y) && (p.y <= (*this)[1].y + eps) ||
			((*this)[1].y - eps <= p.y) && (p.y <= (*this)[0].y + eps));
	}

	inline void get_eq(Line & l) const{
		l.a = (*this)[1].y - (*this)[0].y;
		l.b = (*this)[0].x - (*this)[1].x;
		l.c = (*this)[1].x * (*this)[0].y - (*this)[0].x * (*this)[1].y;
	}

	inline bool lies(IntegerPoint & p) const{
		return this->in_bounding_box(p) &&
			((p.x - (*this)[0].x)*((*this)[1].y - (*this)[0].y) == (p.y - (*this)[0].y)*((*this)[1].x - (*this)[0].x));
	}
};

struct Vertex {
	double weight;
	int num, prev;
	inline bool operator < (const Vertex & other) const {
		return weight > other.weight;
	}
};


inline bool seg_intersect(const Segment & s0, const Segment & s1, DoublePoint & intersection) {
	Line l[2];
	s0.get_eq(l[0]);
	s1.get_eq(l[1]);
	int d = l[0].a * l[1].b - l[1].a * l[0].b;
	if (d == 0)
		return false;
	int d1 = l[0].b * l[1].c - l[1].b * l[0].c;
	int d2 = l[1].a * l[0].c - l[0].a * l[1].c;

	double dd = d;

	intersection.x = d1 / dd;
	intersection.y = d2 / dd;
	return s0.in_bounding_box(intersection) && s1.in_bounding_box(intersection);
}

inline double get_angle(const DoublePoint & v0, const DoublePoint & v1) {
	auto ac = (v0.x*v1.x + v0.y*v1.y) / sqrt(v0.sqr_len() * v1.sqr_len());
	ac = (ac > 1) ? 1 : (ac < -1 ? -1 : ac);
	return acos(ac);
}

int N;
const double INF = numeric_limits<double>::max();
vector<Segment> v;
vector<DoublePoint> vert;
vector<vector<int>> g;
IntegerPoint sti[2];
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
	map<DoublePoint, pair<int, vector<int>>> mp;

	vert_cnt = 0;
	for (int i = 0; i < N; ++i) {
		for (int j = i + 1; j < N; ++j) {
			DoublePoint intersection_point;
			if (seg_intersect(v[i], v[j], intersection_point)) {
				auto it = mp.find(intersection_point);
				if (it != mp.end()) {
					if (it->second.second.front() == i)
						it->second.second.push_back(j);
				}
				else {
					mp[intersection_point] = { vert_cnt++,{ i, j } };
				}
			}
		}
	}

	for (int cntr = 0; cntr < 2; ++cntr) {
		auto & pi = sti[cntr];
		DoublePoint p;
		p.x = pi.x; p.y = pi.y;
		auto it = mp.find(p);
		if (it == mp.end()) {
			stnum[cntr] = vert_cnt;
			for (int i = 0; i < N; ++i) {
				if (v[i].lies(pi)) {
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
		DoublePoint first_vec = vert[v.num] - vert[v.prev];
		auto oldw = d[v.num][v.prev];
		for (auto e : g[v.num]) {
			if (used[e].find(v.num) != used[e].end())
				continue;
			DoublePoint second_vec = vert[e] - vert[v.num];

			auto it = d[e].insert(make_pair(v.num, INF)).first;
			auto w = get_angle(first_vec, second_vec) + oldw;
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
