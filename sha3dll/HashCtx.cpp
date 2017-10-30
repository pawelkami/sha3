#include "HashCtx.h"
#include "sha3.h"



HashCtx::HashCtx()
{
}


HashCtx::~HashCtx()
{
}

void HashCtx::clean()
{
	strategy.reset();
}

void HashCtx::setHashAlgo(HashType hashType)
{
	switch (hashType)
	{
		case HashCtx::SHA3_224: strategy = std::make_unique<sha3>(224); break;
		case HashCtx::SHA3_256: strategy = std::make_unique<sha3>(256); break;
		case HashCtx::SHA3_384: strategy = std::make_unique<sha3>(384); break;
		case HashCtx::SHA3_512: strategy = std::make_unique<sha3>(512); break;
		default: break;
	}
}

std::string HashCtx::computeHash(const std::string& filepath)
{
	if (strategy)
	{
		int i = 0;
		std::vector<unsigned char> data = readFile(filepath, i * strategy->rate, strategy->rate);
		while (data.size() == strategy->rate) // rate bits ; rate = 1600 - 2 * output_size
		{
			strategy->keccak_update(data);
			data = readFile(filepath, (++i) * strategy->rate, strategy->rate);
			std::cout << "DUPA" << std::endl;
		}
		return strategy->keccak_final(data);
	}
	else
		throw std::runtime_error("Hash algorithm wasn't chosen");
}
