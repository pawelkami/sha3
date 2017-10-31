#include "sha3.h"

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
		bits.push_back(bit((c & 0x80) == 0 ? 0 : 1));
		bits.push_back(bit((c & 0x40) == 0 ? 0 : 1));
		bits.push_back(bit((c & 0x20) == 0 ? 0 : 1));
		bits.push_back(bit((c & 0x10) == 0 ? 0 : 1));
		bits.push_back(bit((c & 0x08) == 0 ? 0 : 1));
		bits.push_back(bit((c & 0x04) == 0 ? 0 : 1));
		bits.push_back(bit((c & 0x02) == 0 ? 0 : 1));
		bits.push_back(bit((c & 0x01) == 0 ? 0 : 1));
	}

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
}

std::vector<sha3::bit> sha3::convertStateArrayToString()
{
	std::vector<bit> str;

	std::array<std::array<std::vector<bit>, Y_MAX>, X_MAX> lane;
	for (unsigned i = 0; i < X_MAX; ++i)
		for (unsigned j = 0; j < Y_MAX; ++j)
			for (unsigned w = 0; w < W_MAX; ++w)
				lane[i][j].emplace_back(A[i][j][w]);

	std::array<std::vector<bit>, Y_MAX> plane;
	for (unsigned j = 0; j < Y_MAX; ++j)
		for (unsigned i = 0; i < X_MAX; ++i)
			plane[j].insert(plane[j].end(), lane[i][j].begin(), lane[i][j].end());

	for (unsigned i = 0; i < X_MAX; ++i)
		str.insert(str.end(), plane[i].begin(), plane[i].end());

	return std::move(str);
}

void sha3::pad10_1(unsigned x, unsigned m, std::vector<bit>& p)
{
	int j = m;
	if (abs(-j - 2) % x == 0)
		j = 0;
	else
		j = x - ((m + 2) % x);
	//std::cout << -(int)m << " - 2 " << " mod " << x << " = " << j << std::endl;

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

	return std::move(convertStateArrayToString());
}

void sha3::keccakTheta()
{
	state_array A_prim = A;

	std::array<std::array<bit, W_MAX>, X_MAX> C, D;


	for (unsigned x = 0; x < X_MAX; ++x)
		for (unsigned z = 0; z < W_MAX; ++z)
			C[x][z] = bit(A[x][0][z] ^ A[x][1][z] ^ A[x][2][z] ^ A[x][3][z] ^ A[x][4][z]);

	for (unsigned x = 0; x < X_MAX; ++x)
		for (unsigned z = 0; z < W_MAX; ++z)
			D[x][z] = bit(C[(x-1) % X_MAX][z] ^ C[(x+1) % X_MAX][(z-1) % W_MAX]);

	for (unsigned x = 0; x < X_MAX; ++x)
		for (unsigned y = 0; y < Y_MAX; ++y)
			for (unsigned z = 0; z < W_MAX; ++z)
				A_prim[x][y][z] = bit(A[x][y][z] ^ D[x][z]);
	
	A = A_prim;
}

void sha3::keccakRho()
{
	state_array A_prim = A;

	for (unsigned z = 0; z < W_MAX; ++z)
		A_prim[0][0][z] = A[0][0][z];

	unsigned x = 1, y = 0;

	for (unsigned t = 0; t < NUMBER_OF_ROUNDS; ++t)
	{
		for (unsigned z = 0; z < W_MAX; ++z)
		{
			int z_prim = (z - ((t + 1)*(t + 2) >> 1));
			if (z_prim < 0)
			{
				if((abs(z_prim) % W_MAX) == 0)
					z_prim = 0;
				else
					z_prim = W_MAX - (abs(z_prim) % W_MAX);
			}
			else
				z_prim = z_prim % W_MAX;

			A_prim[x][y][z] = A[x][y][z_prim];
		}
		unsigned old_x = x;
		x = y;
		y = (2 * old_x + 3 * y) % Y_MAX;
	}

	A = A_prim;
}

void sha3::keccakPi()
{
	state_array A_prim = A;

	for (unsigned x = 0; x < X_MAX; ++x)
		for (unsigned y = 0; y < Y_MAX; ++y)
			for (unsigned z = 0; z < W_MAX; ++z)
				A_prim[x][y][z] = A[(x+3*y) % X_MAX][x][z];

	A = A_prim;
}

void sha3::keccakChi()
{
	state_array A_prim = A;

	for (unsigned x = 0; x < X_MAX; ++x)
		for (unsigned y = 0; y < Y_MAX; ++y)
			for (unsigned z = 0; z < W_MAX; ++z)
				A_prim[x][y][z] = bit(A[x][y][z] ^ bit(bit(A[(x+1) % X_MAX][y][z] ^ 1) & A[(x+2) % X_MAX][y][z]));

	A = A_prim;
}

sha3::bit sha3::rc(unsigned t)
{
	if (t % 255 == 0)
		return sha3::bit::ONE;
	else 
	{
		std::vector<bit> R = { bit(1), bit(0), bit(0), bit(0), bit(0), bit(0), bit(0), bit(0) };

		for (unsigned i = 1; i <= t % 255; ++i)
		{
			R.insert(R.begin(), bit::ZERO);
			R[0] = bit(R[0] ^ R[8]);
			R[4] = bit(R[4] ^ R[8]);
			R[5] = bit(R[5] ^ R[8]);
			R[6] = bit(R[6] ^ R[8]);
			R = std::vector<bit>(R.begin(), R.begin() + 8);
		}

		return R[0];
	}
}

void sha3::keccakJota(unsigned int round)
{
	state_array A_prim;

	A_prim = A;

	std::array<bit, W_MAX> RC;
	//std::for_each(RC.begin(), RC.end(), [](bit it) { it = bit::ZERO; });
	for (unsigned i = 0; i < RC.size(); ++i)
		RC[i] = bit::ZERO;

	RC[0] = rc(7 * round);
	for (unsigned j = 1; j <= (NUMBER_OF_ROUNDS - 12) >> 1; ++j)
		RC[(2 << (j-1)) - 1] = rc(j + 7 * round);
	for (unsigned z = 0; z < W_MAX; ++z)
		A_prim[0][0][z] = bit(A_prim[0][0][z] ^ RC[z]);

	A = A_prim;
}

void sha3::rnd(unsigned int round)
{
	keccakTheta();
	keccakRho();
	keccakPi();
	keccakChi();
	keccakJota(round);
}

std::vector<sha3::bit> sha3::sponge(std::vector<bit>& m, bool isFinal)
{
	//convertStringToStateArray(m);
	//return convertStateArrayToString();

	if (isFinal)
	{
		pad10_1(r, (unsigned)m.size(), m);
	}

	for (unsigned j = 0; j < c; ++j)
		m.push_back(bit::ZERO);
	S = keccakPermutation(xor(S, m));

	if (isFinal)
	{
		std::vector<bit> Z(S.begin(), S.begin() + r);
		while (Z.size() < d)
		{
			S = keccakPermutation(S);
			Z.insert(Z.begin(), S.begin(), S.begin() + r);
		}
		return std::move(std::vector<bit>(Z.begin(), Z.begin() + d));
	}
	else
		return std::move(std::vector<bit>());
}

std::vector<unsigned char> sha3::keccak(std::vector<bit>& m, bool isFinal)
{
	return convertBitsToString(sponge(m, isFinal));
}

sha3::sha3()
{
}

sha3::sha3(int size) : S(B, bit::ZERO)
{
	d = size;
	c = 2 * size; 
	r = W_MAX * Y_MAX * X_MAX - c; // r = b - c
	rate = r >> 3; // r in bytes
}


sha3::~sha3()
{
}

void sha3::keccak_update(const std::vector<unsigned char>& data)
{
	//std::cout << "update";
	std::vector<bit> m = convertStringToBits(data);
	keccak(m, false);
}

std::string sha3::keccak_final(const std::vector<unsigned char>& data)
{
	//std::cout << "final";
	std::vector<bit> m = convertStringToBits(data);
	m.push_back(bit::ZERO);
	m.push_back(bit::ONE);

	return bin2hex(keccak(m, true));

	//auto k_res = keccak(m, true);
	//std::string hash;

	//for (auto ch : k_res)
	//	hash += static_cast<char>(ch);

	//return hash;
}

