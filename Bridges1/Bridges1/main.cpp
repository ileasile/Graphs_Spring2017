#ifdef ILEASILE_D
#include "C:\Google Disc\Graphs\Bridges1\UnitTest\stdafx.h"
#endif // ILEASILE_D

#ifndef ILEASILE_D
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <vector>
#include <ctime>
#include <set>
#include <map>
#include <stack>
#endif

using namespace std;
vector<int> g [151];
int f[151];
int tin[151];
int ind[151];
int timer = 0;
int N;
vector<pair<int, int>> br;

template<class T>
struct STACK {
	T data[200];
	T *ptr;
	STACK() :ptr(data) {}
	inline T top() {
		return *(ptr - 1);
	}
	inline void pop() {
		--ptr;
	}
	inline bool empty() {
		return ptr == data;
	}
	inline void push(T v) {
		*ptr = v;
		++ptr;
	}
};

int skip_from;
int skip_to;

void dfs_bridge(int _p, int _v) {
	STACK<pair<int,int>> ST;
	f[_v] = tin[_v] = ++timer;
	ST.push(make_pair(_v,-2));

	while (!ST.empty()){
		int u, v, p;
		tie(v, p) = ST.top();
		if (ind[v] != -1) {
			u = g[v][ind[v]];
			f[v] = min(f[v], f[u]);
			if (f[u] > tin[v]) {
				br.emplace_back(u, v);
			}
		}
		while (1) {
			++ind[v];
			if (ind[v] != g[v].size()) {
				u = g[v][ind[v]];
				while (
					(u == p || (u == skip_from) && (v == skip_to) ||
					(v == skip_to) && (u == skip_from)))
				{
					++ind[v];
					if (ind[v] == g[v].size())
						break;
					u = g[v][ind[v]];
				}
			}
			if (ind[v] == g[v].size()) {	
				ST.pop();
				break;
			}
			else if (tin[u] == -1) {
				f[u] = tin[u] = ++timer;
				ST.push(make_pair(u,v));
				break;
			}
			else {
				f[v] = min(f[v], tin[u]);
			}
		}
		
	}
}

void search_bridges() {
	for (int i = 0; i < N; ++i) {
		f[i] = tin[i] = ind[i] = -1;
	}
	timer = 0;
	br.clear();
	dfs_bridge(-1, 0);
}

int main() {
#ifdef ILEASILE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	clock_t start = clock();
#endif
	cin >> N;
	for (int i = 0; i < N; ++i) {
		int m;
		cin >> m;
		g[i].resize(m);
		for (int j = 0; j < m; ++j) {
			cin >> g[i][j];
		}
	}

	for (int v = 0; v < N; ++v) {
		for (auto u : g[v]) {
			skip_from = v;
			skip_to = u;
			search_bridges();
			for (auto b: br){				
				if (g[u].size() <= 1 || g[v].size() <= 1 ||
					g[b.first].size() <= 1 || g[b.second].size() <= 1 ||
					(u == b.first || u == b.second) && g[u].size() <= 2 ||
					(v == b.first || v == b.second) && g[v].size() <= 2);
				else {
					cout << u << " " << v << "\n";
					cout << b.first << " " << b.second << "\n";
#ifdef ILEASILE
					clock_t finish = clock();
					cout << "\n\n=============\n" << (finish - start) / (double)CLOCKS_PER_SEC;
					fclose(stdin);
					fclose(stdout);
#endif
					return 0;
				}
			}
		}
	}


	return 0;
}