#define CATCH_CONFIG_MAIN
#include <iostream>
#include <vector>
#include "md5.h"
#include "catch.hpp"
#include "RleData.h"
#include "RleFile.h"

// Checks to see if two memory buffers hold the same data after compression or decompression
// @param - const std::vector<int8_t>& for the data results after compression or decompression
// @param - const std::vector<int8_t>& for the expected output of memory
// @return - true if the results matches the expected output
static bool SameBuffers(const std::vector<int8_t>& results, const std::vector<int8_t>& expected)
{
	// Check to see if the sizes are different
	if (results.size() != expected.size())
	{
		FAIL("Results size is " << results.size() << " but the expected size is " << expected.size());
		return false;
	}

	// Loop through the expected buffer and check to see if all values are the same as results
	for (size_t i = 0; i < expected.size(); ++i)
	{
		if (results[i] != expected[i])
		{
			FAIL("Results at index: " << i << " is " << static_cast<int>(results[i]) << " but expected " << static_cast<int>(expected[i]));
			return false;
		}
	}

	return true;
}

// Uses MD5 to check if a compressed/decompressed file's hash matches a prehashed expected result
// @param - const std::string& for the file name to test
// @param - const std::string& for the expected string hash
static bool CheckFileMD5(const std::string& fileName, const std::string& expected)
{
	MD5 md5;
	std::string hash = md5.digestFile(fileName.c_str());
	return hash == expected;
}

// Runs a test whenever data gets compressed
// @param - const std::vector<int8_t>& for the input data
// @param - const std::vector<int8_t>& for the expected data
static bool RunCompressionTest(const std::vector<int8_t>& input, const std::vector<int8_t>& expected)
{
	RleData data;
	data.Compress(input);
	return SameBuffers(data.mData, expected);
}

// Runs a test whenever data gets decompressed
// @param - const std::vector<int8_t>& for the input data
// @param - const std::vector<int8_t>& for the expected data
static bool RunDecompressionTest(const std::vector<int8_t>& input, const std::vector<int8_t>& expected)
{
	RleData data;
	data.Decompress(input, expected.size());
	return SameBuffers(data.mData, expected);
}

// TEST USING CATCH
TEST_CASE("RLE Compression", "[compression]")
{
	SECTION("Basic positive run")
	{
		std::vector<int8_t> test = {
			'a','a','a','b','b','b','c','c','c',42,42,42,
			'a','a','a','b','b','b','c','c','c',42,42,42,
			'a','a','a','b','b','b','c','c','c',42,42,42,
			'a','a','a','b','b','b','c','c','c',42,42,42,
			'a','a','a','b','b','b','c','c','c',42,42,42,
			'a','a','a','b','b','b','c','c','c',42,42,42,
			'a','a','a','b','b','b','c','c','c',42,42,42,
			'a','a','a','b','b','b','c','c','c',42,42,42,
		};
		std::vector<int8_t> expected = {
			3,'a',3,'b',3,'c',3,42,
			3,'a',3,'b',3,'c',3,42,
			3,'a',3,'b',3,'c',3,42,
			3,'a',3,'b',3,'c',3,42,
			3,'a',3,'b',3,'c',3,42,
			3,'a',3,'b',3,'c',3,42,
			3,'a',3,'b',3,'c',3,42,
			3,'a',3,'b',3,'c',3,42,
		};

		bool result = RunCompressionTest(test, expected);
		REQUIRE(result);
	}

	SECTION("Basic negative run")
	{
		std::vector<int8_t> test = {
			'a','a','a','b','c',42,
			'a','a','a','a','a','a',
			'a','b','c',42,'a','a','a','a',
		};
		std::vector<int8_t> expected = {
			3,'a',-3,'b','c',42,7,'a',
			-3,'b','c',42,4,'a',
		};

		bool result = RunCompressionTest(test, expected);
		REQUIRE(result);
	}

	SECTION("+/- mix")
	{
		std::vector<int8_t> test = { 'a','b','a','a','b' };
		std::vector<int8_t> expected = { -2,'a','b',2,'a',1,'b' };

		bool result = RunCompressionTest(test, expected);

		REQUIRE(result);
	}

	SECTION("128 repeat")
	{
		std::vector<int8_t> test = {
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','b',
		};
		std::vector<int8_t> expected = {
			127,'a',-2,'a','b',
		};

		bool result = RunCompressionTest(test, expected);
		REQUIRE(result);
	}

	SECTION("Long repeated run over 127")
	{
		std::vector<int8_t> test = {
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
		};
		std::vector<int8_t> expected = {
			127,'a',127,'a',88,'a',
		};

		bool result = RunCompressionTest(test, expected);
		REQUIRE(result);
	}

	SECTION("Negative Run over 127")
	{
		std::vector<int8_t> test = {
			'b','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b',
		};
		std::vector<int8_t> expected = {
			2,'b',-127,'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b',
			'a','b','a',1,'b',
		};

		bool result = RunCompressionTest(test, expected);
		REQUIRE(result);
	}

	SECTION("Run over 127 +/- mix, start with repeating characters")
	{
		std::vector<int8_t> test = {
			'c','c','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','b','c',42,
		};
		std::vector<int8_t> expected = {
			2,'c',127,'a',2,'a',-3,'b','c',42
		};

		bool result = RunCompressionTest(test, expected);
		REQUIRE(result);
	}

	SECTION("Run over 127 +/- mix, start with two unique characters")
	{
		std::vector<int8_t> test = {
			'b','c','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','b','c',42,
		};
		std::vector<int8_t> expected = {
			-2,'b','c',127,'a',2,'a',-3,'b','c',42
		};

		bool result = RunCompressionTest(test, expected);
		REQUIRE(result);
	}
}

TEST_CASE("RLE Decompression", "[decompression]")
{
	SECTION("Basic positive run")
	{
		std::vector<int8_t> test = { 40,'x' };
		std::vector<int8_t> expected = {
			'x','x','x','x','x','x','x','x','x','x',
			'x','x','x','x','x','x','x','x','x','x',
			'x','x','x','x','x','x','x','x','x','x',
			'x','x','x','x','x','x','x','x','x','x',
		};

		bool result = RunDecompressionTest(test, expected);
		REQUIRE(result);
	}

	SECTION("Basic negative run")
	{
		std::vector<int8_t> test = {
			3,'a',-3,'b','c',42,7,'a',
			-3,'b','c',42,4,'a',
		};

		std::vector<int8_t> expected = {
			'a','a','a','b','c',42,
			'a','a','a','a','a','a',
			'a','b','c',42,'a','a','a','a',
		};

		bool result = RunDecompressionTest(test, expected);
		REQUIRE(result);
	}

	SECTION("Long positive over 127")
	{
		std::vector<int8_t> expected = {
			'c','c','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
			'a','a','a','a','a','b','c',42,
		};
		std::vector<int8_t> test = {
			2,'c',127,'a',2,'a',-3,'b','c',42
		};

		bool result = RunDecompressionTest(test, expected);
		REQUIRE(result);
	}

	SECTION("Long negative run")
	{
		std::vector<int8_t> expected = {
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b',

			'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
		};
		std::vector<int8_t> test = {
			-127,'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b',
			-107,'a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',
			'b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a','b','a',

		};

		bool result = RunDecompressionTest(test, expected);
		REQUIRE(result);
	}
}

TEST_CASE("File compression", "[file_compression]")
{
	RleFile r;
	SECTION("rle.bmp")
	{
		r.CreateArchive("Data/rle.bmp");
		bool result = CheckFileMD5("Data/rle.bmp.rle", "f2a9d8425d53c664e45d9eb1b53137b9");
		REQUIRE(result);
	}
	SECTION("pic.jpg")
	{
		r.CreateArchive("Data/pic.jpg");
		bool result = CheckFileMD5("Data/pic.jpg.rle", "0bbf2a5109b30d79939d2061ea8c74aa");
		REQUIRE(result);
	}
	SECTION("Conquest.ogg")
	{
		r.CreateArchive("Data/Conquest.ogg");
		bool result = CheckFileMD5("Data/Conquest.ogg.rle", "ec29ff368ec5100bfba22635ddc5ba5c");
		REQUIRE(result);
	}
}
