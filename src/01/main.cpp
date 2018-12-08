#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

#include "settings.h"


constexpr char const* const pInputFile = INPUTS_DIR "01.txt";


void loadInputs(std::vector<int>& inputs) {
	std::ifstream ifs{pInputFile};
	if (!ifs.good()) {
		std::cerr << "Error loading input file: " << pInputFile << std::endl;
		throw std::runtime_error(pInputFile);
	}
	int freq;
	ifs >> freq;
	while (ifs.good()) {
		inputs.push_back(freq);
		ifs >> freq;
	}
}


int partOne() {
	std::vector<int> inputs;
	loadInputs(inputs);
	int sum = 0;
	for (unsigned i = 0u, len = unsigned(inputs.size()); i < len; ++i) {
		sum += inputs[i];
	}
	return sum;
}


int partTwo() {
	std::vector<int> inputs;
	loadInputs(inputs);

	int sum = 0;
	std::set<int> frequencies;
	frequencies.insert(sum);
	unsigned len = unsigned(inputs.size());
	bool repeat = true;
	do {
		for (unsigned i = 0u; i < len; ++i) {
			sum += inputs[i];
			auto const result = frequencies.insert(sum);
			if (!result.second) { // Insertion failed; sum already present.
				repeat = false;
				break;
			}
		}
	} while (repeat);
	return sum;
}


int main(int argc, char const* argv[]) {
	std::cout << "Reading input from " << pInputFile << std::endl;

	int part = 0;
	if (argc == 2) {
		part = std::atoi(argv[1]);
	}

	int result = 0;
	switch (part) {
		case 1:
			result = partOne();
			break;
		case 2:
			result = partTwo();
			break;
		default:
			std::cout << "Usage: ./main <part>" << std::endl;
			return 1;
	}

	std::cout << "Answer to part " << part << ": " << result << std::endl;
	return 0;
}
