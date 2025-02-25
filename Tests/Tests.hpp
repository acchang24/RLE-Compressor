#include <iostream>
#include "RleData.h"
#include "RleFile.h"

static void RunCompressionTests()
{
	std::cout << "Run compression tests\n";

	RleData data;
	data.Compress(std::vector<int8_t>{1, 2});

	RleFile file;
	file.CreateArchive("what");
}