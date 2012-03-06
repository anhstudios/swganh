#include "random_generator.h"

using namespace anh;
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