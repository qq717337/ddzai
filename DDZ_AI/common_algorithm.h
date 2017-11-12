#pragma once
#include <unordered_map>
#include "random.h"
inline std::unordered_map<uint8_t, int> cardCountMap(CardVector & cardIndex) {
	std::unordered_map<uint8_t, int>m;
	for (auto &v : cardIndex) {
		std::unordered_map<uint8_t, int>::iterator iter = m.find(v);
		if (iter == m.end()) {
			m.insert(std::make_pair(v, 1));
		}
		else {
			m[v]++;
		}
	}
	return m;
}

template<typename T>
inline T randomChoice(const std::vector<T>& stl) {
	auto index = CommonRandom.NextInt(0, stl.size());
	return stl[index];
}
template<typename T>
inline T randomChoice(const std::set<T>& stl) {
	auto index = CommonRandom.NextInt(0, stl.size());
	do {
		auto iter = stl.begin();
		++iter;
		--index;
	} while (index > 0);
	return *iter;
}
template<typename T>
inline size_t smallerCount(const std::vector<T>& value, T compareValue) {
	int sum = 0;
	for (auto& v : value) {
		if (v < compareValue) {
			++sum;
		}
	}
	return sum;
}

template<typename T>
inline void perm(size_t randomCount,std::vector<T>&value ) {
	if (randomCount == 0)return;
	value.resize(randomCount);
	int start = 0;
	for (; start < randomCount; ++start) {
		value[start] = start;
	}

	for (size_t i = randomCount - 1; i >= 1; --i)
		std::swap(value[i], value[rand() % i]);
}
template<typename T>
inline void perm(size_t randomCount, T *value) {
	if (randomCount == 0)return;
	int start = 0;
	for (; start < randomCount; ++start) {
		value[start] = start;
	}

	for (size_t i = randomCount - 1; i >= 1; --i)
		std::swap(value[i], value[rand() % i]);
}