#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <list>
#include <iomanip>

#include "settings.h"


struct Guard {
	unsigned char m_minutes[60];
	unsigned m_totalSleepMins = 0u;
	unsigned m_sleepiestMinute = 0u;

	inline void
	calcSleepiestMinute() {
		m_sleepiestMinute = std::distance(
			&m_minutes[0], std::max_element(&m_minutes[0], &m_minutes[60]));
	}

	inline unsigned
	sleepiestMinuteValue() const noexcept {
		return m_minutes[m_sleepiestMinute];
	}

	void
	sleepBetween(unsigned begin, unsigned end) noexcept {
		m_totalSleepMins += (end - begin);
		for (; begin < end; ++begin) {
			++m_minutes[begin];
		}
	}
};


using guard_map = std::unordered_map<unsigned, Guard>;


template <typename OS>
OS& operator<<(OS& os, std::pair<unsigned, Guard> const& pair) {
	return os
		<< "Guard(id: " << pair.first
		<< ", sleepiest minute: " << pair.second.m_sleepiestMinute
		<< ", slept: " << pair.second.sleepiestMinuteValue()
		<< ", answer: " << pair.first * pair.second.m_sleepiestMinute
		<< ')';
}


guard_map
loadGuards() {
	std::ifstream ifs{INPUTS_DIR "04.txt"};
	std::list<std::string> lines;
	unsigned count = 0u;
	{
		std::string line;
		while (std::getline(ifs, line)) {
			lines.emplace_back(std::move(line));
			++count;
		}
	}
	lines.sort();

	unsigned guardId = 0u;
	unsigned lastMinute = 0u;
	std::stringstream ss;
	guard_map guards;
	guard_map::iterator iGuard = guards.end();

	for (auto&& line: lines) {
		unsigned minute;
		ss << line;
		ss.ignore(15, ':');
		ss >> minute;
		ss.ignore(2, ' ');
		char const c = ss.get();
		switch (c) {
			case 'G':
				ss.ignore(6, '#');
				ss >> guardId;
				iGuard = guards.emplace(
					std::piecewise_construct,
					std::forward_as_tuple(guardId),
					std::forward_as_tuple()
				).first; // Don't care if it's new or not.
				break;
			case 'f':
				lastMinute = minute;
				break;
			case 'w':
				iGuard->second.sleepBetween(lastMinute, minute);
				break;
			default:
				std::cerr << "Invalid char '" << c << "' at pos "
					<< ss.tellg() << " on line \"" << line << '"' << std::endl;
				break;
		}
		ss.seekg(0, std::ios_base::end);
	}

	for (auto&& pair: guards) {
		pair.second.calcSleepiestMinute();
	}
	return guards;
}


int
main() {
	auto const guards{loadGuards()};

	{ // Part 1.
		auto const iHighestTotal = std::max_element(
			std::begin(guards),
			std::end(guards),
			[](auto const& a, auto const& b) {
				return a.second.m_totalSleepMins < b.second.m_totalSleepMins;
			});

		std::cout << "[Part one]: " << *iHighestTotal << std::endl;
	}

	{ // Part 2.
		auto const iHighestFreq = std::max_element(
			std::begin(guards),
			std::end(guards),
			[](auto const& a, auto const& b) {
				return a.second.sleepiestMinuteValue() <
					b.second.sleepiestMinuteValue();
			});

		std::cout << "[Part two]: " << *iHighestFreq << std::endl;
	}

	return 0;
}
