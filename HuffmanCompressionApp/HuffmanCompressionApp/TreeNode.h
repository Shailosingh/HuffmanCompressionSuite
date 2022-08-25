#pragma once

#include "CharacterTable.h"

class TreeNode
{
private:
	unsigned char Character;
	uint64_t Frequency;
	TreeNode* Left;
	TreeNode* Right;

	/// <summary>
	/// Auxillary function for recursively retrieving binary codes. 
	/// </summary>
	/// <param name="table">Reference to the character table which stores huffman binary representations of each</param>
	/// <param name="currentBits">Binary representation thus far of current path going down tree</param>
	void RetrieveBinaryCodes_Aux(CharacterTable& table, std::vector<bool> currentBits) const;

public:
	/// <summary>
	/// Constructor which creates node with no children but, sets Character and Frequency
	/// </summary>
	/// <param name="newChar">Character to be set</param>
	/// <param name="newFreq">Frequency to be set</param>
	TreeNode(unsigned char newChar, uint64_t newFreq);

	/// <summary>
	/// Constructor which creates a new node with leftNode and rightNode as children, frequency as
	/// the sum of its children's frequencies and a NULL character.
	/// </summary>
	/// <param name="leftNode">Node to be left child</param>
	/// <param name="rightNode">Node to be right child</param>
	TreeNode(TreeNode* leftNode, TreeNode* rightNode);

	/// <summary>
	/// Constructor which is used when there is only one unique character in the input file and thus
	/// priority queue. This creates a parent node with the loneNodeException as its right child and
	/// its frequency as the newly constructed parent node's frequency.
	/// </summary>
	/// <param name="loneNodeException">The lone node in the priority queue with the single unique character</param>
	TreeNode(TreeNode* loneNodeException);

	/// <summary>
	/// Destructor which will recursively destruct all the children of the current node.
	/// </summary>
	~TreeNode();
	
	/// <summary>
	/// Gets the binary code for each character and inserts it into the table.
	/// </summary>
	/// <param name="table">Table that carries each character, frequency and binary representation</param>
	void RetrieveBinaryCodes(CharacterTable& table) const;

	/// <summary>
	/// Takes fileReader handle (pointed at start of huffman code) and interprets the coding
	/// with the tree to write the decompressed bytes to the outputWriter.
	/// </summary>
	/// <param name="fileReader">Open file handle pointed to the huffman coding of the input file</param>
	/// <param name="outputWriter">Open file handle to the new decompressed file</param>
	/// <param name="totalBits">Total number of expected huffman coded bits</param>
	/// <param name="BitCounter">A reference to the Decompressor's bit counter. Keep the decompressor updated</param>
	/// <returns>True if everything is successful. False if something goes bad</returns>
	bool WriteDecompressedFile(std::ifstream& fileReader, std::ofstream& outputWriter, uint64_t totalBits, uint64_t& BitCounter);

	/// <summary>
	/// Compares this and other by frequency numbers
	/// </summary>
	/// <param name="other">Node to be compared</param>
	/// <returns>True if this node has a smaller frequency than other</returns>
	bool operator<(const TreeNode& other) const;
};