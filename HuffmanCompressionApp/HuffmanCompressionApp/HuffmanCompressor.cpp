#include "pch.h"
#include "HuffmanCompressor.h"
#include "GeneralConstants.h"
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

//Constructors---------------------------------------------------------------------------------------------------------------
HuffmanCompressor::HuffmanCompressor()
{
	FileAndDirectoryValidated = false;
	CharacterTableFinished = false;
	BinaryCodesFound = false;
	IsFinished = false;
	ExitError = false;
	BitCounter = 0;
	TotalBitCount = 1;
	StatusMessage = "Ready To Start!\n";
}

//Public functions-----------------------------------------------------------------------------------------------------------
void HuffmanCompressor::BeginCompression(std::string inputFilePath, std::string outputFolderPath)
{
	//Clear object to defaults
	ResetMembers();

	//Get filepath versions of the parameters
	std::filesystem::path inputPathObject = fs::path(inputFilePath);
	std::filesystem::path outputPathObject = fs::path(outputFolderPath);

	//Ensure that input and output paths even exist
	if (!fs::exists(inputPathObject))
	{
		SetupErrorMessage("The input file does not exist.\n");
		return;
	}

	if (!fs::exists(outputPathObject))
	{
		SetupErrorMessage("The output folder does not exist.\n");
		return;
	}

	//Ensure that the input is an actual file
	if (!fs::is_regular_file(inputPathObject))
	{
		SetupErrorMessage("The input file is not an actual file.\n");
		return;
	}

	//Ensure that the input is an actual file
	if (!fs::is_directory(outputPathObject))
	{
		SetupErrorMessage("The input file is not an actual file.\n");
		return;
	}
	FileAndDirectoryValidated = true;

	//Create table of unique characters and frequencies
	CharacterTable table;
	try
	{
		table = CharacterTable(inputFilePath);
	}
	//Catch block will be executed if the input file couldn't be opened
	catch (const std::invalid_argument& ia)
	{
		SetupErrorMessage(ia.what());
		return;
	}
	CharacterTableFinished = true;

	//Load the compressed binary codes into the character table, using PQ and huffman tree
	PriorityQueue pq = PriorityQueue(table, true);
	BinaryCodesFound = true;

	//Finally, create and build the compressed file
	try
	{
		WriteCompressedFile(table, inputFilePath, outputFolderPath);
	}
	//Catch block will be executed if any files could not be opened
	catch (const std::invalid_argument& ia)
	{
		SetupErrorMessage(ia.what());
		return;
	}

	//If it got here, the compressed file is successfully created
	StatusMessage = "Compression Complete!\n";
	ExitError = false;
	IsFinished = true;
}

//Private functions----------------------------------------------------------------------------------------------------------
void HuffmanCompressor::ResetMembers()
{
	FileAndDirectoryValidated = false;
	CharacterTableFinished = false;
	BinaryCodesFound = false;
	IsFinished = false;
	ExitError = false;
	BitCounter = 0;
	TotalBitCount = 1;
	StatusMessage = "Ready To Start!\n";
}

void HuffmanCompressor::SetupErrorMessage(std::string errorMessage)
{
	ExitError = true;
	IsFinished = true;
	StatusMessage = errorMessage;
}
void HuffmanCompressor::WriteCompressedFile(CharacterTable& table, std::string inputFilePath, std::string outputFolderPath)
{
	//Create full path to the output compressed file
	fs::path inputFileName = fs::path(inputFilePath).filename();
	fs::path outputFileName = inputFileName;
	outputFileName.replace_extension(COMPRESSED_EXTENSION);
	fs::path outputFilePath = fs::path(outputFolderPath);
	outputFilePath /= outputFileName;

	//Open up the output file for writing binaries
	std::ofstream outputWriter;
	outputWriter.open(outputFilePath, std::ios::binary);
	if (!outputWriter)
	{
		throw std::invalid_argument("Error in creating and opening compressed output file.\n");
	}

	//Record the input file's extension and its size
	std::string inputFileExtension = inputFileName.extension().string();
	uint16_t cStringExtensionSize = inputFileExtension.length()+1; //Uses 16 bits, since the extension can't have more than 256 (including null) because of filename length restrictions
	outputWriter.write(reinterpret_cast<char*>(&cStringExtensionSize), sizeof(uint16_t));

	const char* cStringExtension = inputFileExtension.c_str();
	outputWriter.write(cStringExtension, cStringExtensionSize);

	//Write the number of unique characters in the input to the output file
	uint64_t numOfUniqueChars = table.CharacterMap.size();
	outputWriter.write(reinterpret_cast<char*>(&numOfUniqueChars), sizeof(uint64_t));

	//Next, write every single unique character and its frequency to the output file
	//At the same time, calculate the number of bits needed for compressed file
	uint64_t currentFrequency;
	uint64_t tempTotalBitCount = 0;
	for (auto const& [key, node] : table.CharacterMap)
	{
		//Write char and frequency to file
		currentFrequency = table[key].GetFrequency();
		outputWriter.write(reinterpret_cast<const char*>(&key), sizeof(unsigned char));
		outputWriter.write(reinterpret_cast<char*>(&currentFrequency), sizeof(uint64_t));

		//Increment total number of bits
		tempTotalBitCount += table[key].BitRepresentation.size() * table[key].GetFrequency();
	}
	TotalBitCount = tempTotalBitCount;

	//Record the total number of bits in the upcoming huffman coding
	outputWriter.write(reinterpret_cast<const char*>(&TotalBitCount), sizeof(uint64_t));

	//Open up the input file to read bytes
	std::ifstream fileReader;
	fileReader.open(inputFilePath, std::ios::binary);
	fileReader >> std::noskipws;
	if (!fileReader)
	{
		throw std::invalid_argument("Unable to open input file to be compressed\n");
	}

	//Go through every character in the input file and write their compressed binary codes to the output
	unsigned char currentOutputByte = 0;
	unsigned char currentByte;
	while (fileReader.read(reinterpret_cast<char*>(&currentByte), sizeof(unsigned char)))
	{
		for (bool bit : table[currentByte].BitRepresentation)
		{
			//If the bit is true, that means a 1 should be set
			if (bit)
			{
				currentOutputByte |= 1 << (BitCounter % CHAR_BIT);
			}

			//If this is the last bit of the currentOutputByte, it should be written to the file and the variable should be reset
			if ((BitCounter % CHAR_BIT) == (CHAR_BIT - 1))
			{
				outputWriter.write(reinterpret_cast<char*>(&currentOutputByte), sizeof(unsigned char));
				currentOutputByte = 0;
			}

			BitCounter++;
		}
	}

	//Close out the input file
	fileReader.close();

	//If the number of bits is not an exact multiple of CHAR_BIT, that must mean the last byte hasn't been written yet and must be now
	if (TotalBitCount % CHAR_BIT != 0)
	{
		outputWriter.write(reinterpret_cast<char*>(&currentOutputByte), sizeof(unsigned char));
	}

	//Close out the output file
	outputWriter.close();
}