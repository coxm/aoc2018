#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

#include "settings.h"


struct Location {
	Location(int x, int y)
		:	m_x(x)
		,	m_y(y)
		,	m_name(s_lastName++)
	{
	}

	int
	distanceFrom(int x, int y) const noexcept {
		return std::abs(m_x - x) + std::abs(m_y - y);
	}

	int m_x;
	int m_y;
	unsigned m_area = 0u;
	bool m_grew = false;
	char m_name;

private:
	static char s_lastName;
};


char Location::s_lastName = 'A';


std::vector<Location>
loadLocations() {
	std::ifstream ifs(INPUTS_DIR "06.txt");
	std::vector<Location> locations;
	while (true) {
		int x, y;
		ifs >> x;
		if (!ifs.good()) {
			break;
		}
		ifs.get(); // ','.
		ifs >> y;
		locations.emplace_back(x, y);
		ifs.get(); // Endline.
	}
	return locations;
}


void
compute(std::vector<Location>& locations, int x, int y) {
	int iBest = 0;
	int minDist = locations[0].distanceFrom(x, y);
	for (int i = 1, len = int(locations.size()); i < len; ++i) {
		auto const& loc{locations[i]};
		int const dist = loc.distanceFrom(x, y);
		if (dist < minDist) {
			minDist = dist;
			iBest = i;
		}
		else if (dist == minDist) {
			iBest = -1;
		}
	}

	if (iBest != -1) {
		++locations[iBest].m_area;
		locations[iBest].m_grew = true;
	}
}


void
partOne(
	std::vector<Location> const& allLocations,
	int const xmin,
	int const xmax,
	int const ymin,
	int const ymax
) {
	std::vector<Location> locations;
	locations.reserve(allLocations.size());
	std::copy_if(
		std::begin(allLocations),
		std::end(allLocations),
		std::back_inserter(locations),
		[xmin, ymin, xmax, ymax] (auto const& loc) {
			return loc.m_x != xmin && loc.m_x != xmax &&
				loc.m_y != ymin && loc.m_y != ymax;
		});

	for (int y = 0; y <= ymax; ++y) {
		for (int x = 0; x <= xmax; ++x) {
			compute(locations, x, y);
		}
	}
	for (auto& loc: locations) {
		loc.m_grew = false;
	}
	for (int x = 0; x <= xmax; ++x) {
		compute(locations, x, ymax);
		compute(locations, x, -1);
	}
	for (int y = 0; y <= ymax; ++y) {
		compute(locations, xmax, y);
		compute(locations, -1, y);
	}

	auto const iLocation = std::max_element(
		locations.begin(),
		locations.end(),
		[](auto const& a, auto const& b) {
			// If both the same finiteness, compare areas.
			if (a.m_grew == b.m_grew) {
				return a.m_area < b.m_area;
			}
			// Otherwise, consider the infinite one as less.
			return a.m_grew;
		});

	std::cout << "Largest finite area: " << iLocation->m_area << std::endl;
}


int
isInRegion(std::vector<Location> const& locations, int x, int y) {
	int dist = 0;
	for (auto const& loc: locations) {
		dist += loc.distanceFrom(x, y);
		if (dist >= 10000) {
			return 0;
		}
	}
	return 1;
}


void
partTwo(
	std::vector<Location> const& locations,
	int xmin,
	int xmax,
	int ymin,
	int ymax
) {
	// If the region is contained within the bounds, there's no point in
	// checking outside it.
	// TODO: this optimisation could be improved by e.g. growing the bounds
	// outrwards and/or inwards instead.
	for (int x = 0; x < xmax; ++x) {
		if (isInRegion(locations, x, 0) || isInRegion(locations, x, ymax)) {
			xmax += 10000;
			break;
		}
	}
	for (int y = 0; y < xmin; ++y) {
		if (isInRegion(locations, 0, y) || isInRegion(locations, xmax, y)) {
			ymax += 10000;
			break;
		}
	}
	std::cout << "Checking bounds: [" << xmin << ", " << xmax << "] x ["
		<< ymin << ", " << ymax << ']' << std::endl;

	int regionSize = 0;
	for (int y = 0; y <= ymax; ++y) {
		for (int x = 0; x <= xmax; ++x) {
			regionSize += isInRegion(locations, x, y);
		}
	}
	std::cout << "Region size: " << regionSize << std::endl;
}


int
main() {
	auto const locations{loadLocations()};
	int xmax = 0;
	int ymax = 0;
	int xmin = std::numeric_limits<int>::max();
	int ymin = xmin;
	for (auto const& loc: locations) {
		xmin = std::min(xmin, loc.m_x);
		xmax = std::max(xmax, loc.m_x);
		ymin = std::min(ymin, loc.m_y);
		ymax = std::max(ymax, loc.m_y);
	}

	partOne(locations, xmin, xmax, ymin, ymax);
	partTwo(locations, xmin, xmax, ymin, ymax);
	return 0;
}
