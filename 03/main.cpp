#include <cstdio>
#include <memory>
#include <vector>
#include <unordered_map>
#include <utility>
#include <stdexcept>
#include <cstdint>

#include "settings.h"


struct Claim {
	inline Claim(
		unsigned id_,
		unsigned xmin_,
		unsigned xmax_,
		unsigned ymin_,
		unsigned ymax_
	)
		:	xmin(xmin_)
		,	xmax(xmax_)
		,	ymin(ymin_)
		,	ymax(ymax_)
		,	id(id_)
	{
	}

	bool
	overlaps(Claim const& c) const noexcept {
		bool const xOverlap = (xmin <= c.xmax && c.xmin <= xmax);
		bool const yOverlap = (ymin <= c.ymax && c.ymin <= ymax);
		return xOverlap && yOverlap;
	}

	inline bool
	operator<(Claim const& claim) const noexcept {
		return id < claim.id;
	}

	unsigned xmin;
	unsigned xmax;
	unsigned ymin;
	unsigned ymax;
	unsigned id;
	bool hasOverlap = false;
};


struct Deleter {
	inline void operator()(FILE* p) const noexcept { fclose(p); }
};


constexpr char const* pInputFile = INPUTS_DIR "03.txt";


std::vector<Claim>
loadInputs() {
	std::vector<Claim> claims;
	std::unique_ptr<std::FILE, Deleter> pFile(fopen(pInputFile, "r"));
	while (!std::feof(pFile.get())) {
		unsigned id, xOffset, yOffset, width, height;
		int const result = std::fscanf(
			pFile.get(), "#%d @ %d,%d: %dx%d\n",
			&id, &xOffset, &yOffset, &width, &height);
		if (result != 5) {
			throw std::runtime_error("Error parsing input");
		}
		claims.emplace_back(
			id, xOffset, xOffset + width, yOffset, yOffset + height);
	}
	return claims;
}


struct Hash {
	inline std::size_t
	operator()(std::pair<unsigned, unsigned> const& pair) const noexcept {
		std::size_t a = pair.first;
		std::size_t b = pair.second;
		return (a << 32) | b;
	}
};


void
partOne(std::vector<Claim> const& claims) {
	using key_type = std::pair<unsigned, unsigned>;
	std::unordered_map<key_type, unsigned, Hash> fabric;
	for (auto const& claim: claims) {
		key_type key(0u, 0u);
		for (key.first = claim.xmin; key.first < claim.xmax; ++key.first) {
			for (key.second = claim.ymin; key.second < claim.ymax; ++key.second) {
				auto const result = fabric.emplace(key, 1u);
				if (!result.second) {
					++result.first->second;
				}
			}
		}
	}

	unsigned count = 0u;
	for (auto const& pair: fabric) {
		if (pair.second > 1u) {
			++count;
		}
	}
	std::printf("%u square inches overlapping\n", count);
}


void
partTwo(std::vector<Claim>& claims) {
	auto const len = unsigned(claims.size());
	for (unsigned i = 0u; i < len; ++i) {
		// Can't bail out early as we're assuming future claims will have been
		// checked against all past ones.
		for (unsigned j = i + 1u; j < len; ++j) {
			if (claims[i].overlaps(claims[j])) {
				claims[j].hasOverlap = true;
				claims[i].hasOverlap = true;
			}
		}

		if (!claims[i].hasOverlap) {
			std::printf("ID: %d\n", claims[i].id);
			break;
		}
	}
}


int
main(int argc, char const* const argv[]) {
	int part = 0;
	if (argc == 2) {
		part = std::atoi(argv[1]);
	}

	auto claims(loadInputs());
	std::printf("Parsed %lu claims\n", claims.size());
	switch (part) {
		case 1:
			partOne(claims);
			break;
		case 2:
			partTwo(claims);
			break;
		default:
			std::printf("Usage: %s <part>\n", argv[0]);
	}
	return 0;
}
