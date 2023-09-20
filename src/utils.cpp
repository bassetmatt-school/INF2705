#include "utils.hpp"

#include <random>
#include <chrono>

double rand01()
{
	static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	static std::uniform_real_distribution<double> distribution(0, 1);
	return distribution(generator);
}