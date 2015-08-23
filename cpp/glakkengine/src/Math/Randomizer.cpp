/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Math/Randomizer.hpp"

#include <cstdlib>

using kke::Randomizer;

void Randomizer::SetSeed(unsigned int seed)
{
	srand(seed);
}

float Randomizer::Random(float first, float last)
{
	return ((last - first) * ((float) rand() / RAND_MAX)) + first;
}

int Randomizer::Random(int first, int last)
{
	return rand() % last + first;
}
