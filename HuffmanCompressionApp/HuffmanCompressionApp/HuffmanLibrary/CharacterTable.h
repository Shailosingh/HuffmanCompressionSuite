#pragma once

#include "CharacterTableNode.h"
#include <map> //NEVER CHANGE THIS TO "unordered_map." It won't work because the CharacterTable order needs to be the exact same during compression and decompression, for the huffman trees to be identical.
#include <string>

class CharacterTable
{
public:
	std::map<unsigned char, CharacterTableNode> CharacterMap;

	/// <summary>
	/// Creates a literally empty table
	/// </summary>
	CharacterTable(); 

	/// <summary>
	/// Creates character table out of file to be compressed
	/// </summary>
	/// <param name="decompressedFilePath">Uncompressed file</param>
	CharacterTable(std::wstring decompressedFilePath); 

	/// <summary>
	/// Creates character table out of file being decompressed
	/// </summary>
	/// <param name="compressedFileReader">Compressed file</param>
	CharacterTable(std::ifstream& compressedFileReader);

	//Operator overloads to get and set CharacterMap values in a safer way (using "at")
	CharacterTableNode operator[](unsigned char key) const;
	CharacterTableNode& operator[](unsigned char key);
};