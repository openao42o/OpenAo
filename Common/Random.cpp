#include "stdafx.h"

#include "Random.h"

namespace random
{
	thread_local std::mt19937 gen;

	std::uniform_int_distribution<> dist100 { 0, 99 };
	std::uniform_int_distribution<> dist256 { 0, 255 };
	std::uniform_int_distribution<> dist1000 { 0, 999 };
	std::uniform_int_distribution<> dist10k { 0, 9999 };
	std::uniform_int_distribution<> dist100k { 0, 99999 };
	std::uniform_int_distribution<> dist1000k { 0, 999999 };

	std::bernoulli_distribution distBool { 0.5f };
	
	std::mt19937& generator() { return gen; }
}

int RAND100() { return random::dist100(random::gen); }
int RAND256() { return random::dist256(random::gen); }
int RAND1000() { return random::dist1000(random::gen); }
int RAND10K() { return random::dist10k(random::gen); }
int RAND100K() { return random::dist100k(random::gen); }
int RAND1000K() { return random::dist1000k(random::gen); }

bool RANDBOOL() { return random::distBool(random::gen); }

int RANDI(int max)
{
	std::uniform_int_distribution<int> d { 0, max - 1 };

	return d(random::gen);
}

int RANDI(int min, int max)
{
	std::uniform_int_distribution<int> d { min, max };

	return d(random::gen);
}

double RANDF(float min, float max)
{
	std::uniform_real_distribution<double> d { min, max };

	return d(random::gen);
}