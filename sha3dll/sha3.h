#ifndef __SHA3_H__
#define __SHA3_H__

#include "HashStrategy.h"
#include <algorithm>
#include <memory>
#include <array>
#include <iostream>

class sha3 :
	public HashStrategy
{
private:
	static const unsigned int NUMBER_OF_ROUNDS = 24;
	static const int X_MAX = 5;
	static const int Y_MAX = 5;
	static const int W_MAX = 64;

	//std::vector<std::vector<std::vector<uint8_t>>> A;
	std::array<std::array<std::array<uint8_t, W_MAX>, Y_MAX>, X_MAX> A;

	void convertStringToStateArray(const std::vector<char>& str);
	std::vector<char> convertStateArrayToString();

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