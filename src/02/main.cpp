#include <iostream>
#include <cstring>
#include <climits>
#include <fstream>
#include <string>
#include <vector>

#include "settings.h"


void
loadInputs(std::vector<std::string>& inputs, char const* pFilePath) {
	std::ifstream ifs{pFilePath};
	std::string line;
	while (std::getline(ifs, line) && !line.empty()) {
		inputs.emplace_back(std::move(line));
	}
}


constexpr unsigned g_countRange = 26u;


void
countOccurrences(std::string const& line, unsigned* pCounts) {
	auto const len = line.size();
	for (unsigned i = 0u; i < len; ++i) {
		char ch = line[i];
		++pCounts[ch - 'a'];
	}
}


int
partOne(std::vector<std::string> const& inputs) {
	unsigned counts[g_countRange] = {
		0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
		0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};

	unsigned numPairs = 0u;
	unsigned numTriples = 0u;
	for (std::size_t i = 0ul, len = inputs.size(); i < len; ++i) {
		countOccurrences(inputs[i], &counts[0]);

		unsigned hasPair = 0u;
		unsigned hasTriple = 0u;
		for (unsigned j = 0u; j < g_countRange; ++j) {
			if (counts[j] == 2u) {
				hasPair = 1u;
			}
			else if (counts[j] == 3u) {
				hasTriple = 1;
			}
		}
		numPairs += hasPair;
		numTriples += hasTriple;
		std::memset(&counts[0], 0, sizeof(counts));
	}

	std::cout << "Pairs: " << numPairs << "; triples: " << numTriples
		<< std::endl;
	return numPairs * numTriples;
}


int
distance(std::string const& a, std::string const& b) {
	unsigned len = unsigned(a.size());
	int diff = 0;
	for (unsigned i = 0u; i < len; ++i) {
		if (a[i] == b[i]) {
			continue;
		}
		if (diff != 0) {
			diff = INT_MAX;
			break;
		}
		diff = 1;
	}
	return diff;
}


std::string
removeDistinct(std::string const& a, std::string const& b) {
	std::string result{a};
	for (unsigned i = 0u, len = unsigned(a.size()); i < len; ++i) {
		if (a[i] != b[i]) {
			result.erase(i, 1);
			return result;
		}
	}
	return result;
}


void
partTwo(std::vector<std::string> const& inputs) {
	unsigned const len = unsigned(inputs.size());
	int dist = 0;
	for (unsigned i = 0u; i < len; ++i) {
		for (unsigned j = i + 1; j < len; ++j) {
			dist = distance(inputs[i], inputs[j]);
			if (dist == 1) {
				std::cout
					<< "IDs: " << inputs[i] << ", " << inputs[j]
					<< "; chars: " << removeDistinct(inputs[i], inputs[j])
					<< std::endl;
				return;
			}
		}
	}

	std::cerr << "Failed to find IDs" << std::endl;
}


int
main(int argc, char const* argv[]) {
	std::vector<std::string> inputs;
	loadInputs(inputs, INPUTS_DIR "02.txt");
	std::cout << "Part one: " << partOne(inputs) << std::endl;
	partTwo(inputs);
	return 0;
}
