#include <iostream>
#include <algorithm>
#include <limits>
#include <list>
#include <unordered_map>


using circle_type = std::list<unsigned>;
using iterator = circle_type::iterator;

using score_map = std::unordered_map<unsigned, unsigned>;


struct State {
	State(unsigned numPlayers, unsigned maxMarble)
		:	m_scores{}
		,	m_circle{0u}
		,	m_iCurrent{m_circle.begin()}
		,	m_nextMarble{0u}
		,	m_numPlayers{numPlayers}
		,	m_currentPlayer{0u}
		,	m_maxMarble{maxMarble}
	{
	}

	bool
	iterate() {
		++m_nextMarble;
		if (m_nextMarble % 23u == 0u) {
			auto const begin = m_circle.cbegin();
			for (unsigned i = 0u; i < 7u; ++i) {
				if (m_iCurrent == begin) {
					m_iCurrent = m_circle.cend();
				}
				--m_iCurrent;
			}
			unsigned const marbleRemoved = *m_iCurrent;
			m_scores[m_currentPlayer] += m_nextMarble + marbleRemoved;
			m_iCurrent = m_circle.erase(m_iCurrent);
		}
		else {
			auto const end = m_circle.end();
			if (++m_iCurrent == end) m_iCurrent = m_circle.begin();
			if (++m_iCurrent == end) m_iCurrent = m_circle.begin();
			m_iCurrent = m_circle.insert(m_iCurrent, m_nextMarble);
		}
		m_currentPlayer = (m_currentPlayer + 1u) % m_numPlayers;

		return m_nextMarble < m_maxMarble;
	}

	unsigned
	highScore() const {
		auto const iMax = std::max_element(
			m_scores.cbegin(),
			m_scores.cend(),
			[](auto const& a, auto const& b) { return a.second < b.second; });
		return iMax->second;
	}

	score_map m_scores;
	circle_type m_circle;
	circle_type::const_iterator m_iCurrent;
	unsigned m_nextMarble;
	unsigned m_numPlayers;
	unsigned m_currentPlayer;
	unsigned m_maxMarble;
};


unsigned
parseUintEnvVar(char const* const pEnvVar, unsigned defaultValue) {
	char const* const pValue = std::getenv(pEnvVar);
	if (!pValue) {
		return defaultValue;
	}
	auto const value = std::stoul(pValue);
	if (value > std::numeric_limits<unsigned>::max()) {
		throw std::out_of_range(pValue);
	}
	return unsigned(value);
}


int
main() {
	auto const numPlayers = parseUintEnvVar("PLAYERS", 9ul);
	auto const numMarbles = parseUintEnvVar("MARBLES", 25u);
	std::cout << "Players: " << numPlayers << "; marbles: " << numMarbles
		<< std::endl;
	State game(numPlayers, numMarbles);

	// Part one.
	while (game.iterate());
	std::cout << "Part one: " << game.highScore() << std::endl;

	// Part two.
	game.m_maxMarble *= 100u;
	while (game.iterate());
	std::cout << "Part two: " << game.highScore() << std::endl;
	return 0;
}
