

#include "utils.h"
#include <algorithm>
#include <map>
#include <fstream>

std::string bin2hex(const std::vector<unsigned char>& bin)
{
	std::string converted;
	std::string hexCodes = "0123456789abcdef";

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

	if (hex.size() == 1)	// one digit hex number
	{
		binVec.push_back(HEX2DIGIT(hex[0]));
		return binVec;
	}

	// an even hex string
	for (unsigned int i = 0; i < hex.size(); i += 2)
	{
		binVec.push_back(HEX2DIGIT(hex[i]) << 4 | HEX2DIGIT(hex[i+1]) & 0x0f);
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
	return std::vector<unsigned char>(fileContents.begin() + from, from + bytesToRead < fileContents.size() ? fileContents.begin() + from + bytesToRead : fileContents.end());
}

bool doFileExist(const std::string & filepath)
{
	std::ifstream f(filepath);
	return f.good();
}
