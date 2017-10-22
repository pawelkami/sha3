#ifndef __HASHCTX__H__
#define __HASHCTX__H__

#include "HashStrategy.h"
#include <memory>
#include <vector>

class HashCtx
{
private:
	std::unique_ptr<HashStrategy> strategy;

public:
	enum HashType
	{
		SHA3_224 = 0,
		SHA3_256 = 1,
		SHA3_384 = 2,
		SHA3_512 = 3,
	};

	HashCtx();
	~HashCtx();

	void setHashAlgo(HashType hashType);
	std::string computeHash(const std::string& filepath);
	
};


#endif