#include "sha3.h"



void sha3::keccakPermutation()
{
	// TODO przekonwertowa� S na A - prawdopodobnie w zewn�trznej funkcji
	for (int i = 0; i < NUMBER_OF_ROUNDS; ++i)
	{
		rnd(i);
	}

	// TODO przekonwertowa� A na S', prawdopodobnie w zewn�trznej funkcji
}

void sha3::keccakTheta()
{
	// TODO
}

void sha3::keccakRho()
{
	// TODO
}

void sha3::keccakPi()
{
	// TODO
}

void sha3::keccakChi()
{
	// TODO
}

void sha3::keccakJota(unsigned int round)
{
	// TODO
}

void sha3::rnd(unsigned int round)
{
	keccakTheta();
	keccakRho();
	keccakPi();
	keccakChi();
	keccakJota(round);
}

sha3::sha3()
{
}

sha3::sha3(int size)
{
	// TODO init
}


sha3::~sha3()
{
}

std::string sha3::compute(const std::vector<char>& data)
{
	return "abcde";
}
