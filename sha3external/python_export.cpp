

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <vector>
#include "HashCtx.h"

BOOST_PYTHON_MODULE(sha3cryptopp)
{
	using namespace boost::python;

	enum_<HashCtx::HashType>("HashType")
		.value("SHA3_224", HashCtx::SHA3_224)
		.value("SHA3_256", HashCtx::SHA3_256)
		.value("SHA3_384", HashCtx::SHA3_384)
		.value("SHA3_512", HashCtx::SHA3_512)
		;

	class_<HashCtx, boost::noncopyable>("HashCtx")
		.def("setHashAlgo", &HashCtx::setHashAlgo, args("hashAlgo"))
		.def("computeHash", &HashCtx::computeHash, args("filepath"))
		.def("clean", &HashCtx::clean)
		;

}