#include "RleData.h"

void RleData::Compress(const std::vector<int8_t>& inputData)
{
	// Clear data just in case
	mData.clear();

	// Reserve double inputData size (worst-case largest output)
	mData.reserve(inputData.size() * 2);

	std::vector<int8_t> temp;

	// Positive run (first loop through the inputData array to see if the data at an index is repeated
	for (size_t i = 0; i < inputData.size(); ++i)
	{
		// Start a count of the data
		int8_t count = 1;
		// Loop through the rest of the array and check to see if the data or character repeats
		// Loop as long as inputData is repeated in the next index and count is less than 127
		while ((i < inputData.size() - 1) && (inputData[i] == inputData[i + 1]) && (count < 127))
		{
			++count;
			++i;
		}
		// Add the data and the number of times it repeats into the temp array
		temp.emplace_back(count);
		temp.emplace_back(inputData[i]);
	}

	// Array for unique data
	std::vector<int8_t> uniqueArray;

	// Loop through the count of each data
	for (size_t i = 0; i < temp.size(); i += 2)
	{
		// Keep track of a count for unique data, each time a unique character or data is seen, decrement this counter
		int8_t negativeCount = 0;
		// The current count of the data
		int8_t currentCount = temp[i];

		// If the current count is 1, loop to check to see if there is a string of unique data that also has a count of 1
		if (currentCount == 1)
		{
			// Subtract from counter and add the data to the unique array
			--negativeCount;
			uniqueArray.emplace_back(temp[i + 1]);

			// Loop through temp array, checking to see if the data after the current one also has a count of 1
			// Loop as long as the count is 1 in the next bit of data and negative count is greater than -127 and add to unique array
			while ((i < temp.size() - 2) && currentCount == 1 && (temp[i] == temp[i + 2]) && negativeCount > -127)
			{
				--negativeCount;
				i += 2;
				uniqueArray.emplace_back(temp[i + 1]);
			}

			// If negative count is -1, a unique bit of data is only seen once, so revert to back to positive 1 and add to mData
			if (negativeCount == -1)
			{
				mData.emplace_back(1);
				mData.emplace_back(uniqueArray[0]);
			}
			else
			{
				// Add the count of unique characters or data to mData
				mData.emplace_back(negativeCount);
				// Loop through the unique array and add them to mData
				for (auto u : uniqueArray)
				{
					mData.emplace_back(u);
				}
			}
			// Clear the count and unique array
			negativeCount = 0;
			uniqueArray.clear();
		}
		else
		{
			// No unique data, just add count and data to mData
			mData.emplace_back(currentCount);
			mData.emplace_back(temp[i + 1]);
		}	
	}
}

void RleData::Decompress(const std::vector<int8_t>& inputData, size_t outputSize)
{
	// Clear mData just in case
	mData.clear();

	// Reserve the desired output size in mData
	mData.reserve(outputSize);

	// Loop through input data and check all the number of elements for each data
	for (size_t i = 0; i < inputData.size(); ++i)
	{
		// Get the current count of elements
		int8_t currentCount = inputData[i];

		// If count is positive, loop through the number of elements and add to mData
		if (currentCount > 0)
		{
			for (size_t j = 0; j < currentCount; ++j)
			{
				mData.emplace_back(inputData[i + 1]);
			}
			// Increment i to check the next count of elements
			++i;
		}
		else
		{
			// Make the number of unique elements positive
			currentCount *= -1;
			// Loop through the count and add each unique element to mData
			for (size_t j = 0; j < currentCount; ++j)
			{
				mData.emplace_back(inputData[i + 1]);
				++i;
			}
		}
	}
}