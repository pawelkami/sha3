#ifndef __HASHSTRATEGY__H__
#define __HASHSTRATEGY__H__

#include <vector>
#include <string>

class HashStrategy
{
public:
	unsigned rate;

	HashStrategy();
	virtual ~HashStrategy();

	virtual void keccak_update(const std::vector<unsigned char>& data) = 0;
	virtual std::string keccak_final(const std::vector<unsigned char>& data) = 0;
};

#endif