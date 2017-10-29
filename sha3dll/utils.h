#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <vector>


std::string bin2hex(const std::vector<unsigned char>& bin);

std::vector<unsigned char> hex2bin(std::string hex);

bool checkIfHexString(const std::string& hex);

std::vector<unsigned char> readFile(const std::string& filepath, unsigned from, unsigned bytesToRead);

bool doFileExist(const std::string& filepath);


#endif