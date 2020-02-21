#include <iostream>
#include "roman.h"

using namespace std;

int main(void)
{
	for (uint16_t i = 1; i < 2000; i++) {
		cout << i << " = " << Fs2a::toRoman(i) << endl;
	}

	return 0;
}
