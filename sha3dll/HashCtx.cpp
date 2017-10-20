#include "HashCtx.h"
#include "sha3_224.h"
#include "sha3_256.h"
#include "sha3_384.h"
#include "sha3_512.h"



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
		case HashCtx::SHA3_224: strategy = std::make_unique<sha3_224>(); break;
		case HashCtx::SHA3_256: strategy = std::make_unique<sha3_256>(); break;
		case HashCtx::SHA3_384: strategy = std::make_unique<sha3_384>(); break;
		case HashCtx::SHA3_512: strategy = std::make_unique<sha3_512>(); break;
		default: break;
	}
}

std::string HashCtx::computeHash(std::vector<char> data)
{
	return strategy->compute(data);
}
