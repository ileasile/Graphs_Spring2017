#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <iostream>
#include <vector>
#include <ctime>
#include <set>
#include <map>

using namespace std;
vector<vector<int>> g;
bool used[2000];
int f[2000];
int tin[2000];
int timer = 0;
int N;
vector<pair<int, int>> br;

//удаление ребра
int skip_from;
int skip_to;
void erase_edge(int from, int to) {
	skip_from = from;
	skip_to = to;
}
bool is_erased(int from, int to) {
	return 
		(from == skip_from) && (to == skip_to) ||
		(from == skip_to) && (to == skip_from);
}

void dfs_bridge(int p, int v) {
	f[v] = tin[v] = ++timer;
	used[v] = true;
	for (auto u : g[v]) {
		if (is_erased(u, v))
			continue;
		if (u == p)
			continue;
		else if (!used[u]) {
			dfs_bridge(v, u);
			f[v] = min(f[v], f[u]);
			if (f[u] > tin[v])
				br.emplace_back(u, v);
		}
		else {
			f[v] = min(f[v], tin[u]);
		}
	}
}

void search_bridges() {
	//инициализация
	fill(used, used + N, false);
	fill(f, f + N, -1);
	fill(tin, tin + N, -1);
	timer = 0;
	br.clear();
	//запуск DFS. Так, потому что граф связный
	dfs_bridge(-1, 0);
}

int main() {
	cin >> N;
	g.resize(N);

	for (int i = 0; i < N; ++i) {
		int m;
		cin >> m;
		g[i].resize(m);
		for (int j = 0; j < m; ++j) {
			int to;
			cin >> to;
			g[i][j] = to;
		}
	}

	for (int v = 0; v < N; ++v) {
		for (auto u : g[v]) {
			erase_edge(u, v);
			search_bridges();
			for (auto b: br){
				//проверка условия на непустоту множества переходов
				map<int, int> s({ {u,0}, {v,0}, {b.first,0 }, {b.second, 0} });
				++s[u];	++s[v];	++s[b.first]; ++s[b.second];
				for (auto si : s) {
					if (g[si.first].size() - si.second <= 0) {
						goto skip;
					}
				}

				cout << u << " " << v << "\n";
				cout << b.first << " " << b.second << "\n";
				return 0;
			skip:;
			}
		}
	}
	return 0;
}