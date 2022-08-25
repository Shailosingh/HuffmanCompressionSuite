#include "pch.h"
#include "GeneralConstants.h"
#include "HuffmanDecompressor.h"
#include "PriorityQueue.h"
#include <fstream>
#include <filesystem>
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
	StatusMessage = "Ready to go!\n";
}

//Public functions-----------------------------------------------------------------------------------------------------------
void HuffmanDecompressor::BeginDecompression(std::string inputFilePath, std::string outputFolderPath)
{
	//Clear object to defaults
	ResetMembers();

	//Get filepath versions of the parameters
    fs::path inputPathObject = fs::path(inputFilePath);
	fs::path outputPathObject = fs::path(outputFolderPath);

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

	//Ensure that the input's extension is the correct one for files compressed with this program
	if (inputPathObject.extension().string() != COMPRESSED_EXTENSION)
	{
		SetupErrorMessage("The input file is invalid. Wrong extension!\n");
		return;
	}
	FileAndDirectoryValidated = true;

	//Create file handler for binary reading, so we can have the info needed to create the decompressed file
	std::ifstream fileReader;
	fileReader.open(inputFilePath, std::ios::binary);
	fileReader >> std::noskipws;
	if (!fileReader)
	{
		SetupErrorMessage("Unable to open input file to be decompressed!\n");
		return;
	}

	//Get the soon to be decompressed file's extension and handle any corruption errors
	std::string extension;
	try
	{
		extension = GetDecompressedFileExtension(fileReader);
	}
	catch (const std::invalid_argument& ia)
	{
		SetupErrorMessage(ia.what());
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
		SetupErrorMessage(ia.what());
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
		SetupErrorMessage(ia.what());
		return;
	}

	//Close up the input fileReader handle
	fileReader.close();
	
	//Success reached
	StatusMessage = "Decompression Complete!\n";
	ExitError = false;
	IsFinished = true;
}

//Private functions----------------------------------------------------------------------------------------------------------
std::string HuffmanDecompressor::GetDecompressedFileExtension(std::ifstream& fileReader)
{
	//Get the size of the decompressed file extension
	uint16_t sizeOfExtension;
	if (!fileReader.read(reinterpret_cast<char*>(&sizeOfExtension), sizeof(uint16_t)))
	{
		throw std::invalid_argument("Input file corrupt or catastrophic error occured!\n");
	}

	//If the sizeOfExtension-1 (to account for the null char) is more than the max filename size, the input file must be corrupt
	if ((sizeOfExtension - 1) > MAX_FILENAME_LENGTH)
	{
		throw std::invalid_argument("Input file corrupt!\n");
	}

	//Create buffer and fill it with the file extension
	char* extensionBuffer = new char[sizeOfExtension];
	if (!fileReader.read(extensionBuffer, sizeof(char) * sizeOfExtension))
	{
		throw std::invalid_argument("Input file corrupt or catastrophic error occured!\n");
	}
	std::string extensionString(extensionBuffer);
	
	//Delete the buffer as it isn't needed anymore
	delete[] extensionBuffer;

	return extensionString;
}

void HuffmanDecompressor::WriteDecompressedFile(fs::path& inputPathObject, fs::path& outputPathObject, std::string& extension, std::ifstream& fileReader, TreeNode* huffmanTree)
{
	//Get the number of huffman coding bits to come
	uint64_t totalBits;
	if (!fileReader.read(reinterpret_cast<char*>(&totalBits), sizeof(uint64_t)))
	{
		throw std::invalid_argument("Input file corrupt or catastrophic error occured!\n");
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
		throw std::invalid_argument("Error in creating and opening decompressed output file.\n");
	}

	//Use the huffman tree to finish the decompressed file
	if (!huffmanTree->WriteDecompressedFile(fileReader, outputWriter, totalBits, BitCounter))
	{
		throw std::invalid_argument("Catastrophic error in reading the input file's huffman coding!\n");
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
	StatusMessage = "Ready to go!\n";
}

void HuffmanDecompressor::SetupErrorMessage(std::string errorMessage)
{
	ExitError = true;
	IsFinished = true;
	StatusMessage = errorMessage;
}