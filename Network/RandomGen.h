#pragma once

class RandomGen
{
private:
	static long Seed;
	RandomGen() {} // do not allow instances of this class
public:
	static void Init(long seed);
	static double Uniform();
	static double Nonuniform(double a, double b);
	static double Exponential(double lamda);
};