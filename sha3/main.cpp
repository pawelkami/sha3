// sha3.cpp: Definiuje punkt wejścia dla aplikacji konsolowej.
//

#include "HashCtx.h"
#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include "utils.h"

using namespace boost::program_options;

int main(int argc, char** argv)
{
	try
	{
		options_description desc{ "Compute SHA3 hash of file" };
		desc.add_options()
			("help,h", "Help screen")
			("224", value<std::string>()->value_name("file"), "Program computes SHA3-224 of file")
			("256", value<std::string>()->value_name("file"), "Program computes SHA3-256 of file")
			("384", value<std::string>()->value_name("file"), "Program computes SHA3-384 of file")
			("512", value<std::string>()->value_name("file"), "Program computes SHA3-512 of file");

		variables_map vm;
		store(parse_command_line(argc, argv, desc), vm);
		notify(vm);

		HashCtx hashCtx;
		std::string filepath;

		if (vm.count("help"))
			std::cout << desc << '\n';
		else if (vm.count("224"))
		{
			hashCtx.setHashAlgo(HashCtx::SHA3_224);
			filepath = vm["224"].as<std::string>();
		}
		else if (vm.count("256"))
		{
			hashCtx.setHashAlgo(HashCtx::SHA3_256);
			filepath = vm["256"].as<std::string>();
		}
		else if (vm.count("384"))
		{
			hashCtx.setHashAlgo(HashCtx::SHA3_384);
			filepath = vm["384"].as<std::string>();
		}
		else if (vm.count("512"))
		{
			hashCtx.setHashAlgo(HashCtx::SHA3_512);
			filepath = vm["512"].as<std::string>();
		}
		else
			throw error("No algo chosen");

		if (!doFileExist(filepath))
			throw std::runtime_error("File path is invalid");

		std::cout << hashCtx.computeHash(filepath) << std::endl;
	}
	catch (const error &ex)
	{
		std::cerr << ex.what() << '\n';
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 2;
	}
	
    return 0;
}

