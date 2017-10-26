#include "HashCtx.h"
#include "utils.h"
#include "sha3.h"



HashCtx::HashCtx()
{
}


HashCtx::~HashCtx()
{
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
	std::vector<char> data = readFile(filepath);
	if (strategy)
		return strategy->compute(data);
	else
		throw std::runtime_error("Hash algorithm wasn't chosen");
}
