/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_RANDOMIZER_HPP
#define KKE_RANDOMIZER_HPP

#include "Interface/INoncopyable.hpp"

namespace kke
{
class Randomizer : public INoncopyable
{
public:
	/**
	 * @brief Set seed of randomizer.
	 *
	 * @param seed Seed.
	 * @return void
	 **/
	static void SetSeed(unsigned int seed);

	/**
	 * @brief Randomize a value between values.
	 *
	 * @param first Minimum value to randomize.
	 * @param last Maximum value to randomize.
	 * @return float
	 **/
	static float Random(float first, float last);

	/**
	 * @brief Randomize a value between values.
	 *
	 * @param first Minimum value to randomize.
	 * @param last Maximum value to randomize.
	 * @return float
	 **/
	static int Random(int first, int last);
private:

	Randomizer();
	~Randomizer();
};
}

#endif // KKE_RANDOMIZER_HPP
