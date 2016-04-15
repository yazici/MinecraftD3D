#ifndef _PerlinNoise_H
#define _PerlinNoise_H

#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

class PerlinNoise
{
public:
	PerlinNoise();
	~PerlinNoise();

	// Generate a new hash map
	void SetSeed(int Seed = -1);
	
	// Getters
	double CreateNoise(double x, double y, double z);
	double Fade(double t);
	double Lerp(double t, double a, double b);
	double Gradient(int hash, double x, double y, double z);

private:
	std::vector<int> HashMap_;
};

#endif