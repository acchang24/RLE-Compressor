#pragma once
#include <vector>

// Run-length encoding compression and decompression
struct RleData
{
	// Vector to store compressed or decompressed data
	std::vector<int8_t> mData;

	// Compresses input data and store them into mData
	// @param - const std::vector<int8_t>& for the data to compress
	void Compress(const std::vector<int8_t>& inputData);

	// Decompresses input data and stores them into mData
	// @param - const std::vector<int8_t>& for the data to decompress
	// @param - the output size of the file
	void Decompress(const std::vector<int8_t>& inputData, size_t outputSize);
};
