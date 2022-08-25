#include "pch.h"
#include "CharacterTableNode.h"

//Constructors---------------------------------------------------------------------------------------------------------------
CharacterTableNode::CharacterTableNode()
{
	Frequency = 0;
}

CharacterTableNode::CharacterTableNode(uint64_t freq)
{
	Frequency = freq;
}

//Operator overloads---------------------------------------------------------------------------------------------------------
void CharacterTableNode::operator++(int incrementAmount)
{
	if (incrementAmount == 0)
	{
		Frequency++;
	}

	else
	{
		Frequency += incrementAmount;
	}
}

//Public functions-----------------------------------------------------------------------------------------------------------
uint64_t CharacterTableNode::GetFrequency() const
{
	return Frequency;
}