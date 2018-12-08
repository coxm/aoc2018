#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>

#include "settings.h"


template <typename IS>
std::pair<unsigned, unsigned>
parseNode(IS& is) {
	unsigned numChildren = 0u;
	unsigned numMetadata = 0u;
	is >> numChildren >> numMetadata;

	unsigned totalMetadataSum = 0u;
	std::vector<unsigned> childValues;
	childValues.resize(numChildren);
	for (unsigned i = 0u; i < numChildren; ++i) {
		auto const result = parseNode(is);
		totalMetadataSum += result.first;
		childValues[i] = result.second;
	}

	if (numChildren == 0u) {
		for (unsigned i = 0u; i < numMetadata; ++i) {
			unsigned datum;
			is >> datum;
			totalMetadataSum += datum;
		}
		return {totalMetadataSum, totalMetadataSum};
	}

	unsigned value = 0u;
	for (unsigned i = 0u; i < numMetadata; ++i) {
		unsigned datum = numChildren;
		is >> datum;
		totalMetadataSum += datum;
		if (--datum < numChildren) {
			value += childValues[datum];
		}
	}

	return {totalMetadataSum, value};
}


int
main() {
	std::ifstream ifs{INPUTS_DIR "08.txt"};
	auto const parts = parseNode(ifs);
	std::cout << "Part one: " << parts.first << std::endl
	          << "Part two: " << parts.second << std::endl;
	return 0;
}
