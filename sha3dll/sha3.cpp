#include "sha3.h"

const uint64_t sha3::keccak_round_constants[sha3::NUMBER_OF_ROUNDS] = {
	uint64_t(0x0000000000000001), uint64_t(0x0000000000008082), uint64_t(0x800000000000808A), uint64_t(0x8000000080008000),
	uint64_t(0x000000000000808B), uint64_t(0x0000000080000001), uint64_t(0x8000000080008081), uint64_t(0x8000000000008009),
	uint64_t(0x000000000000008A), uint64_t(0x0000000000000088), uint64_t(0x0000000080008009), uint64_t(0x000000008000000A),
	uint64_t(0x000000008000808B), uint64_t(0x800000000000008B), uint64_t(0x8000000000008089), uint64_t(0x8000000000008003),
	uint64_t(0x8000000000008002), uint64_t(0x8000000000000080), uint64_t(0x000000000000800A), uint64_t(0x800000008000000A),
	uint64_t(0x8000000080008081), uint64_t(0x8000000000008080), uint64_t(0x0000000080000001), uint64_t(0x8000000080008008)
};

std::vector<sha3::bit> sha3:: xor(const std::vector<bit>& f, const std::vector<bit>& s)
{
	std::vector<bit> xored;
	for (int i = 0; i < f.size(); ++i)
		xored.push_back(bit(f[i] ^ s[i]));
	return std::move(xored);
}

std::vector<sha3::bit> sha3::convertStringToBits(const std::vector<unsigned char>& str)
{
	std::vector<bit> bits;

	for (unsigned char c : str)
	{
		bits.push_back(bit(((int)c & 0x80) == 0 ? 0 : 1));
		bits.push_back(bit(((int)c & 0x40) == 0 ? 0 : 1));
		bits.push_back(bit(((int)c & 0x20) == 0 ? 0 : 1));
		bits.push_back(bit(((int)c & 0x10) == 0 ? 0 : 1));
		bits.push_back(bit(((int)c & 0x08) == 0 ? 0 : 1));
		bits.push_back(bit(((int)c & 0x04) == 0 ? 0 : 1));
		bits.push_back(bit(((int)c & 0x02) == 0 ? 0 : 1));
		bits.push_back(bit(((int)c & 0x01) == 0 ? 0 : 1));
	}

	for (bit b : bits)
		std::cout << b;

	return std::move(bits);
}

std::vector<unsigned char> sha3::convertBitsToString(const std::vector<bit>& bits)
{
	std::vector<unsigned char> str;
	
	for (int i = 0; i < bits.size(); i += 8)
		str.push_back(unsigned char((bits[i] << 7) + (bits[i+1] << 6) + (bits[i+2] << 5) + (bits[i+3] << 4) + (bits[i+4] << 3) + (bits[i+5] << 2) + (bits[i+6] << 1) + bits[i+7]));

	return std::move(str);
}

void sha3::convertStringToStateArray(const std::vector<bit>& str)
{
	for (unsigned x = 0; x < X_MAX; ++x)
		for (unsigned y = 0; y < Y_MAX; ++y)
			for (unsigned z = 0; z < W_MAX; ++z)
				A[x][y][z] = str[W_MAX * (5 * y + x) + z];

	//unsigned i = 0;
	//for (unsigned x = 0; x < X_MAX; ++x)
	//	for (unsigned y = 0; y < Y_MAX; ++y)
	//		for (unsigned z = 0; z < Z_MAX; ++z)
	//			A[y][x][z] = str[i++];
}

std::vector<sha3::bit> sha3::convertStateArrayToString()
{
	std::vector<bit> str;

	std::array<std::array<std::vector<bit>, Y_MAX>, X_MAX> lane;
	for (unsigned i = 0; i < X_MAX; ++i)
		for (unsigned j = 0; j < Y_MAX; ++j)
			for (unsigned w = 0; w < W_MAX; ++w)
				lane[j][i].emplace_back(A[j][i][w]);

	std::array<std::vector<bit>, Y_MAX> plane;
	for (unsigned j = 0; j < Y_MAX; ++j)
		for (unsigned i = 0; i < X_MAX; ++i)
			plane[j].insert(plane[j].end(), lane[i][j].begin(), lane[i][j].end());

	for (unsigned i = 0; i < X_MAX; ++i)
		str.insert(str.end(), plane[i].begin(), plane[i].end());

	//unsigned i = 0;
	//for (unsigned x = 0; x < X_MAX; ++x)
	//	for (unsigned y = 0; y < Y_MAX; ++y)
	//		for (unsigned z = 0; z < Z_MAX; ++z)
	//			str.emplace_back(A[y][x][z]);

	return std::move(str);
}

void sha3::pad10_1(unsigned x, unsigned m, std::vector<bit>& p)
{
	int j = m;
	j = (-j - 2) % x;

	p.push_back(bit::ONE);
	for(int i = 0; i < j; ++i)
		p.push_back(bit::ZERO);
	p.push_back(bit::ONE);
}

std::vector<sha3::bit> sha3::keccakPermutation(std::vector<bit>& m)
{
	convertStringToStateArray(m);
	
	for (int i = 0; i < NUMBER_OF_ROUNDS; ++i)
	{
		rnd(i);
	}

	return convertStateArrayToString();
}

void sha3::keccakTheta()
{
	//state_array A_prim;
	//std::array<std::array<uint8_t, X_MAX>, Z_MAX> C, D;

	//for (unsigned x = 0; x < X_MAX; ++x)
	//{
	//	for (unsigned z = 0; z < W_MAX; ++z)
	//	{
	//		unsigned mask = (1 << (z % BITS_IN_CHAR));
	//		//std::cout << "mask: " << mask << std::endl;
	//		unsigned az = (unsigned)(z / BITS_IN_CHAR) + 1;
	//		C[x][z] = (A[x][0][az] & mask);
	//		for (unsigned y = 1; y < Y_MAX; ++y)
	//			C[x][z] ^= (A[x][y][az] & mask);
	//	}
	//}

	//for (unsigned x = 0; x < X_MAX; ++x)
	//	for (unsigned z = 0; z < W_MAX; ++z)
	//		D[x][z] = C[(x - 1) % 5][z] ^ C[(x + 1) % 5][(z - 1) % W_MAX];
	

	//for (unsigned x = 0; x < X_MAX; ++x)
	//{
	//	for (unsigned y = 0; y < Y_MAX; ++y)
	//	{
	//		for (unsigned z = 0; z < W_MAX; ++z)
	//		{
	//			w = ;
	//			A_prim[x][y][w] = (A[x][y][w] & )
	//		}
	//	}
	//}
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
	unsigned r = keccak_round_constants[round];
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

std::vector<sha3::bit> sha3::sponge(std::vector<bit>& m)
{
	std::vector<bit> p(m.begin(), m.end());
	pad10_1(r, m.size(), p);
	unsigned n = p.size() / r;

	std::vector<std::vector<bit>> pn;
	for (int i = 0; i < n; ++i)
		pn.push_back(std::vector<bit>(p.begin() + (i * r), p.begin() + ((i+1) * r)));

	std::vector<bit> S(B, bit::ZERO);

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < c; ++j)
			pn[i].push_back(bit::ZERO);

		S = keccakPermutation(xor(S, pn[i]));
	}

	std::vector<bit> Z(S.begin(), S.begin() + r);

	while (Z.size() < d) 
	{
		S = keccakPermutation(S);
		for (int i = 0; i < r; ++i)
			Z.push_back(S[i]);
	}

	return std::move(std::vector<bit>(Z.begin(), Z.begin() + d));
}

std::vector<unsigned char> sha3::keccak(std::vector<bit>& m)
{
	return convertBitsToString(sponge(m));
}

sha3::sha3()
{
}

sha3::sha3(int size)
{
	d = size;
	c = 2 * size; 
	r = W_MAX * Y_MAX * X_MAX - c; // r = b - c
}


sha3::~sha3()
{
}

std::string sha3::compute(const std::vector<unsigned char>& data)
{
	//std::vector<unsigned char> ldata(data.begin(), data.begin() + 1600);
	//for (unsigned char it : ldata)
	//	std::cout << it;

	//std::cout << std::endl << std::string(40, '=') << std::endl;

	//convertStringToStateArray(ldata);
	//for (unsigned char it : convertStateArrayToString())
	//	std::cout << it;
	
	//std::vector<unsigned char> ldata(data.begin(), data.begin() + 200);
	//for (unsigned char it : ldata)
	//	std::cout << it;
	//std::cout << std::endl << std::string(40, '=') << std::endl;
	//
	//std::vector<bit> m = convertStringToBits(ldata);
	//
	//std::cout << std::endl << std::string(40, '=') << std::endl;
	//for (auto b : m)
	//	std::cout << b;
	//std::cout << std::endl << std::string(40, '=') << std::endl;

	//convertStringToStateArray(m);
	//m = convertStateArrayToString();
	//std::vector<unsigned char> out = convertBitsToString(m);

	//std::cout << std::endl << std::string(40, '=') << std::endl;

	//for (unsigned char it : out)
	//	std::cout << it;

	std::vector<bit> m = convertStringToBits(data);
	m.push_back(bit::ZERO);
	m.push_back(bit::ONE);
	
	//return bin2hex(keccak(m));

	return "abcd";
}
