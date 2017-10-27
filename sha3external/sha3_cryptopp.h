#ifndef __SHA3_CRYPTOPP_H__
#define __SHA3_CRYPTOPP_H__

#include <cryptopp/sha3.h>

#include "HashStrategy.h"
class sha3_cryptopp :
	public HashStrategy
{
private:
	CryptoPP::SHA3 hash;

public:
	sha3_cryptopp();
	sha3_cryptopp(int size);
	~sha3_cryptopp();

	std::string compute(const std::vector<char>& data);

};

#endif