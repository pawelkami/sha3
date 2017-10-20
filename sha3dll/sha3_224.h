#ifndef __SHA3_224__H__
#define __SHA3_224__H__


#include "HashStrategy.h"

class sha3_224 :
	public HashStrategy
{
public:
	sha3_224();
	~sha3_224();
	
	std::string compute(const std::vector<char>& data);

};

#endif