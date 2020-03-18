#include "random_generator.h"

RandomGen::RandomGen(){
	gen = boost::random::mt19937(static_cast<std::uint32_t>(std::time(0)));
}

uint32_t RandomGen::get_random(int a, int b){
	boost::random::uniform_int_distribution<> dist{a, b};
	return dist(gen);
}

std::uint64_t RandomGen::get_device_random(int a, int b){
	boost::random::uniform_int_distribution<> dist{a, b};
	return dist(dev_gen);
}
