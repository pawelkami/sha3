#ifndef __SHA3_H__
#define __SHA3_H__

#include "HashStrategy.h"

class sha3 :
	public HashStrategy
{
public:
	sha3();
	sha3(int size);
	~sha3();

	std::string compute(const std::vector<char>& data);
};

#endif