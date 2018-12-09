#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <numeric>

#include "settings.h"


std::vector<int>
loadInputs() {
	constexpr char const* pInputFile = INPUTS_DIR "01.txt";
	std::ifstream ifs{pInputFile};
	if (!ifs.good()) {
		std::cerr << "Error loading input file" << std::endl;
		throw std::runtime_error(pInputFile);
	}
	std::vector<int> inputs;
	int freq;
	ifs >> freq;
	while (ifs.good()) {
		inputs.push_back(freq);
		ifs >> freq;
	}
	return inputs;
}


int
partOne(std::vector<int> const& inputs) {
	return std::accumulate(inputs.begin(), inputs.end(), 0);
}


int
partTwo(std::vector<int> const& inputs) {
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


int
main() {
	auto const inputs{loadInputs()};
	auto const one = partOne(inputs);
	auto const two = partTwo(inputs);
	std::cout << "Part one: " << one << std::endl;
	std::cout << "Part two: " << two << std::endl;
	return 0;
}
