#pragma once
#include <random>
#include "RandomNumberGenerator.h"

static std::random_device rd;  //Will be used to obtain a seed for the random number engine
static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

int RandomNumberGenerator::getRandomNumber(int min, int max)
{
	std::uniform_real_distribution<> dis(min, max + 1);
	return dis(gen);
}
