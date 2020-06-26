#include "CompressUtils.h"
#define ZLIB_INTERNAL
#include "zlib.h"
#include "zconf.h"
#include "zutil.h"

#pragma comment(lib, "zlibstat.lib")

std::string UE_CompressZlib(unsigned char *Contents, unsigned int NumBytes)
{
	std::string res;
	res.resize(compressBound(NumBytes)+1);
	unsigned long res_size = static_cast<unsigned long>(res.size());
	compress((Bytef*)&res[0], &res_size, Contents, NumBytes);
	res.resize(res_size);
	return res;
}