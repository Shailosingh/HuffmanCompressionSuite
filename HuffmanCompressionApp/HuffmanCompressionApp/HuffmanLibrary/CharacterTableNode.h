#pragma once

#include <vector>

class CharacterTableNode
{
private:
	uint64_t Frequency;

public:
	std::vector<bool> BitRepresentation;

	/// <summary>
	/// Creates a default node with 0 frequency and no BitRepresentation
	/// </summary>
	CharacterTableNode();

	/// <summary>
	/// Creates node with set frequency and no bit representation
	/// </summary>
	/// <param name="frequency">Frequency value to be set to node</param>
	CharacterTableNode(uint64_t frequency);

	/// <summary>
	/// Increments the frequency of the the character
	/// </summary>
	/// <param name="incrementAmount">The amount the frequency shall be incrmented by</param>
	void operator++(int incrementAmount);

	uint64_t GetFrequency() const;
};