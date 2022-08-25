#include "pch.h"
#include "CharacterTable.h"
#include <fstream>

//Constructors---------------------------------------------------------------------------------------------------------------
CharacterTable::CharacterTable(std::string decompressedFilePath)
{
	//Open file for reading binary
	std::ifstream fileReader;
	fileReader.open(decompressedFilePath, std::ios::binary);
	fileReader >> std::noskipws;

	//If file unable to open, throw an error
	if (!fileReader)
	{
		throw std::invalid_argument("Unable to open input file to be compressed\n");
	}

	//Read every byte of the file and record their frequencies in the map
	unsigned char currentByte;
	while (fileReader.read(reinterpret_cast<char*>(&currentByte), sizeof(unsigned char)))
	{
		CharacterMap[currentByte]++;
	}

	//Close up the file stream
	fileReader.close();
}

CharacterTable::CharacterTable(std::ifstream& compressedFileReader)
{
	//Get the number of unique characters from file
	uint64_t numberOfUniqueCharacters;
	if (!compressedFileReader.read(reinterpret_cast<char*>(&numberOfUniqueCharacters), sizeof(uint64_t)))
	{
		throw std::invalid_argument("Input file corrupt or catastrophic error occured!\n");
	}

	//Ensure that the given number of unique characters is possible (not larger than 2^CHAR_BIT)
	if (numberOfUniqueCharacters > pow(2, CHAR_BIT))
	{
		throw std::invalid_argument("Input file corrupt!\n");
	}

	//Go through every unique character and place it in the table
	unsigned char currentChar;
	uint64_t currentFrequency;
	for (uint64_t index = 0; index < numberOfUniqueCharacters; index++)
	{
		if (!compressedFileReader.read(reinterpret_cast<char*>(&currentChar), sizeof(unsigned char)))
		{
			throw std::invalid_argument("Input file corrupt or catastrophic error occured!\n");
		}
		if (!compressedFileReader.read(reinterpret_cast<char*>(&currentFrequency), sizeof(uint64_t)))
		{
			throw std::invalid_argument("Input file corrupt or catastrophic error occured!\n");
		}

		CharacterMap[currentChar] = CharacterTableNode(currentFrequency);
	}
}

CharacterTable::CharacterTable()
{
	//Does nothing
}

//Operator overloads---------------------------------------------------------------------------------------------------------
CharacterTableNode CharacterTable::operator[](unsigned char key) const
{
	return CharacterMap.at(key);
}

CharacterTableNode& CharacterTable::operator[](unsigned char key)
{
	return CharacterMap.at(key);
}