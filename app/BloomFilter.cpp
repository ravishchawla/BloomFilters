#include "BloomFilter.h"

/*
 * Default constructor, initializes with default values.
 */
BloomFilter::BloomFilter() {
	init(this->DEFAULT_N, ComputeOptimalHashNumber(this->DEFAULT_C), this->DEFAULT_C);
}

/*
 * Constructor when only capacity is provided. Number of ptimal hash functions is 
 * computed with default c value.
 */
BloomFilter::BloomFilter(int capacity) {
	init(capacity, ComputeOptimalHashNumber(this->DEFAULT_C), this->DEFAULT_C);
}

/*
 * Constructor to initialize both capacity and c ratio. Optimal hash function is 
 * computed with c ratio.
 */
BloomFilter::BloomFilter(int capacity, float c_ratio) {
	init(capacity, ComputeOptimalHashNumber(c_ratio), c_ratio);
}

/*
 * Initilization function. 
 */	
void BloomFilter::init(int capacity, int num_hash_functions, float c_ratio) {
	this->capacity = capacity;
	this->num_hash_functions = num_hash_functions;
	this->total_inserted = 0;
	this->c_ratio = c_ratio;

	filter = std::vector<bool>(capacity, 0);
	this->SetTotalHashFunctions(num_hash_functions);

	#if DEBUG == 1
	std::cout << "Initialized with n = " << this->capacity 
			  << " k = " << this->num_hash_functions << std::endl;
	#endif
}

/*
 * Sets total number of hash functions, and instantiates the number of instances
 * and puts them in an array.
 */	
void BloomFilter::SetTotalHashFunctions(int total) {
	for(int i = 0; i < total; i++) {
		hash_functions.push_back(SeededHash());
	}
}

/*
 * Returns number of hash functions.
 */	
int BloomFilter::GetTotalHashFunctions() {
	return hash_functions.size();
}

/*
 * Sets C ratio.
 */	
void BloomFilter::SetCRatio(int value) {
	this->c_ratio = value;
}

/*
 * Inserts a value into th Bloom Filter by setting multiple Hash Indices.
 */	

bool BloomFilter::insert(int value) {

	//If the maximum allowable number of values have been inserted, return without inserting.
	if(isFull()) {
		return 0;
	}

	//Iterate though all Hash Functions, get a Hash Index for the element, and set its value.
	int hash_pos;
	for(auto it = hash_functions.begin(); 
			 it != hash_functions.end(); it++) {

		hash_pos = it->hash(value) % this->capacity;

		filter[hash_pos] = 1;
	}

	#if DEBUG == 1
	std::cout << "Inserted Value: " << value << std::endl;
	#endif

	//Increment Total Inserted counter and return True.
	this->total_inserted++;
	return 1;
}

/*
 * Queries an element. Returns True if its in the Filter, False Otherwise.
 */	
bool BloomFilter::query(int value) {

	//Iterate though all Hash Functions, get Hash Indices that correspond to it, and 
	//check if it is set. Return False if any index is unset.
	int hash_pos;
	for(auto it = hash_functions.begin();
			 it != hash_functions.end(); it++) {

		hash_pos = it->hash(value) % this->capacity;
		if(!filter[hash_pos]) return 0;
	}

	#if DEBUG == 1
	std::cout << "Queried Value: " << value << std::endl;
	#endif

	//If all values are set, return True.
	return 1;
}

/*
 * Computes the Optimal Hash Number using the c ratio value.
 */	
int BloomFilter::ComputeOptimalHashNumber(float c_ratio) {

	#if DEBUG == 1
	std::cout << "Hashes: " << ((float)c_ratio) * log(2) << std::endl;
	#endif

	return c_ratio * log(2);
}

/*
 * Returns the number of elements inserted.
 */	
int BloomFilter::size() {
	return this->total_inserted;
}

/*
 * Returns True if m < n/c, False otherwise.
 */	
bool BloomFilter::isFull() {

	//If no elements have been inserted, return False.
	if(this->total_inserted <= 0) return 0;

	#if DEBUG == 1
	std::cout << "Fill ratio: " << ((float)this->capacity)/((float)this->total_inserted) << std::endl;
	#endif

	return (((float)this->capacity)/((float)this->total_inserted) - this->c_ratio < 1);
}

/*
 * Main driver of the Program.
 */	
int main() {

	//Output is printed to a file.
	std::ofstream file;
	file.open("../data/bloom_filter_out.csv");

	file << "n";

	//The values of n used are fixed. They must be strictly increasing.
	int n_vals[] = {500, 2000, 8000, 64000, 256000};

	//The values of c used are fixed.
	int c_vals[] = {2, 4, 5, 6, 7, 8, 9, 10};

	int n_len = sizeof(n_vals)/sizeof(n_vals[0]);
	int c_len = sizeof(c_vals)/sizeof(c_vals[0]);

	//printing the header to the file.
	for(int n_pos = 0; n_pos < n_len; n_pos++) {
		file << ",n = " << n_vals[n_pos]; 
	}

	file << ",Theoretical";
	file << "\n";

	
	std::vector<int> dataset;
	//Rand() function is seeded with current Unix timestamp in milliseconds.
	std::srand(std::chrono::system_clock::now().time_since_epoch().count());

	//The dataset is an array of random numbers, of size max-n.
	for(int i = 0; i < n_vals[n_len - 1]; i++) {
		dataset.push_back(std::rand());
	}

	//Iterating over the values of c.
	for(int c_pos = 0; c_pos < c_len; c_pos++) { 
		int c = c_vals[c_pos];
		file << c;
		int prev_f = 0;

		//Iterating over the values of n, for each c.
		for(int n_pos = 0; n_pos < n_len; n_pos++) {
			int n = n_vals[n_pos];

			//If c > n, continue to next iteration.
			if(c >= n) {
				file << "," << prev_f;
				continue;
			}

			//Create a new Bloom Filter instance.
			BloomFilter *filter(new BloomFilter(n, c));

			int k = filter->GetTotalHashFunctions();
			int num_inserted = 0;

			//Insert the maximum allowable elements into the filter.
			//The filter returns False when m exceeds n/c
			bool res = 1;
			while (res) {
				res = filter->insert(dataset.at(num_inserted));
				if(res) {
					num_inserted++;
				}
			}

			//Iterate over all values of n, and accumulate values needed to calculate False
			//Positive rate.
			float false_positive_rate = 0;
			int num_found = 0;
			int num_not_found = 0;
			int num_false_negatives = 0;

			for(int i = 0; i < n; i++) {
				if(filter->query(dataset.at(i))) {
					num_found++;
				} else {
					num_not_found++;

					if(i < num_inserted) {
						num_false_negatives++;
					}
				}
			}

			//Calculating the False Positive Rate
			int num_false_positives = num_found - num_inserted;
			false_positive_rate = (float)num_false_positives/(num_false_positives + num_not_found);

			//Delete the instance of Bloom Filter
			delete filter;

			//Print the results to standard output.
			std::cout << "n: " << n << " c: " << c << std::endl;
			std::cout << "Num Inserted/total: " << num_inserted << "/" << n << std::endl;
			std::cout << "Num Found/Num Not Found: " << num_found << "/" << num_not_found << std::endl;
			std::cout << "False Positives/False Negatives: " << num_false_positives << "/" << num_false_negatives << std::endl;
			std::cout << "False Positive rate: " << false_positive_rate << std::endl;
			std::cout << "=========================================================" << std::endl;

			//Print the False Positive Rate to the file.
			file << "," << false_positive_rate;
			prev_f = false_positive_rate;
		}

		//Print the Theoretical rate to the file, for each value of c.
		file << "," << pow(0.6185, c);
		file << "\n";
	}
	
	//Close the file.
	file.close();
}