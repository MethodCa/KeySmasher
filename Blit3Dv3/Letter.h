#pragma once

#include<Blit3D.h>
#include <string.h>


class Letter
{
public:
	bool status = false;
	float timer = 0;	//timer for the letter jto be displayed
	char value = ' ';
	std::string letra = " ";
	std::vector<Letter> getAlphabet();
};


class Position
{
public:
	glm::vec2 gridPosition;
	bool status;
	std::vector<Position> positionOnDisplay();
};

class LetterPosition
{
public:
	Position position;
	Letter letter;
	bool status = false;
	int timer = 0;
};

class Util
{
public:
	void InitializeRNG();
	int getRandom(int value1, int value2);
	std::vector<Letter> shuffle(std::vector<Letter> alphabet);

	std::vector<Position> shufflePos(std::vector<Position> posGrid);
	std::vector<LetterPosition> createGameList(std::vector<Letter> alphabet, std::vector<Position> posGrid, int timer);
};

