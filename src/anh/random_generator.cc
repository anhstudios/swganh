// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "random_generator.h"

using namespace swganh;
using namespace boost::random;

RandomGenerator::RandomGenerator(uniform_int_distribution<> distribution)
    : generator()
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
int RandomGenerator::Rand(int start, int end)
{
    distribution_ = uniform_int_distribution<>(start, end);
    return Rand();
}
int RandomGenerator::LastRand()
{
    return last_random;
}