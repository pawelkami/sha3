#include "sha3_cryptopp.h"
#include <cryptopp/sha3.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>


sha3_cryptopp::sha3_cryptopp() : hash(224 / 8)
{
}

sha3_cryptopp::sha3_cryptopp(int size) : hash(size / 8)
{
}


sha3_cryptopp::~sha3_cryptopp()
{
}

std::string sha3_cryptopp::compute(const std::vector<char>& data)
{
	std::unique_ptr<byte> digest(new byte[hash.DigestSize()]);
	hash.CalculateDigest(digest.get(), (const unsigned char*)data.data(), data.size());

	CryptoPP::HexEncoder encoder;
	std::string output;
	encoder.Attach(new CryptoPP::StringSink(output));
	encoder.Put(digest.get(), sizeof(hash.DigestSize()));
	encoder.MessageEnd();
	return output;
}
