#include <iostream>
#include <map>
#include "roman.h"

namespace Fs2a {

	std::string toRoman(const uint16_t num)
	{
		uint16_t rem = num; // Remainder of num to convert
		std::string out;  // Output variable
		std::map<uint16_t, char> rn; // Roman numerals


		rn[1000] = 'M';
		rn[500] = 'D';
		rn[100] = 'C';
		rn[50] = 'L';
		rn[10] = 'X';
		rn[5] = 'V';
		rn[1] = 'I';

		/* An important discovery I made:
		 * The 5* number (V, L and D) are never used as a prefix to substract
		 * it from the following character. So only the 1* numbers (I, X and
		 * C) are * substracted from the following character. Also, at most
		 * one substraction is done. */

		// Walk the map in reverse order
		for (auto i = rn.rbegin(); i != rn.rend(); i++) {
//			std::cout << "Checking roman numerals map at " << i->first << " = " << i->second << std::endl;

			while (rem >= i->first) {
//				std::cout << "Filling up with " << i->first << " = " << i->second << std::endl;
				out += i->second; rem -= i->first;
			}
			for (auto j = rn.begin(); j->first != i->first; j++) {
//				std::cout << "Checking substractions with " << j->first << " = " << j->second << std::endl;
				if (j->first != 100 && j->first != 10 && j->first != 1) continue;
				if (rem >= i->first - j->first) {
//					std::cout << "Found substraction for remaining " << rem << " with " << i->first << " - " << j->first << std::endl;
					out += j->second;
					out += i->second;
					rem -= (i->first - j->first);
					break;
				}
			}
		}

		return out;
	}

} // Fs2a namespace
