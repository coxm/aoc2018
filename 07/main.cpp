#include <iostream>
#include <cstdio>
#include <vector>
#include <memory>
#include <algorithm>

#include "settings.h"


constexpr char NOTHING = '@';


class Step {
public:
	static char s_extraCost;

	inline static unsigned
	indexOf(char c) noexcept {
		return c - 'A';
	}

	inline static unsigned
	indexOf(Step const step) {
		return indexOf(step.m_name);
	}

	Step(char name)
		:	m_dependencies(0u)
		,	m_name(name)
		,	m_remainingSeconds(s_extraCost + name - NOTHING)
	{
	}

	inline char name() const noexcept { return m_name; }

	bool
	operator<(Step const& other) const noexcept {
		auto const thisName = name();
		auto const otherName = other.name();
		return other.dependsOn(thisName) ||
			(!dependsOn(otherName) && thisName < otherName);
	}

	void
	addDependency(char other) noexcept {
		m_dependencies = m_dependencies | (1 << indexOf(other));
	}

	bool
	dependsOn(char other) const noexcept {
		return m_dependencies & (1 << indexOf(other));
	}

	template <typename Iter>
	bool
	allDepsAreIn(Iter begin, Iter end) const noexcept {
		unsigned remaining = m_dependencies;
		for (; begin != end; ++begin) {
			auto const index = indexOf(*begin);
			remaining &= ~(1 << index);
		}
		return !remaining;
	}

	inline void markUsed(bool used = true) noexcept { m_isUsed = used; }

	inline bool isUsed() const noexcept { return m_isUsed; }

	inline bool isComplete() const noexcept { return m_isComplete; }

	inline char remainingSeconds() const noexcept
	{ return m_remainingSeconds; }

	Step& operator--() noexcept {
		if (!m_isComplete) {
			m_isComplete = (--m_remainingSeconds == 0);
		}
		return *this;
	}

private:
	unsigned m_dependencies;
	char m_name;
	char m_remainingSeconds;
	bool m_isUsed = false;
	bool m_isComplete = false;
};


char Step::s_extraCost = 0;


struct FileDeleter {
	inline void operator()(FILE* pFile) const noexcept { fclose(pFile); }
};


std::vector<Step>
partOne() {
	unsigned count = 0u;
	std::vector<Step> steps;
	steps.reserve(26);
	for (char c = 'A'; c <= 'Z'; ++c) {
		steps.emplace_back(c);
	}

	std::unique_ptr<FILE, FileDeleter> pFile(fopen(INPUTS_DIR "07.txt", "r"));
	while (!std::feof(pFile.get())) {
		char first, second;
		int const result = std::fscanf(
			pFile.get(),
			"Step %c must be finished before step %c can begin.\n",
			&first, &second);
		if (result != 2 || second < 'A') {
			std::printf("Error parsing input!\n");
			break;
		}

		unsigned const thisMax(1u + std::max(
			Step::indexOf(first), Step::indexOf(second)));
		count = std::max(count, thisMax);
		steps[Step::indexOf(second)].addDependency(first);
	}
	steps.erase(steps.begin() + count, steps.end());
	std::printf("Loaded %u steps\n", count);

	std::vector<Step> sorted;
	sorted.reserve(count);
	std::printf("Part one: ");
	for (unsigned i = 0u; i < count; ++i) {
		for (auto& step: steps) {
			if (step.isUsed()) {
				continue;
			}

			if (step.allDepsAreIn(std::begin(sorted), std::end(sorted))) {
				std::printf("%c", step.name());
				step.markUsed();
				sorted.push_back(step);
				break;
			}
		}
	}
	std::printf("\n");
	return sorted;
}


void
partTwo(std::vector<Step>& steps, unsigned numWorkers) {
	for (auto& step: steps) {
		step.markUsed(false);
	}

	char done[26];
	unsigned nextDone = 0u;

	unsigned sec = 0u;
	unsigned numWorkersInUse = 0u;
	for (; nextDone < steps.size(); ++sec) {
		// Decrement remaining seconds for active steps.
		for (auto& step: steps) {
			if (step.isUsed()) {
				--step;

				if (step.isComplete()) {
					step.markUsed(false);
					done[nextDone++] = step.name();
					--numWorkersInUse;
					// std::printf("Done %c at %us\n", step.name(), sec);
				}
			}
		}

		if (numWorkersInUse == numWorkers) {
			continue;
		}

		// If any have been completed, reassign to another worker.
		for (auto& step: steps) {
			if (step.isUsed() || step.isComplete()) {
				continue;
			}
			if (step.allDepsAreIn(&done[0], &done[nextDone])) {
				step.markUsed(true);
				// std::printf("Working on %c\n", step.name());
				if (++numWorkersInUse >= numWorkers) {
					break;
				}
			}
		}
	}

	std::printf("Part two: %u seconds\n", std::max(sec, 1u) - 1u);
}


int
main() {
	// Configuration.
	char const* const pCost = std::getenv("COST");
	if (pCost) {
		Step::s_extraCost = char(std::atoi(pCost));
	}
	char const* const pNumWorkers = std::getenv("WORKERS");
	unsigned const numWorkers = pNumWorkers
		?	unsigned(std::atoi(pNumWorkers))
		:	5u;
	std::printf(
		"Extra cost: %u; num workers: %u\n",
		unsigned(Step::s_extraCost),
		numWorkers);

	auto steps{partOne()};
	partTwo(steps, numWorkers);
	return 0;
}
