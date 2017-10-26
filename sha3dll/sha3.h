#ifndef __SHA3_H__
#define __SHA3_H__

#include "HashStrategy.h"

class sha3 :
	public HashStrategy
{
private:
	static const unsigned int NUMBER_OF_ROUNDS = 24;

	std::vector<uint8_t> A;


	void keccakPermutation();
	void keccakTheta();
	void keccakRho();
	void keccakPi();
	void keccakChi();
	void keccakJota(unsigned int round);

	void rnd(unsigned int round);

public:
	sha3();
	sha3(int size);
	~sha3();
	std::string compute(const std::vector<char>& data);
};

#endif