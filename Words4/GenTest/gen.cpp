#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <tuple>
#include <cstdlib>
#include <algorithm>

using namespace std;

int main() {
	ofstream f("input.txt");
	int V, H;

	V = 1000;
	H = 1000;

	f << V << " " << H << " \n";
	for (int i = 1; i <= V; ++i) {
		f << i*i << " 0 999999 \n";
	}
	for (int i = 1; i <= H; ++i) {
		f << "0 999999 " << i*i << " \n";
	}

	return 0;
}