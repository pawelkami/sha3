#ifndef __SHA3_256__H__
#define __SHA3_256__H__

#include "HashStrategy.h"
class sha3_256 :
	public HashStrategy
{
public:
	sha3_256();
	~sha3_256();

	std::string compute(const std::vector<char>& data);
};

#endif