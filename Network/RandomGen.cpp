#include "RandomGen.h"
#include <cstdlib>
#include <cmath>


void RandomGen::Init(long seed)
{
	RandomGen::Seed = seed;
	srand(seed);
}

double RandomGen::Uniform()
{
	return (double)rand() / RAND_MAX;
}

double RandomGen::Uniform(double a, double b)
{
	double u = RandomGen::Uniform();
	return a + u * (b - a);
}

double RandomGen::Exponential(double lamda)
{
	double u = RandomGen::Uniform();
	return -(1 / lamda) * log(u);
}