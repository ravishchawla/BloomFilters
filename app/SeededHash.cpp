#include "SeededHash.h"

/*
 * Default Constructor, uses a default Prime value, and gets a random seed.
 */	
SeededHash::SeededHash() {
	this->seed = getRandomSeed();
	this->prime = DEFAULT_PRIME;
}

/*
 * Uses a provided seed, with a default Prime value.
 */	
SeededHash::SeededHash(int seed) {
	this->seed = seed;
	this->prime = DEFAULT_PRIME;
}

/*
 * Uses provided seed value and prime value to initialize.
 */	
SeededHash::SeededHash(int seed, int prime) {
	this->seed = seed;
	this->prime = prime;
}

/*
 * Hashes a given value and returns the Hashed index.
 */	
unsigned int SeededHash::hash(int value) {

	//Iterate over bytes, and hash each byte.
	unsigned char *bytePtr = (unsigned char *)&value;
	unsigned int hashValue = seed;
	for(int i = 0; i < sizeof(value); i++) {
		hashValue = hashByte(bytePtr[i], hashValue); 
	}

	return hashValue;
}

/*
 * Hashes a byte of data and returns the hashed value.
 */	
unsigned int SeededHash::hashByte(char byte, int old_hash) {

	//The hashed value is the product of an XOR of the previous hash with the current byte.
	return (byte ^ old_hash) * prime;
}

/*
 * Gets a random seed value.
 */	
unsigned int SeededHash::getRandomSeed() {

	//Initialize rand with current unix timestamp in milliseconds.
	std::srand(std::chrono::system_clock::now().time_since_epoch().count());
	return std::rand();
}
