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

	enum bit : uint8_t {
		ZERO = 0x00,
		ONE = 0x01
	};

	static const unsigned int NUMBER_OF_ROUNDS = 24;
	static const unsigned int X_MAX = 5;
	static const unsigned int Y_MAX = 5;
	static const unsigned int W_MAX = 64;
	static constexpr unsigned int B = X_MAX * Y_MAX * W_MAX;
	static constexpr unsigned int BITS_IN_CHAR = sizeof(char) * 8;
	static constexpr unsigned int Z_MAX = W_MAX / BITS_IN_CHAR;

	//typedef std::array<std::array<std::array<unsigned char, Z_MAX>, Y_MAX>, X_MAX> state_array;
	typedef std::array<std::array<std::array<bit, W_MAX>, Y_MAX>, X_MAX> state_array;

	std::vector<bit> S;
	state_array A;
	unsigned c;
	unsigned r;
	unsigned d;
	unsigned rate;

	std::vector<unsigned char> rest;

	inline std::vector<bit> xor(const std::vector<bit>& f, const std::vector<bit>& s);

	std::vector<bit> convertStringToBits(const std::vector<unsigned char>& str);
	std::vector<unsigned char> convertBitsToString(const std::vector<bit>& bits);
	void convertStringToStateArray(const std::vector<bit>& str);
	std::vector<bit> convertStateArrayToString();

	void pad10_1(unsigned x, unsigned m, std::vector<bit>& p);

	std::vector<sha3::bit> keccakPermutation(std::vector<bit>& m);
	void keccakTheta();
	void keccakRho();
	void keccakPi();
	void keccakChi();
	void keccakJota(unsigned int round);
	
	bit rc(unsigned t);
	void rnd(unsigned int round);

	std::vector<bit> sponge(std::vector<bit>& m, bool isFinal);
	std::vector<unsigned char> keccak(std::vector<bit>& m, bool isFinal);

public:
	sha3();
	sha3(int size);
	~sha3();
	void update(const std::vector<unsigned char>& data);
	std::string final(const std::vector<unsigned char>& data);

	virtual std::string compute(const std::vector<unsigned char>& data);

};

#endif