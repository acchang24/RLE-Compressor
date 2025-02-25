#include <iostream>
#include <vector>
#include "RleCompressor/RleData.h"
#include "RleCompressor/RleFile.h"

int main(int argc, const char* argv[])
{
	RleData data;
	data.Compress(std::vector<int8_t>{1, 2});

	RleFile file;
	file.CreateArchive("what");

	std::cout << "Compressor i guess\n";
	return 0;
}
