
#include"Letter.h"
#include <random>

std::mt19937 rng;

void Util::InitializeRNG()
{
	std::random_device rd;
	rng.seed(rd());
}

int Util::getRandom(int value1, int value2)
{
	std::uniform_int_distribution<int> randomNumber(value1, value2);
	return randomNumber(rng);
}

std::vector<Letter> Util::shuffle(std::vector<Letter> alphabet)
{
	std::vector<Letter> t_alphabet;
	for (int i = 0; i < 30; i++)
	{
		int number = getRandom(0, 29);
		if (alphabet.at(number).status)
		{
			t_alphabet.push_back(alphabet.at(number));
			alphabet.at(number).status = false;
		}
		else
		{
			int var = 0;
			while (!alphabet.at(var).status && var <30)
			{
				var++;
			}
			t_alphabet.push_back(alphabet.at(var));
			alphabet.at(var).status = false;
		}

	}

	return t_alphabet;
}

std::vector<Position> Util::shufflePos(std::vector<Position> posGrid)
{
	std::vector<Position> t_posGrid;

	for (int i = 0; i < 30; i++)
	{
		int number = getRandom(0, 29);
		if (posGrid.at(number).status)
		{
			t_posGrid.push_back(posGrid.at(number));
			posGrid.at(number).status = false;
		}
		else
		{
			int var = 0;
			while (!posGrid.at(var).status)
			{
				var++;
			}
			t_posGrid.push_back(posGrid.at(var));
			posGrid.at(var).status = false;
		}
	}
	return( t_posGrid);
	
}

std::vector<LetterPosition> Util::createGameList(std::vector<Letter> alphabet, std::vector<Position> posGrid, int timer)
{
	std::vector<LetterPosition> gameO;
	
	for (int i = 0; i < 30; i++)
	{
		LetterPosition game;
		game.position = posGrid.at(i);
		game.letter = alphabet.at(i);
		game.status = true;
		game.timer = timer;

		gameO.push_back(game);
	}
	return gameO;
}

std::vector<Letter> Letter::getAlphabet()
{
	std::vector<Letter> alphabet;
	for (int i = 65; i < 92; i++) //95
	{
		Letter letra;
		if (i > 90)
		{
			for (int j = 0; j < 4; j++)
			{
				letra.status = true;
				letra.timer = 0;
				letra.value = (char)(i-1);
				letra.letra = "Media\\letters\\KS_" + std::to_string(letra.value) + ".png";
				alphabet.push_back(letra);
			}
		}
		else
		{
			letra.status = true;
			letra.timer = 0;
			letra.value = (char)i;
			letra.letra = "Media\\letters\\KS_" + std::to_string(letra.value) + ".png";
			alphabet.push_back(letra);
		}
		

	}
	return alphabet;
}

std::vector<Position> Position::positionOnDisplay()
{
	Position posGrid;
	std::vector<Position> positionsOnDisplay;

	for (int i = 0; i < 6; i++)
	{

		for (int j = 0; j < 5; j++)
		{
			posGrid.status = true;
			posGrid.gridPosition.x = 620 + (i * 200);
			posGrid.gridPosition.y = 1040 - (j * 200);

			positionsOnDisplay.push_back(posGrid);
		}
	}
	return positionsOnDisplay;
}
