#define CATCH_CONFIG_MAIN
#include <iostream>
#include <vector>
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
		}
		return false;
	}

	return true;
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

TEST_CASE("RLE Compression", "[compression]")
{
	SECTION("+/- mix")
	{
		std::vector<int8_t> test = { 'a','b','a','a','b' };
		std::vector<int8_t> expected = { -2,'a','b',2,'a',1,'b' };

		bool result = RunCompressionTest(test, expected);

		REQUIRE(result);
	}
}
