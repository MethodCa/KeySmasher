
#include "Blit3D.h"
#include "Letter.h"
#include <iostream>
#include <conio.h>

Blit3D *blit3D = NULL;

//memory leak detection
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//GLOBAL DATA
Sprite *backgroundSprite = NULL; //a pointer to a background sprite

AngelcodeFont * pixeled = NULL;
AngelcodeFont* pixeledWhite = NULL;
AngelcodeFont* time35 = NULL;
AngelcodeFont* time35white = NULL;

//**************************************************
Sprite* BG = NULL; //a pointer to a background sprite
Sprite* backgroundStats = NULL; //a pointer to a background sprite
Sprite* backgroundGame = NULL; //a pointer to a background sprite
Sprite* youwin = NULL; //a pointer to a you win sprite
Sprite* spriteLetter = NULL; //sprite for letters to display
Sprite* gameOverMessage = NULL; //a pointer to a game over sprite
Letter* letter;
Util* util;
Position* position;
LetterPosition* play;
std::vector<LetterPosition> playable;
bool timeToDraw = false;
bool gameOver = false;
int timer = 150;
int timeLife = 600;
int highScore = 0;
int level = 0;
double elapsedTime = 0;
int currentIndex = 0;
double frames = timer;
int lives = 6;
char currentKeyPressed = '1';
bool liveDiscount = false;
bool keyPressed= false;
int splashScreenTime = 180;
bool youwon = false;


void makeLevel()
{
	std::vector<Letter> alphabet;
	std::vector<Position> positionsOnDisplay;
	

	level++;
	currentKeyPressed = '1';
	switch (level)
	{
	case 1:
		timer = 80;	//time between letters displayed
		timeLife =200;	//life time of a letter on display
		break;
	case 2:
		timer = 70;
		timeLife = 150;
		break;
	case 3:
		timer = 55;
		timeLife = 125;
		break;
	case 4:
		timer = 50;
		timeLife = 100;
		break;

	}


	letter = new Letter;
	util = new Util;

	alphabet = letter->getAlphabet();		//generates an alphabet

	alphabet = util->shuffle(alphabet);		//shuffles the alphabet on random
	positionsOnDisplay = position->positionOnDisplay(); //generates positions 

	positionsOnDisplay = util->shufflePos(positionsOnDisplay); // shuffles positions on random

	playable = util->createGameList(alphabet, positionsOnDisplay, timeLife); //creates an object to assign a location for display and a letter

}


void Init()
{
	//load background image
	backgroundStats = blit3D->MakeSprite(0, 40, 420, 1000, "Media\\backgrounds\\bgStatsB.png");
	backgroundGame = blit3D->MakeSprite(620, 40, 1200, 1000, "Media\\backgrounds\\bgGameBoard.png");
	gameOverMessage = blit3D->MakeSprite(0, 0, 500, 281, "Media\\backgrounds\\gameOverMessage.png");
	BG = blit3D->MakeSprite(0, 0, 1920, 1080, "Media\\backgrounds\\BG.png");
	youwin = blit3D->MakeSprite(0, 0, 1920, 1080, "Media\\backgrounds\\youwin.png");

	//load an Angelcode binary32 font file
	pixeledWhite = blit3D->MakeAngelcodeFontFromBinary32("Media\\fonts\\pixeled\\pixeled40White.bin");
	time35white = blit3D->MakeAngelcodeFontFromBinary32("Media\\fonts\\time\\time35white.bin");
	//******************************************************************************************************************
	util->InitializeRNG();
	
	makeLevel();

}



void DeInit(void)
{
	// DO NOT delete the font!
	//If we need to delete a font at runtime, call DeleteFont().

	//any sprites/fonts still allocated are freed automatically by the Blit3D object when we destroy it
}

void Update(double seconds)
{

	if (splashScreenTime > 0)	//intro sprite
	{
		splashScreenTime--;
	}
	else
	{

		if (lives > 0)
		{
			frames++;
			if (frames >= timer / 2 && level == 4)
			{
				timer = 90;
				timeLife = 490;
			}

			if (currentIndex < 30)
			{
				if (frames >= timer)
				{
					playable.at(currentIndex).status = false;
					currentIndex++;
					frames = 0;
				}
			}
			else
			{
				bool stillShow = false;
				for (int i = 0; i < 30; i++)
				{
					if (!playable.at(i).status)
						stillShow = true;
				}
				if (!stillShow)
				{
					if (level < 4)
					{
						makeLevel();
						currentIndex = 0;
					}
					else
					{
						youwon = true;
					}
					
				}


			}


			for (int i = 0; i < 30; i++)
			{

				if (playable.at(i).status == false && playable.at(i).timer > 0)
				{
					playable.at(i).timer -= seconds;
					if (playable.at(i).letter.value == currentKeyPressed)
					{
						highScore += 10;
						playable.at(i).status = true;
						keyPressed = false;
					}
					else
					{
						if (playable.at(i).letter.value != currentKeyPressed && playable.at(i).status == false && keyPressed)
						{
							keyPressed = false;
						}
					}
				}

				if (playable.at(i).status == false && playable.at(i).timer == 0)
				{
					playable.at(i).status = true;
					lives--;
				}
			}
		}
		else
			gameOver = true;

		keyPressed = false;
	}
	
	//if enter key is pressed and is game over or won will start again
	if (currentKeyPressed == '0' && gameOver)
	{
		level = 0;
		lives = 6;
		highScore = 0;
		keyPressed = false;
		gameOver = false;
		youwon = false;
		makeLevel();
	}

}

void Draw(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	//clear colour: r,g,b,a 	
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(splashScreenTime > 0)
		BG->Blit(1920.f / 2, 1080.f / 2);
	else
	{
		//draw the background for the sections of the game/stats
		backgroundStats->Blit(620.f / 2, 1080.f / 2); 
		backgroundGame->Blit(1220.f, 1080.f / 2);
		pixeledWhite->BlitText(150, 750, "Score: " + std::to_string(highScore));
		pixeledWhite->BlitText(150, 650, "Life : " + std::to_string(lives));
		pixeledWhite->BlitText(150, 120, "Level : " + std::to_string(level));

			if (!gameOver)
			{
				for (int i = 0; i < 30; i++)
				{
					if (!playable.at(i).status) //if status true will be displayed
					{

						Letter t_letter = playable.at(i).letter;
						std::string t_value;
						t_value.assign(1, t_letter.value);
						spriteLetter = blit3D->MakeSprite(0, 0, 195, 195, t_letter.letra);
						spriteLetter->Blit(playable.at(i).position.gridPosition.x + 100, playable.at(i).position.gridPosition.y - 100);
						time35white->BlitText(playable.at(i).position.gridPosition.x + 80, playable.at(i).position.gridPosition.y - 180, std::to_string(((playable.at(i).timer) * 100) / timeLife) + "%");
					}
				}
			}
			else
			{
				gameOverMessage->Blit(1920.f / 2, 1080.f / 2);
				pixeledWhite->BlitText((1920.f / 2) - 450, (1080.f / 2) - 100, "Press Enter to play again");
				
			}
		
	
			if (youwon)
			{
				youwin->Blit(1920.f / 2, 1080.f / 2);
				//pixeledWhite->BlitText((1920.f / 2) - 450, (1080.f / 2) - 100, "Press Enter to play again");
			}
			
		
	}
	


}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence

	if (action == GLFW_RELEASE)
		keyPressed = true;

	if (action == GLFW_PRESS)
		currentKeyPressed = static_cast<char>(key);
	if (key == GLFW_KEY_ENTER)
		currentKeyPressed = '0';
}

int main(int argc, char *argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	blit3D = new Blit3D(Blit3DWindowModel::BORDERLESSFULLSCREEN_1080P, 640, 400);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}