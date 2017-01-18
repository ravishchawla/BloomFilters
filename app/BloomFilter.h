#include "SeededHash.h"

class BloomFilter {
	public:
		BloomFilter();
		BloomFilter(int capacity);
		BloomFilter(int capacity, float c_ratio);

		void init(int capacity, int num_hash_functions, float c_ratio);
		void SetTotalHashFunctions(int total);
		int GetTotalHashFunctions();
		void SetCRatio(int value);
		bool insert(int value);
		bool query(int value);
		int size();
		bool isFull();

	private:
		int capacity;
		int total_inserted;
		float c_ratio;
		int num_hash_functions;
		std::vector<bool> filter;
		std::vector<SeededHash> hash_functions;
		int ComputeOptimalHashNumber(float c_ratio);

		int const DEFAULT_C = 8;
		int const DEFAULT_N = 10;
};