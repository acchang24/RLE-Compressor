#include <iostream>
#include "RleCompressor/RleFile.h"

int main(int argc, const char* argv[])
{
	RleFile file;
	file.CreateArchive("data/Rumba Dancing.dae");
	file.CreateArchive("data/sponza.obj");

	return 0;
}
