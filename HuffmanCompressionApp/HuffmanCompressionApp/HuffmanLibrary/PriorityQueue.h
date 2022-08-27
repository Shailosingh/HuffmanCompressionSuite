#pragma once
#include "TreeNode.h"
#include <list>

class PriorityQueue
{
private:
	std::list<TreeNode*> Queue;

	/// <summary>
	/// Inserts the new node and maintains the least frequency to greatest frequency order of the queue.
	/// </summary>
	/// <param name="newNode">TreeNode pointer to be inserted</param>
	void SortedInsert(TreeNode* newNode);
	
	/// <summary>
	/// Turns the priority queue into a huffman tree and sets the pointer to the  head of the tree to "HuffmanTree"
	/// </summary>
	/// <returns>Pointer to the head of the huffman tree</returns>
	TreeNode* CreateTree();

public:
	TreeNode* HuffmanTree;

	/// <summary>
	/// Constructor uses character table to create a priority queue which will then be morphed into a huffman tree.
	/// </summary>
	/// <param name="table">Reference to character table with unique characters and its frequencies and binary representation</param>
	/// <param name="doRetrieveBinaryCode">If this is true, it will fill the table with huffman binary codes</param>
	PriorityQueue(CharacterTable& table, bool doRetrieveBinaryCode);

	/// <summary>
	/// Deconstructor which will destruct its associated huffman tree
	/// </summary>
	~PriorityQueue();
};