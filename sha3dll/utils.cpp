

#include "utils.h"
#include <algorithm>
#include <map>
#include <fstream>

std::string bin2hex(const std::vector<unsigned char>& bin)
{
	std::string converted;
	std::string hexCodes = "0123456789ABCDEF";

	for (const auto& c : bin)
	{
		converted.push_back(hexCodes[c >> 4]);
		converted.push_back(hexCodes[c & 0x0f]);
	}


	return converted;
}

std::vector<unsigned char> hex2bin(std::string hex)
{
	std::vector<unsigned char> binVec;

	if(!checkIfHexString(hex))
		throw std::runtime_error("Given argument was not hex string");

	std::map<unsigned char, unsigned char> hex2binMap =
	{
		std::make_pair('0', 0x00),
		std::make_pair('1', 0x01),
		std::make_pair('2', 0x02),
		std::make_pair('3', 0x03),
		std::make_pair('4', 0x04),
		std::make_pair('5', 0x05),
		std::make_pair('6', 0x06),
		std::make_pair('7', 0x07),
		std::make_pair('8', 0x08),
		std::make_pair('9', 0x09),
		std::make_pair('a', 0x0a),
		std::make_pair('b', 0x0b),
		std::make_pair('c', 0x0c),
		std::make_pair('d', 0x0d),
		std::make_pair('e', 0x0e),
		std::make_pair('f', 0x0f),
		std::make_pair('A', 0x0a),
		std::make_pair('B', 0x0b),
		std::make_pair('C', 0x0c),
		std::make_pair('D', 0x0d),
		std::make_pair('E', 0x0e),
		std::make_pair('F', 0x0f),
	};

	for (int i = 0; i < hex.size(); i += 2)
	{
		unsigned char c = 0;
		c |= hex2binMap[hex[i]] << 4;
		c |= hex2binMap[hex[i + 1] & 0x0f];
		binVec.push_back(c);
	}

	return binVec;
}

bool checkIfHexString(const std::string & hex)
{
	return std::all_of(hex.begin(), hex.end(), 
		[](unsigned char c) 
		{
			return (c >= 'a' && c <= 'f' || c >= '0' && c <= '9' || c >= 'A' && c <= 'F');
		}
	);
}


std::vector<unsigned char> readFile(const std::string& filepath, unsigned from, unsigned bytesToRead)
{
	std::ifstream stream(filepath, std::ios::binary);
	if (!stream)
		throw std::runtime_error("Problem reading file " + filepath);

	std::vector<unsigned char> fileContents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	return fileContents;
}

bool doFileExist(const std::string & filepath)
{
	std::ifstream f(filepath);
	return f.good();
}
