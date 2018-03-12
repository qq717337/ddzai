#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <cstdint>

#include <random>
#include <vector>
class Random {
public:
	/*!
	* \brief Constructor, with random seed
	*/

	Random()
		:distribution_zero_to_one_(0.0, 1.0) {
		std::random_device rd;
		generator_ = std::mt19937(rd());
	}
	/*!
	* \brief Constructor, with specific seed
	*/
	static  Random common_random;
	Random(unsigned int seed)
		:generator_(seed), distribution_zero_to_one_(0.0, 1.0) {
	}
	/*!
	* \brief Generate random integer
	* \param lower_bound lower bound
	* \param upper_bound upper bound
	* \return The random integer between [lower_bound, upper_bound)
	*/
	inline int64_t NextInt(int64_t lower_bound, int64_t upper_bound) {
		// get random interge in [a,b)
		std::uniform_int_distribution<int64_t> distribution(lower_bound, upper_bound - 1);
		return distribution(generator_);
	}
	
	/*!
	* \brief Generate random float data
	* \return The random float between [0.0, 1.0)
	*/
	inline double NextDouble() {
		// get random float in [0,1)
		return distribution_zero_to_one_(generator_);
	}
	/*!
	* \brief Sample K data from {0,1,...,N-1}
	* \param N
	* \param K
	* \return K Ordered sampled data from {0,1,...,N-1}
	*/
	inline std::vector<size_t> Sample(size_t N, size_t K) {
		std::vector<size_t> ret;
		if (K > N || K < 0) {
			return ret;
		}
		for (size_t i = 0; i < N; ++i) {
			double prob = (K - ret.size()) / static_cast<double>(N - i);
			if (NextDouble() < prob) {
				ret.push_back(i);
			}
		}
		return ret;
	}
private:
	/*! \brief Random generator */
	std::mt19937 generator_;
	/*! \brief Cache distribution for [0.0, 1.0) */
	std::uniform_real_distribution<double> distribution_zero_to_one_;
};
#endif
