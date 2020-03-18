#ifndef _REANDOM_GENERATOR_H
#define _REANDOM_GENERATOR_H
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

#include <ctime>
#include <cstdint>

class RandomGen{

	boost::random::mt19937 gen;
	boost::random::random_device dev_gen;

public:
	RandomGen();
	uint32_t get_random(int a, int b);
	std::uint64_t get_device_random(int a, int b);
};

#endif //_REANDOM_GENERATOR_H
