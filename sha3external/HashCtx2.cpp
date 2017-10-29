#include "HashCtx.h"
#include "utils.h"
#include "cryptopp/sha3.h"
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include "sha3_cryptopp.h"


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
	case HashCtx::SHA3_224: strategy = std::make_unique<sha3_cryptopp>(224); break;
	case HashCtx::SHA3_256: strategy = std::make_unique<sha3_cryptopp>(256); break;
	case HashCtx::SHA3_384: strategy = std::make_unique<sha3_cryptopp>(384); break;
	case HashCtx::SHA3_512: strategy = std::make_unique<sha3_cryptopp>(512); break;
	default: break;
	}
}

std::string HashCtx::computeHash(const std::string& filepath)
{
	std::vector<unsigned char> data = readFile(filepath);
	if (strategy)
		return strategy->compute(data);
	else
		throw std::runtime_error("Hash algorithm wasn't chosen");
}
