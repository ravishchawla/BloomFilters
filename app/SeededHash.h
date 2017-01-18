//Used for rand()
#include <cstdlib>

//Used for standard out
#include <iostream>

//Used for File out
#include <fstream>

//Used for Natural Log
#include <cmath>

//Used for arrays.
#include <vector>

//Used to get Unix timestamp for seeding.
#include <chrono>

//Set to 1 to get Debug output.
#define DEBUG 0

class SeededHash {
	public:
		SeededHash();
		SeededHash(int seed);
		SeededHash(int seed, int prime);
		unsigned int hash(int value);

	private:
		int  seed;
		int prime;

		const int DEFAULT_PRIME = 16777619;

		unsigned int hashByte(char byte, int old_hash);
		unsigned int getRandomSeed();

};