#pragma once

#include "CharacterTable.h"
#include "PriorityQueue.h"

class HuffmanCompressor
{
private:
	/// <summary>
	/// Resets all members (status indicators) to default settings
	/// </summary>
	void ResetMembers();

	/// <summary>
	/// Puts the compressor into error state, passing an error message
	/// </summary>
	/// <param name="errorMessage">Error message to be passed</param>
	void SetupErrorMessage(std::string errorMessage);

	/// <summary>
	/// Uses the binary codes in the table to create the compressed file at the outputFolderPath
	/// </summary>
	/// <param name="table">The character table that carries each unique character's frequency and binary code</param>
	/// <param name="inputFilePath">Path to the input file to be compressed</param>
	/// <param name="outputFolderPath">Path to the folder where the compressed file shall be created in</param>
	void WriteCompressedFile(CharacterTable& table, std::string inputFilePath, std::string outputFolderPath);

public:
	bool FileAndDirectoryValidated;
	bool CharacterTableFinished;
	bool BinaryCodesFound;
	bool IsFinished;
	bool ExitError;
	uint64_t BitCounter;
	uint64_t TotalBitCount;
	std::string StatusMessage;

	/// <summary>
	/// Constructor for the compressor. Will get the object ready to start compression process
	/// </summary>
	HuffmanCompressor();

	/// <summary>
	/// Compresses file pointed to by the inputFilePath and puts the compressed file in the outputFolderPath
	/// </summary>
	/// <param name="inputFilePath">Path to file to be compressed</param>
	/// <param name="outputFolderPath">Path to folder where compressed file shall be placed</param>
	void BeginCompression(std::string inputFilePath, std::string outputFolderPath);
};