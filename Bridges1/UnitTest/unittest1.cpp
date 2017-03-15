#include "stdafx.h"
#include "CppUnitTest.h"
#include <ctime>
#include <iostream>
#include <fstream>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

int main();

namespace UnitTest
{	
	void testit(int a, int b, int c, int d) {
		int i = 0;
		int w[4];
		std::ifstream f("output.txt");
		while (f && i < 4) {
			f >> w[i];
			++i;
		}
		if (!f)
			Assert::Fail(L"Presentation Error");
		f.close();

		if (w[0] > w[1]) std::swap(w[0], w[1]);
		if (w[2] > w[3]) std::swap(w[2], w[3]);
		if (a > b) std::swap(a, b);
		if (c > d) std::swap(c, d);
		
		if (a == w[0] && b == w[1] && c == w[2] && d == w[3])	return;
		if (c == w[0] && d == w[1] && a == w[2] && b == w[3])	return;

		Assert::Fail(L"WA");
	}

	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			freopen("input1.txt", "r", stdin);
			freopen("output.txt", "w", stdout);
			clock_t start = clock();
			main();
			clock_t finish = clock();
			testit(88, 56, 87, 78);
			//std::cout << "\n\n=============\n" << (finish - start) / (double)CLOCKS_PER_SEC;
			fclose(stdin);
			fclose(stdout);
		}
		
	};
}