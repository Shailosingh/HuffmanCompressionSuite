#include <pch.h>
#include "GeneralConstants.h"
#include "HuffmanDecompressor.h"
#include "PriorityQueue.h"
#include <fstream>
#include <filesystem>
#include <Windows.h>
namespace fs = std::filesystem;

//Constructors---------------------------------------------------------------------------------------------------------------
HuffmanDecompressor::HuffmanDecompressor()
{
	FileAndDirectoryValidated = false;
	RecordedFileExtension = false;
	CharacterTableFinished = false;
	TreeConstructed = false;
	IsFinished = false;
	ExitError = false;
	BitCounter = 0;
	TotalBitCount = 1;
	StatusMessage = L"Ready to go!";
}

//Public functions-----------------------------------------------------------------------------------------------------------
void HuffmanDecompressor::BeginDecompression(std::wstring inputFilePath, std::wstring outputFolderPath)
{
	//Clear object to defaults
	ResetMembers();

	//Get filepath versions of the parameters
    fs::path inputPathObject = fs::path(inputFilePath);
	fs::path outputPathObject = fs::path(outputFolderPath);

	//Ensure that input and output paths even exist
	if (!fs::exists(inputPathObject))
	{
		SetupErrorMessage(L"The input file does not exist.");
		return;
	}

	if (!fs::exists(outputPathObject))
	{
		SetupErrorMessage(L"The output folder does not exist.");
		return;
	}

	//Ensure that the input is an actual file
	if (!fs::is_regular_file(inputPathObject))
	{
		SetupErrorMessage(L"The input file is not an actual file.");
		return;
	}

	//Ensure that the input is an actual file
	if (!fs::is_directory(outputPathObject))
	{
		SetupErrorMessage(L"The input file is not an actual file.");
		return;
	}

	//Ensure that the input's extension is the correct one for files compressed with this program
	if (inputPathObject.extension().wstring() != COMPRESSED_EXTENSION)
	{
		SetupErrorMessage(L"The input file is invalid. Wrong extension!");
		return;
	}
	FileAndDirectoryValidated = true;

	//Create file handler for binary reading, so we can have the info needed to create the decompressed file
	std::ifstream fileReader;
	fileReader.open(inputFilePath, std::ios::binary);
	fileReader >> std::noskipws;
	if (!fileReader)
	{
		SetupErrorMessage(L"Unable to open input file to be decompressed!");
		return;
	}

	//Get the soon to be decompressed file's extension and handle any corruption errors
	std::wstring extension;
	try
	{
		extension = GetDecompressedFileExtension(fileReader);
	}
	catch (const std::invalid_argument& ia)
	{
		SetupErrorMessage(ConvertNarrowStringToWideString(ia.what()));
		return;
	}
	RecordedFileExtension = true;

	//Create an empty CharacterTable and have it filled up with the input compressed file's info
	CharacterTable table;
	try
	{
		table = CharacterTable(fileReader);
	}
	catch (const std::invalid_argument& ia)
	{
		SetupErrorMessage(ConvertNarrowStringToWideString(ia.what()));
		return;
	}
	CharacterTableFinished = true;

	//Construct the huffman tree but, do not get the binary codes for each char yet
	PriorityQueue pq = PriorityQueue(table, false);
	TreeConstructed = true;

	//Use all information gathered to finish up the decompressed file
	try
	{
		WriteDecompressedFile(inputPathObject, outputPathObject, extension, fileReader, pq.HuffmanTree);
	}
	catch (const std::invalid_argument& ia)
	{
		SetupErrorMessage(ConvertNarrowStringToWideString(ia.what()));
		return;
	}

	//Close up the input fileReader handle
	fileReader.close();
	
	//Success reached
	StatusMessage = L"Decompression Complete!";
	ExitError = false;
	IsFinished = true;
}

//Private functions----------------------------------------------------------------------------------------------------------
std::wstring HuffmanDecompressor::GetDecompressedFileExtension(std::ifstream& fileReader)
{
	//Get the size of the decompressed file extension
	uint16_t sizeOfExtension;
	if (!fileReader.read(reinterpret_cast<char*>(&sizeOfExtension), sizeof(uint16_t)))
	{
		throw std::invalid_argument("Input file corrupt or catastrophic error occured!");
	}

	//If the sizeOfExtension-1 (to account for the null char) is more than the max filename size, the input file must be corrupt
	if ((sizeOfExtension - 1) > MAX_FILENAME_LENGTH)
	{
		throw std::invalid_argument("Input file corrupt!");
	}

	//Create buffer and fill it with the file extension
	wchar_t* extensionBuffer = new wchar_t[sizeOfExtension];
	if (!fileReader.read(reinterpret_cast<char*>(extensionBuffer), sizeof(wchar_t) * sizeOfExtension))
	{
		throw std::invalid_argument("Input file corrupt or catastrophic error occured!");
	}
	std::wstring extensionString(extensionBuffer);
	
	//Delete the buffer as it isn't needed anymore
	delete[] extensionBuffer;

	return extensionString;
}

void HuffmanDecompressor::WriteDecompressedFile(fs::path& inputPathObject, fs::path& outputPathObject, std::wstring& extension, std::ifstream& fileReader, TreeNode* huffmanTree)
{
	//Get the number of huffman coding bits to come
	uint64_t totalBits;
	if (!fileReader.read(reinterpret_cast<char*>(&totalBits), sizeof(uint64_t)))
	{
		throw std::invalid_argument("Input file corrupt or catastrophic error occured!");
	}
	TotalBitCount = totalBits;

	//Create full path to the output decompressed file
	fs::path inputFileName = inputPathObject.filename();
	fs::path outputFileName = inputFileName;
	outputFileName.replace_extension(extension);
	fs::path outputFilePath = outputPathObject;
	outputFilePath /= outputFileName;

	//Open up the output file for writing binaries
	std::ofstream outputWriter;
	outputWriter.open(outputFilePath, std::ios::binary);
	if (!outputWriter)
	{
		throw std::invalid_argument("Error in creating and opening decompressed output file.");
	}

	//Use the huffman tree to finish the decompressed file
	if (!huffmanTree->WriteDecompressedFile(fileReader, outputWriter, totalBits, BitCounter))
	{
		throw std::invalid_argument("Catastrophic error in reading the input file's huffman coding!");
	}

	//Close up the output file
	outputWriter.close();
}

void HuffmanDecompressor::ResetMembers()
{
	FileAndDirectoryValidated = false;
	RecordedFileExtension = false;
	CharacterTableFinished = false;
	TreeConstructed = false;
	IsFinished = false;
	ExitError = false;
	BitCounter = 0;
	TotalBitCount = 1;
	StatusMessage = L"Ready to go!";
}

void HuffmanDecompressor::SetupErrorMessage(std::wstring errorMessage)
{
	ExitError = true;
	IsFinished = true;
	StatusMessage = errorMessage;
}

std::wstring HuffmanDecompressor::ConvertNarrowStringToWideString(const std::string& narrowString) const
{
	//Allocates a wide string of appropriate size for conversion from narrow string
	int neededWideBufferSize = MultiByteToWideChar(CP_UTF8, 0, narrowString.c_str(), -1, NULL, 0);
	std::wstring wideString(neededWideBufferSize, 0);

	//Fills and returns the wide string
	MultiByteToWideChar(CP_UTF8, 0, narrowString.c_str(), -1, &wideString[0], neededWideBufferSize);
	return wideString;
}