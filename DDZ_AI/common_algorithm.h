#pragma once
#include <unordered_map>

inline std::unordered_map<uint8_t, int> cardCountMap(std::vector<uint8_t>& cardIndex) {
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
inline size_t smallerCount(const std::vector<T>& value, T compareValue) {
	int sum = 0;
	for (auto& v : value) {
		if (v < compareValue) {
			++sum;
		}
	}
	return sum;
}