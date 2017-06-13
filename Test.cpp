#include "Unary_Polynomial.h"

using namespace std;

int main() {
	Unary_Polynomial a, b;
	string express, express1;

	while (cin >> express >> express1) {
		a.Input(express);
		b.Input(express1);

		auto c = a + b;
		auto d = a - b;

		cout << endl << "a + b:" << endl;
		c.Print();
		cout << "a - b:" << endl;
		d.Print();

		a.Clear();
		b.Clear();
		c.Clear();
		d.Clear();
	}

	return 0;
}
