// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "random_generator.h"

#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

using namespace swganh;
using namespace boost::random;

RandomGenerator::RandomGenerator(uniform_int_distribution<> distribution)
    : generator(rand())
    , distribution_(distribution)    
{
}

RandomGenerator::RandomGenerator(int start, int end)
    : generator()
    , distribution_(uniform_int_distribution<>(start, end))
{
}

int RandomGenerator::Rand()
{
    int rand = distribution_(generator);
    last_random = rand;
    return rand;
}

float RandomGenerator::Randf()
{
	boost::uniform_real<float> u(0.0f, 1.0f);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > gen(generator, u);
	last_randomf = gen();
	return last_randomf;
}

int RandomGenerator::Rand(int start, int end)
{
    distribution_ = uniform_int_distribution<>(start, end);
    return Rand();
}

float RandomGenerator::Randf(float start, float end)
{
	boost::uniform_real<float> u(start, end);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > gen(generator, u);
	last_randomf = gen();
	return last_randomf;
}


int RandomGenerator::LastRand()
{
    return last_random;
}

float RandomGenerator::LastRandf()
{
	return last_randomf;
}