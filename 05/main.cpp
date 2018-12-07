#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cstring>

#include "settings.h"


bool
willReact(char a, char b) {
	return std::abs(a - b) == 32; // 'a' - 'A'.
}


int
partOne(std::string& input) {
	// Ignore sentinels.
	int lhs = 1;
	int const len = int(input.length() - 1);
	for (int rhs = 1; rhs < len;) {
		char const currentRHS = input[rhs];

		if (willReact(currentRHS, input[rhs + 1])) {
			for (
				rhs += 2;
				willReact(input[lhs - 1], input[rhs]);
				++rhs, --lhs
			);
		}
		else {
			input[lhs] = currentRHS;
			++lhs;
			++rhs;
		}
	}
	return lhs - 1;
}


int
partTwo(std::string const& input) {
	auto least = int(input.size());
	for (char c = 'a', C = 'A'; c <= 'z'; ++c, ++C) {
		std::string filtered;
		std::copy_if(
			std::begin(input),
			std::end(input),
			std::back_inserter(filtered),
			[c, C] (auto const elem) {
				return elem != c && elem != C;
			});

		int const thisSize = partOne(filtered);
		least = std::min(least, thisSize);
	}
	return least;
}


std::string
getInput() {
	char const* const pInputStr = std::getenv("INPUT");
	if (pInputStr) {
		return std::string(pInputStr);
	}
	std::string input;
	std::ifstream ifs(INPUTS_DIR "05.txt");
	std::getline(ifs, input);
	return input;
}


int
main(int argc, char const* const argv[]) {
	int part = 0;
	{
		char const* const pPart = std::getenv("PART");
		if (pPart) {
			part = std::atoi(pPart);
		}
		if (part != 1 && part != 2) {
			std::cout << "Usage: PART=<part> " << argv[0] << std::endl;
			return 1;
		}
	}

	auto input{getInput()};
	input = "/" + input + "/"; // Sentinels.
	auto const result = (part == 1) ? partOne(input) : partTwo(input);
	std::cout << "Chars: " << result << std::endl;
	return 0;
}
