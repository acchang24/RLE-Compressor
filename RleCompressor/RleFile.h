#pragma once
#include <string>
#include "RleData.h"

class RleFile
{
public:
	struct Header
	{
		
	};

	// Creates a file archive by taking in a file and compressesing it,
	// outputing the newly compressed file
	void CreateArchive(const std::string& fileName);

	// Extracts a file archive by taking in a file and decompressing it,
	// outputing the newly extracted file
	void ExtractArchive(const std::string& fileName);

private:
	// Data for RLE
	RleData mData;

	// Header for the file
	Header mHeader;
};
