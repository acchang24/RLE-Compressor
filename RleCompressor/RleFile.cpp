#include "RleFile.h"
#include <iostream>
#include <fstream>

void RleFile::CreateArchive(const std::string& fileName)
{
	// Open the file in binary mode and load data into array
	std::ifstream::pos_type size;

	// char array to store data
	char* memblock = nullptr;

	// Array of the data used to compress
	std::vector<int8_t> data;

	// Open the file for input, in binary mode, and start ATE (at the end)
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		// Save the size of the file
		size = file.tellg();
		unsigned int arraySize = static_cast<unsigned int>(size);
		// Initialize the memblock array with the size
		memblock = new char[arraySize];
		// Seek back to the start of the file
		file.seekg(0, std::ios::beg);
		// Read the data into memblock
		file.read(memblock, size);
		file.close();

		// Fill in header data
		mHeader.mFileName = fileName;
		mHeader.mFileNameLength = static_cast<unsigned char>(fileName.length());
		mHeader.mFileSize = static_cast<int>(size);

		// Copy memblock into a new vector to compress
		data.reserve(static_cast<size_t>(mHeader.mFileSize));
		for (unsigned int i = 0; i < arraySize; ++i)
		{
			data.emplace_back(memblock[i]);
		}

		mData.Compress(data);

		// Free memblock data
		delete[] memblock;
	}

	// Open the file for output, in binary mode, and overwrite an existing file
	std::ofstream arc(fileName + ".rle", std::ios::out | std::ios::binary | std::ios::trunc);

	if (arc.is_open())
	{
		// Write the header
		// 4 bytes for file type
		arc.write(mHeader.mSig, 4);
		// Original size of the source file
		arc.write(reinterpret_cast<char*>(&(mHeader.mFileSize)), 4); // index 4
		// One byte for number of letters in the file name
		arc.write(reinterpret_cast<const char*>(&mHeader.mFileNameLength), 1); // index 8
		// Name of the file, use length of file name for the byte size
		arc.write(mHeader.mFileName.c_str(), static_cast<int>(mHeader.mFileNameLength)); // index 9

		// Write each byte in the mData vector
		for (int i = 0; i < mData.mData.size(); i++)
		{
			arc.write(reinterpret_cast<char*>(&(mData.mData[i])), 1);
		}
		arc.close();
	}
}

void RleFile::ExtractArchive(const std::string& fileName)
{
	std::cout << fileName << "\n";
}
