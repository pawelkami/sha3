#ifndef __HASHSTRATEGY__H__
#define __HASHSTRATEGY__H__

#include <vector>
#include <string>

class HashStrategy
{
public:
	HashStrategy();
	virtual ~HashStrategy();

	virtual void update(const std::vector<unsigned char>& data) = 0;
	virtual std::string final(const std::vector<unsigned char>& data) = 0;
	virtual std::string compute(const std::vector<unsigned char>& data) = 0;
};

#endif