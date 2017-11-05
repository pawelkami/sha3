#include "sha3.h"

const uint64_t sha3::keccak_round_constants[NUMBER_OF_ROUNDS] = {
	0x0000000000000001, 0x0000000000008082, 0x800000000000808A, 0x8000000080008000,
	0x000000000000808B, 0x0000000080000001, 0x8000000080008081, 0x8000000000008009,
	0x000000000000008A, 0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
	0x000000008000808B, 0x800000000000008B, 0x8000000000008089, 0x8000000000008003,
	0x8000000000008002, 0x8000000000000080, 0x000000000000800A, 0x800000008000000A,
	0x8000000080008081, 0x8000000000008080, 0x0000000080000001, 0x8000000080008008
};

const uint8_t sha3::rho_offset[NUMBER_OF_ROUNDS] = {
	1, 62, 28, 27, 36, 44, 6, 55, 20, 3, 10, 43,
	25, 39, 41, 45, 15, 21, 8, 18, 2, 61, 56, 14
};

const uint8_t sha3::pi_positions[NUMBER_OF_ROUNDS] = {
	6, 12, 18, 24, 3, 9, 
	10, 16, 22, 1, 7, 13, 
	19, 20, 4, 5, 11, 17, 
	23, 2, 8, 14, 15, 21 
};

uint64_t sha3::rotl64(uint64_t n, int c)
{
	uint8_t s = c >= 0 ? c % 64 : -((-c) % 64);
	return (n << s) | (n >> (64 - s));
}

uint64_t sha3::rotr64(uint64_t n, int c)
{
	uint8_t s = c >= 0 ? c % 64 : -((-c) % 64);
	return (n >> s) | (n << (64 - s));
}

void sha3::convertStringToStateArray(const std::vector<unsigned char>& str)
{
	int j = 0;
	for (int i = 0; i < X*Y; ++i)
		A[i] =  ((uint64_t)(str[j++]) & 0x00000000000000FFU) |
				((uint64_t)(str[j++]) << 8 & 0x000000000000FF00U) |
				((uint64_t)(str[j++]) << 16 & 0x0000000000FF0000U) |
				((uint64_t)(str[j++]) << 24 & 0x00000000FF000000U) |
				((uint64_t)(str[j++]) << 32 & 0x000000FF00000000U) |
				((uint64_t)(str[j++]) << 40 & 0x0000FF0000000000U) |
				((uint64_t)(str[j++]) << 48 & 0x00FF000000000000U) |
				((uint64_t)(str[j++]) << 56 & 0xFF00000000000000U);
}

std::vector<unsigned char> sha3::convertStateArrayToString()
{
	std::vector<unsigned char> str;

	for (int i = 0; i < X*Y; ++i)
	{
		str.push_back(static_cast<unsigned char>(uint8_t((A[i] & 0x00000000000000FFU))));
		str.push_back(static_cast<unsigned char>(uint8_t((A[i] & 0x000000000000FF00U) >> 8)));
		str.push_back(static_cast<unsigned char>(uint8_t((A[i] & 0x0000000000FF0000U) >> 16)));
		str.push_back(static_cast<unsigned char>(uint8_t((A[i] & 0x00000000FF000000U) >> 24)));
		str.push_back(static_cast<unsigned char>(uint8_t((A[i] & 0x000000FF00000000U) >> 32)));
		str.push_back(static_cast<unsigned char>(uint8_t((A[i] & 0x0000FF0000000000U) >> 40)));
		str.push_back(static_cast<unsigned char>(uint8_t((A[i] & 0x00FF000000000000U) >> 48)));
		str.push_back(static_cast<unsigned char>(uint8_t((A[i] & 0xFF00000000000000U) >> 56)));
	}

	return std::move(str);
}

std::vector<unsigned char> sha3::keccakPermutation(const std::vector<unsigned char>& m)
{
	convertStringToStateArray(m);
	
	for (int i = 0; i < NUMBER_OF_ROUNDS; ++i)
		rnd(i);

	return std::move(convertStateArrayToString());
}

void sha3::keccakTheta()
{
	uint64_t C[X], D[X];

	for (int x = 0; x < X; x++)
		C[x] = A[x] ^ A[x + 5] ^ A[x + 10] ^ A[x + 15] ^ A[x + 20];
	
	D[0] = rotl64(C[1], 1) ^ C[4];
	D[1] = rotl64(C[2], 1) ^ C[0];
	D[2] = rotl64(C[3], 1) ^ C[1];
	D[3] = rotl64(C[4], 1) ^ C[2];
	D[4] = rotl64(C[0], 1) ^ C[3];

	for (int x = 0; x < X; x++)
	{
		A[x] ^= D[x];
		A[x + 5] ^= D[x];
		A[x + 10] ^= D[x];
		A[x + 15] ^= D[x];
		A[x + 20] ^= D[x];
	}
}

void sha3::keccakRho()
{
	for (int i = 1; i < X*Y; ++i)
		A[i] = rotl64(A[i], rho_offset[i-1]);
}

void sha3::keccakPi()
{
	state_array A_prim = A;

	for (int i = 1; i < X*Y; ++i)
		A_prim[i] = A[pi_positions[i-1]];

	A = A_prim;
}

void sha3::keccakChi()
{
	for (int i = 0; i < X*Y; i += 5) {
		uint64_t A0 = A[0 + i], A1 = A[1 + i];
		A[0 + i] ^= ~A1 & A[2 + i];
		A[1 + i] ^= ~A[2 + i] & A[3 + i];
		A[2 + i] ^= ~A[3 + i] & A[4 + i];
		A[3 + i] ^= ~A[4 + i] & A0;
		A[4 + i] ^= ~A0 & A1;
	}
}

void sha3::keccakJota(unsigned int round)
{
	A[0] ^= keccak_round_constants[round];
}

void sha3::rnd(unsigned int round)
{
	keccakTheta();
	keccakRho();
	keccakPi();
	keccakChi();
	keccakJota(round);
}

std::vector<unsigned char> sha3::sponge(const std::vector<unsigned char>& m)
{
	std::vector<unsigned char> data;

	int i = 0;
	for (; i < m.size(); ++i)
		data.push_back((unsigned char)(S[i] ^ m[i]));
	data.insert(data.end(), S.begin() + i, S.end());

	S = keccakPermutation(data);
	return S;
}

std::vector<unsigned char> sha3::keccak(const std::vector<unsigned char>& m)
{
	return sponge(m);
}

sha3::sha3()
{
}

sha3::sha3(int size)
{
	d = size;
	c = 2 * size; 
	r = B - c; // r = b - c
	rate = r >> 3;

	S.assign(B, 0);
}


sha3::~sha3()
{
}

void sha3::update(const std::vector<unsigned char>& data)
{
	std::vector<std::vector<unsigned char> > p;
	std::vector<unsigned char> all = rest;
	
	all.insert(all.end(), data.begin(), data.end());

	int i = 0;
	for (; i < all.size() / rate; ++i)
		p.push_back(std::vector<unsigned char>(all.begin() + i * rate, all.begin() + (i+1) * rate));
	
	int l = all.size() % rate;
	if (l)
		rest = std::vector<unsigned char>(all.end() - l, all.end());

	for(auto p_i : p)
		S = keccak(p_i);
}

std::string sha3::final(const std::vector<unsigned char>& data)
{
	do
	{
		update(data);
	} while (rest.size() > rate);

	rest.push_back(0x06);

	for (int i = rest.size(); i < rate; i++)
		rest.push_back(0);

	rest[rate - 1] |= 0x80;
	

	std::vector<unsigned char> z;
	std::vector<unsigned char> res = keccak(rest);

	z.insert(z.end(), res.begin(), res.begin() + rate);

	return bin2hex(std::vector<unsigned char>(z.begin(), z.begin() + (d >> 3)));
}

std::string sha3::compute(const std::vector<unsigned char>& data)
{
	return final(data);
}
