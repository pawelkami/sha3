#include "sha3.h"



void sha3::convertStringToStateArray(const std::vector<char>& str)
{
	for (unsigned x = 0; x < X_MAX; ++x)
		for (unsigned y = 0; y < Y_MAX; ++y)
			for (unsigned z = 0; z < W_MAX; ++z)
				A[x][y][z] = str[W_MAX * (5 * y + x) + z];
}

std::vector<char> sha3::convertStateArrayToString()
{
	std::vector<char> str;

	std::array<std::array<std::vector<char>, Y_MAX>, X_MAX> lane;
	for (unsigned i = 0; i < X_MAX; ++i)
		for (unsigned j = 0; j < Y_MAX; ++j)
			for (unsigned w = 0; w < W_MAX; ++w)
				lane[i][j].emplace_back(A[i][j][w]);

	std::array<std::vector<char>, Y_MAX> plane;
	for (unsigned j = 0; j < Y_MAX; ++j)
		for (unsigned i = 0; i < X_MAX; ++i)
			plane[j].insert(plane[j].end(), lane[i][j].begin(), lane[i][j].end());

	for (unsigned i = 0; i < X_MAX; ++i)
		str.insert(str.end(), plane[i].begin(), plane[i].end());

	return std::move(str);
}

void sha3::keccakPermutation()
{
	// TODO przekonwertowaæ S na A - prawdopodobnie w zewnêtrznej funkcji
	for (int i = 0; i < NUMBER_OF_ROUNDS; ++i)
	{
		rnd(i);
	}

	// TODO przekonwertowaæ A na S', prawdopodobnie w zewnêtrznej funkcji
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
//	A.reserve(5);
//	std::for_each(A.begin(), A.end(), [](std::vector<std::vector<uint8_t>> &a2Dim) -> void { a2Dim.reserve(Y_MAX); });
}


sha3::~sha3()
{
}

std::string sha3::compute(const std::vector<char>& data)
{
//	std::vector<char> ldata(data.begin(), data.begin() + 1600);
//	for (char it : ldata)
//		std::cout << it;
//
//	std::cout << std::endl;
//
//	convertStringToStateArray(ldata);
//	for (char it : convertStateArrayToString())
//		std::cout << it;

	return "abcde";
}
