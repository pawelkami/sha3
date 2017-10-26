#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <vector>


std::string bin2hex(const std::vector<char>& bin);

std::vector<char> hex2bin(std::string hex);

bool checkIfHexString(const std::string& hex);

std::vector<char> readFile(const std::string& filepath);

bool doFileExist(const std::string& filepath);


#endif