#ifndef __SHA3_H__
#define __SHA3_H__

#include "HashStrategy.h"
#include <algorithm>
#include <memory>
#include <array>
#include <iostream>
#include <map>
#include "utils.h"

class sha3 :
	public HashStrategy
{
private:

	static const unsigned int X = 5;
	static const unsigned int Y = 5;
	static const unsigned int W = 64;
	static const unsigned int B = 1600;
	static const unsigned int NUMBER_OF_ROUNDS = 24;
	static const uint64_t keccak_round_constants[NUMBER_OF_ROUNDS];
	static const uint8_t rho_offset[NUMBER_OF_ROUNDS];
	static const uint8_t pi_positions[NUMBER_OF_ROUNDS];

	typedef std::array<uint64_t, X*Y> state_array;

	state_array A;
	std::vector<unsigned char> S;
	std::vector<unsigned char> rest;
	
	unsigned c;
	unsigned r;
	unsigned d;
	unsigned rate;

	inline uint64_t rotl64(uint64_t n, int c);
	inline uint64_t rotr64(uint64_t n, int c);

	void convertStringToStateArray(const std::vector<unsigned char>& str);
	std::vector<unsigned char> convertStateArrayToString();

	void keccakPermutation();
	void keccakTheta();
	void keccakRho();
	void keccakPi();
	void keccakChi();
	void keccakJota(unsigned int round);
	
	void rnd(unsigned int round);

	void sponge(const std::vector<unsigned char>& m);
	void keccak(const std::vector<unsigned char>& m);

public:
	sha3();
	sha3(int size);
	~sha3();
	void update(const std::vector<unsigned char>& data);
	std::string final(const std::vector<unsigned char>& data);

	virtual std::string compute(const std::vector<unsigned char>& data);

};

#endif