// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <cstdint>

namespace swganh {

class RandomGenerator
{
public:
    explicit RandomGenerator(boost::random::uniform_int_distribution<> distribution);
    RandomGenerator(int start, int end);
    /*
    * Gets the next random number in the generator
    */
    int Rand();
	float Randf();
    // generates a random number between the start and end
    // Closed set,meaning it can be the start and end numbers
    int Rand(int start, int end);
    float Randf(float start, float end);
	
	int LastRand();
	float LastRandf();
private:
    int last_random;
	float last_randomf;
    // Mersenne Twister Generator
    boost::random::mt19937 generator;
    boost::random::uniform_int_distribution<> distribution_;
};

}
