#include <pch.h>
#include "PriorityQueue.h"

//Constructors and Destructors-----------------------------------------------------------------------------------------------
PriorityQueue::PriorityQueue(CharacterTable& table, bool doRetrieveBinaryCode)
{
	//Iterate through the CharacterTable's map and insert its elements into the PriorityQueue
	TreeNode* newlyCreatedNode;
	for (auto const& [key, node] : table.CharacterMap)
	{
		newlyCreatedNode = new TreeNode(key, node.GetFrequency());
		SortedInsert(newlyCreatedNode);
	}

	//Perform the huffman process on the queue to create the huffman tree
	HuffmanTree = CreateTree();

	//Get the binary code (if user wants) from each character and place it into table
	if (doRetrieveBinaryCode)
	{
		HuffmanTree->RetrieveBinaryCodes(table);
	}

	//Clear out the priority queue
	Queue.clear();
}

PriorityQueue::~PriorityQueue()
{
	//Clean out the huffman tree before the priority queue is destroyed
	delete HuffmanTree;
	HuffmanTree = nullptr;
}

//Private functions----------------------------------------------------------------------------------------------------------
void PriorityQueue::SortedInsert(TreeNode* newNode)
{
	//Iterate through the Queue and insert newNode in the first position where the next element is greater than newNode
	for(std::list<TreeNode*>::iterator iterator = Queue.begin(); iterator != Queue.end(); iterator++)
	{
		if (*newNode < **iterator)
		{
			Queue.insert(iterator, newNode);
			return;
		}
	}

	//If it hasn't been inserted yet, insert at end of list
	Queue.push_back(newNode);
}

TreeNode* PriorityQueue::CreateTree()
{
	//Initialize variables
	TreeNode* head;

	//If there is only one unique char in the queue, create special tree
	if (Queue.size() == 1)
	{
		head = new TreeNode(Queue.front());
	}

	//If there is more than one unique character, perform the normal huffman process and create the tree
	TreeNode* leftNode;
	TreeNode* rightNode;
	TreeNode* combinedNode;
	while (Queue.size() != 1)
	{
		//Take the two smallest frequency nodes out of the queue, combine them and add the combined node back
		leftNode = Queue.front();
		Queue.pop_front();
		rightNode = Queue.front();
		Queue.pop_front();
		combinedNode = new TreeNode(leftNode, rightNode);
		SortedInsert(combinedNode);
	}

	//Once the queue is only one element long, the remaining pointer must be the head of the completed huffman tree
	return Queue.front();
}