#ifndef __SHA3_384_H__
#define __SHA3_384_H__

#include "HashStrategy.h"

class sha3_384 :
	public HashStrategy
{
public:
	sha3_384();
	~sha3_384();

	std::string compute(const std::vector<char>& data);
};


#endif