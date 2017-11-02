#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <vector>
#include <fstream>

#define HEX2DIGIT(c) ((c) <= '9' ? (c) & 0xF : ((c) - 'a' + 10) & 0xF)

std::string bin2hex(const std::vector<unsigned char>& bin);

std::vector<unsigned char> hex2bin(std::string hex);

bool checkIfHexString(const std::string& hex);

std::vector<unsigned char> readFile(const std::string& filepath);

std::vector<unsigned char> readFile(const std::string& filepath, unsigned from, unsigned bytesToRead);

long getFileSize(const std::string& filename);

bool doFileExist(const std::string& filepath);


#endif