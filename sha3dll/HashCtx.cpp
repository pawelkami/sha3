#include "HashCtx.h"
#include "sha3.h"

#define MAX_BUF_FILESIZE (1 << 20)	// 1MB

HashCtx::HashCtx()
{
}


HashCtx::~HashCtx()
{
}

void HashCtx::clean()
{
	strategy.reset();
}

void HashCtx::setHashAlgo(HashType hashType)
{
	switch (hashType)
	{
		case HashCtx::SHA3_224: strategy = std::make_unique<sha3>(224); break;
		case HashCtx::SHA3_256: strategy = std::make_unique<sha3>(256); break;
		case HashCtx::SHA3_384: strategy = std::make_unique<sha3>(384); break;
		case HashCtx::SHA3_512: strategy = std::make_unique<sha3>(512); break;
		default: break;
	}
}

std::string HashCtx::computeHash(const std::string& filepath)
{
	if (strategy)
	{
		int i = 0;
		long filesize = getFileSize(filepath);
		std::vector<unsigned char> data;
		while (i < filesize / MAX_BUF_FILESIZE)
		{
			data = readFile(filepath, (i++) * MAX_BUF_FILESIZE, MAX_BUF_FILESIZE);
			strategy->update(data);

		}// how many times we have to read
		
		data.clear();

		if (filesize % MAX_BUF_FILESIZE)	// rest of bytes
		{
			data = readFile(filepath, i * MAX_BUF_FILESIZE, MAX_BUF_FILESIZE);
		}

		std::string result = strategy->final(data);
		
		LOG("Computed digest of " + filepath + " is " + result);

		return result;
	}
	else
		throw std::runtime_error("Hash algorithm wasn't chosen");
}
