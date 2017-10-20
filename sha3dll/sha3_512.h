#ifndef __SHA3_512_H__
#define __SHA3_512_H__

#include "HashStrategy.h"
class sha3_512 :
	public HashStrategy
{
public:
	sha3_512();
	~sha3_512();

	std::string compute(const std::vector<char>& data);
};

#endif