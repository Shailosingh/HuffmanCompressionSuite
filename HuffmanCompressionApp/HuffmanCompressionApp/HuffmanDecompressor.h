#pragma once

#include <string>
#include "TreeNode.h"
#include <filesystem>
namespace fs = std::filesystem;

class HuffmanDecompressor
{
private:
	/// <summary>
	/// Uses the fileReader handle to get the size of and then actual
	/// extension of the soon to be decompressed file.
	/// </summary>
	/// <param name="fileReader">File handler for binary reading of compressed file</param>
	/// <returns>Extension of soon to be decompressed file</returns>
	std::string GetDecompressedFileExtension(std::ifstream& fileReader);

	/// <summary>
	/// Writes decompressed file into the output path, using the input compressed file
	/// </summary>
	/// <param name="inputPathObject">Path to input compressed file</param>
	/// <param name="outputPathObject">Path to output folder where decompressed file will be made</param>
	/// <param name="extension">Extension of soon to be decompressed file</param>
	/// <param name="fileReader">Open file handle to the input compressed file. Ready to read the number of bits in the upcoming huffman coding.</param>
	/// <param name="huffmanTree">Head of huffman tree for compressed file</param>
	void WriteDecompressedFile(fs::path& inputPathObject, fs:: path& outputPathObject, std::string& extension, std::ifstream& fileReader, TreeNode* huffmanTree);

	/// <summary>
	/// Resets all members (status indicators) to default settings
	/// </summary>
	void ResetMembers();

	/// <summary>
	/// Puts the decompressor into error state, passing an error message
	/// </summary>
	/// <param name="errorMessage">Error message to be passed</param>
	void SetupErrorMessage(std::string errorMessage);

public:
	bool FileAndDirectoryValidated;
	bool RecordedFileExtension;
	bool CharacterTableFinished;
	bool TreeConstructed;
	bool IsFinished;
	bool ExitError;
	uint64_t BitCounter;
	uint64_t TotalBitCount;
	std::string StatusMessage;

	/// <summary>
	/// Constructor for decompressor. Will get the object ready to start compression process
	/// </summary>
	HuffmanDecompressor();

	/// <summary>
	/// Decompresses file pointed to by the inputFilePath and puts the compressed file in the outputFolderPath
	/// </summary>
	/// <param name="inputFilePath">Path to file to be decompressed</param>
	/// <param name="outputFolderPath">Path to folder where decompressed file shall be placed</param>
	void BeginDecompression(std::string inputFilePath, std::string outputFolderPath);
};