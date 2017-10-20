#ifndef __HASHSTRATEGY__H__
#define __HASHSTRATEGY__H__

#include <vector>
#include <string>

class HashStrategy
{
public:
	HashStrategy();
	virtual ~HashStrategy();

	virtual std::string compute(const std::vector<char>& data) = 0;
};

#endif