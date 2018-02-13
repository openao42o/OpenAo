#pragma once

#include <random>


constexpr auto MAX_RAND100_VALUE = 99;			// int, 0 ~ 99
constexpr auto MAX_RAND1000_VALUE = 999;		// int, 0 ~ 999
constexpr auto MAX_RAND10K_VALUE = 9999;		// int, 0 ~ 9999
constexpr auto MAX_RAND100K_VALUE = 99999;		// int, 0 ~ 99999
constexpr auto MAX_RAND1000K_VALUE = 999999;	// int, 0 ~ 999999
constexpr auto MAX_RAND256_VALUE = 255;			// int, 0 ~ 255

// provides thread local random generators
namespace random
{
	// access to the internal generator, avoid use
	std::mt19937& generator();

	// initializes the thread local random generator;
	// must be called once per thread thats uses this namespace
	inline void init()
	{
		std::random_device rd;

		generator().seed(rd());
	}

	// randomly shuffle the elements of a collection using the local random generator
	template<typename Collection_t>
	void shuffle(Collection_t& collection)
	{
		std::shuffle(
			begin(collection), 
			end(collection), 
			generator()
		);
	}
}


int RAND100();
int RAND256();
int RAND1000();
int RAND10K();
int RAND100K();
int RAND1000K();

bool RANDBOOL();

// returns a random integer in [0, max)
int RANDI(int max);

// returns a random integer in [min, max]
int RANDI(int min, int max);

// returns a random double in [min, max]
double RANDF(float min, float max);