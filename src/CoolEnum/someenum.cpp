#include <iostream>
#include "someenum.h"

#include "CoolEnum.cpp"
#include "someenum.def"

using namespace std;

int main(void)
{
	someenum se;

	cout << "Key for DELTA is " << someenum_(DELTA) << endl;

	return 0;
}
