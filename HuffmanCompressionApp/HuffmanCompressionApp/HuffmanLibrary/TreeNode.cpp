#include <pch.h>
#include "TreeNode.h"
#include <fstream>

//Constructors and Deconstructors--------------------------------------------------------------------------------------------
TreeNode::TreeNode(unsigned char newChar, uint64_t newFreq)
{
	Character = newChar;
	Frequency = newFreq;
	Left = nullptr;
	Right = nullptr;
}

TreeNode::TreeNode(TreeNode* leftNode, TreeNode* rightNode)
{
	Character = NULL;
	Frequency = leftNode->Frequency + rightNode->Frequency;
	Left = leftNode;
	Right = rightNode;
}

TreeNode::TreeNode(TreeNode* loneNodeException)
{
	Character = NULL;
	Frequency = loneNodeException->Frequency;
	Left = nullptr;
	Right = loneNodeException;
}

TreeNode::~TreeNode()
{
	//Clear values
	Character = NULL;
	Frequency = NULL;

	//Free memory of children
	delete Left;
	Left = nullptr;
	delete Right;
	Right = nullptr;
}

//Public functions-----------------------------------------------------------------------------------------------------------
void TreeNode::RetrieveBinaryCodes(CharacterTable& table) const
{
	//If the tree isn't just a head, go down its branches and record the proper binary code for each char
	if (Left != nullptr)
	{
		std::vector<bool> leftBitRep;
		leftBitRep.reserve(8);
		leftBitRep.push_back(false);
		Left->RetrieveBinaryCodes_Aux(table, leftBitRep);
	}

	if (Right != nullptr)
	{
		std::vector<bool> rightBitRep;
		rightBitRep.reserve(8);
		rightBitRep.push_back(true);
		Right->RetrieveBinaryCodes_Aux(table, rightBitRep);
	}
}

bool TreeNode::WriteDecompressedFile(std::ifstream& fileReader, std::ofstream& outputWriter, uint64_t totalBits, uint64_t& BitCounter)
{
	//Go through every bit in the huffman coding and interpret which characters to print into the decompressed file
	unsigned char currentOutputByte;
	uint64_t remainingBitCounter = totalBits;
	int bitEndCounter = CHAR_BIT;
	unsigned char currentByte;
	TreeNode* currentNode = this;

	while (fileReader.read(reinterpret_cast<char*>(&currentByte), sizeof(unsigned char)))
	{
		//If there is less than a full byte of bits left, ensure it only reads that many bits
		if (remainingBitCounter < CHAR_BIT)
		{
			//This causes a warning about converting "uint64_t" to "int". This is not a concern because, there is no way any computer will have more bits in a byte than an int's worth
			bitEndCounter = remainingBitCounter;
		}

		remainingBitCounter -= bitEndCounter;

		//Update the decompresor's bit counter
		BitCounter = totalBits - remainingBitCounter;

		for (int currentBit = 0; currentBit < bitEndCounter; currentBit++)
		{
			//If current bit leads down the left branch
			if ((currentByte & (1 << currentBit)) == 0)
			{
				if (currentNode->Left == nullptr)
				{
					return false;
				}
				
				currentNode = currentNode->Left;
			}

			//If current bit leads down the right branch
			//NOTE: I made this a simple "else" statement instead of a deliberate  "else if" because, it is simpler and there isn't an easy way to scan for bits being 1 without just checking if it is not 0.
			else
			{
				if (currentNode->Right == nullptr)
				{
					return false;
				}

				currentNode = currentNode->Right;
			}

			//Tree node has no children
			if (currentNode->Left == nullptr && currentNode->Right == nullptr)
			{
				currentOutputByte = currentNode->Character;
				outputWriter.write(reinterpret_cast<char*>(&currentOutputByte), sizeof(unsigned char));

				//Reset currentNode to the root of the tree
				currentNode = this;
			}
		}

		if (remainingBitCounter == 0)
		{
			break;
		}

	}

	//If it got this far, everything must be good
	return true;
}


//Private functions----------------------------------------------------------------------------------------------------------
void TreeNode::RetrieveBinaryCodes_Aux(CharacterTable& table, std::vector<bool> currentBits) const
{
	//If both the left and right node are nullptr, we are at a character and should record the binary code
	if (Left == nullptr && Right == nullptr)
	{
		table[Character].BitRepresentation = currentBits;
		return;
	}

	//Continue down the non null branches
	if (Left != nullptr)
	{
		std::vector<bool> leftBitRep = currentBits;
		leftBitRep.push_back(false);
		Left->RetrieveBinaryCodes_Aux(table, leftBitRep);
	}

	if (Right != nullptr)
	{
		std::vector<bool> rightBitRep = currentBits;
		rightBitRep.push_back(true);
		Right->RetrieveBinaryCodes_Aux(table, rightBitRep);
	}
}

//Operator overloads---------------------------------------------------------------------------------------------------------
bool TreeNode::operator<(const TreeNode& other) const
{
	return ((this->Frequency) < (other.Frequency));
}