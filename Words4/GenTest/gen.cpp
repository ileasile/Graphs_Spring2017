#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <tuple>
#include <cstdlib>
#include <algorithm>

using namespace std;

int main() {
	ofstream f("../Graphs4/input.txt");
	vector<string> v;

	int V, H;

	V = 1000;
	H = 1000;

	f << V << " " << H << " \n";

	v.resize(V);
	for (int i = 1; i <= V; ++i) {
		v[i - 1] = to_string(i*i) + " " + "0 999999" + "\n";
	}
	std::random_shuffle(v.begin(), v.end());
	for (auto & s : v) {
		f << s;
	}


	for (int i = 1; i <= H; ++i) {
		v[i - 1] =  "0 999999 "  + to_string(i*i)  + "\n";
	}
	std::random_shuffle(v.begin(), v.end());
	for (auto & s : v) {
		f << s;
	}

	f.close();
	return 0;
}