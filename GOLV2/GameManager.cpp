#pragma once
#include "GameManager.h"

	int squareSize = 15;
	int squareBufferSize = 2;

	GameManager::GameManager(bool** startingMap, int rows, int cols) 
	{
        map = new bool*[rows];
        for (int i = 0; i < rows; ++i) {
            map[i] = new bool[cols];
            for (int j = 0; j < cols; ++j) {
                map[i][j] = startingMap[i][j];  // Copy values
            }
        }
        this->rows = rows;
        this->cols = cols;
	}

	// Destructor to free the dynamically allocated memory
	GameManager::~GameManager() {
		// Free each row first
		for (int i = 0; i < rows; ++i) {
			delete[] map[i];
		}

		// Then free the array of pointers (the rows)
		delete[] map;
	}


	void GameManager::CheckClickCollision(int w, int h, SDL_Renderer* renderer) 
	{
		std::cout << "Click Collision Called" << "x - " << w << "y - " << h << std::endl;

		int currentYPosition = 0;
		int currentXPosition = 0;


		for(int x = 0; x < rows; x++) {
			currentXPosition = 0;

			for(int y = 0; y < cols; y++) {

				if( w >= currentXPosition && w < currentXPosition + squareSize && h >= currentYPosition && h <= currentYPosition + squareSize) 
				{
					//Click happened in this square
					bool populatedValue = map[x][y];

					SDL_Rect square;
					square.w = squareSize;  // Width of the square
					square.h = squareSize;  // Height of the 
					square.x = currentXPosition;  // X position
					square.y = currentYPosition;  // Y position

					if(populatedValue) 
					{
						std::cout << "Clicked already alive square - killing it" << std::endl;

						map[x][y] = false;
						SDL_SetRenderDrawColor(renderer, 0, 0,0, 255);  // Set to white
					}
					else 
					{
						std::cout << "Clicked dead square - reviving it" << std::endl;
						map[x][y] = true;
						SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Set to red
					}

					SDL_RenderFillRect(renderer, &square);
				}

				currentXPosition += squareSize + squareBufferSize;
			}

			currentYPosition += squareSize + squareBufferSize;
		}
	}

	void GameManager::DrawMap(SDL_Renderer* renderer) 
	{
		//std::cout << "Drawing Map" << std::endl;

		int currentXPosition = 0;
		int currentYPosition = 0;

		SDL_Rect square;

		square.w = squareSize;  // Width of the square
		square.h = squareSize;  // Height of the square

		for(int x = 0; x < rows; x++) {
			currentXPosition = 0;

			for(int y = 0; y < cols; y++) {
					
				if(map[x][y]) 
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Set to red
				else 
					SDL_SetRenderDrawColor(renderer, 0, 0,0, 255);  // Set to white

				square.x = currentXPosition;  // X position
				square.y = currentYPosition;  // Y position

				//DRAW
				SDL_RenderFillRect(renderer, &square);

				currentXPosition += squareSize + squareBufferSize;
			}

			currentYPosition += squareSize + squareBufferSize;
		}
	}

	void GameManager::EnforceRules(SDL_Renderer* renderer) 
	{
		//std::cout << "Enforcing Rules" << std::endl;

		for(int x = 0; x < rows; x++) {
			for(int y = 0; y < cols; y++) {
				int numberOfAliveNeighbors = CheckNeighbors(x, y);
					
				if(map[x][y]) 
				{
					//If cell is populated
					if(numberOfAliveNeighbors < 2) 
					{
						//die by solitude
						map[x][y] = false;
					}
					else if(numberOfAliveNeighbors >= 4) 
					{
						//Death by over population
						map[x][y] = false;
					}
					else if(numberOfAliveNeighbors >= 3 && numberOfAliveNeighbors <= 4) 
					{
						map[x][y] = true;
					}
				}
				else 
				{
					if(numberOfAliveNeighbors == 3) 
					{
						map[x][y] = true;
					}
					else 
					{
						map[x][y] = false;
					}
				}
			}
		}
	}

	int GameManager::CheckNeighbors(int x, int y) {

		int aliveNeighbors = 0;

		if(x > 0) 
		{
			//Check Above Neighbor
			aliveNeighbors += map[x-1][y];
		}

		if(x < rows-1) 
		{
			//Check Bottom Neighbor
			aliveNeighbors += map[x+1][y];
		}

		if(y < cols-1) 
		{
			//Check Right Neighbor
			aliveNeighbors += map[x][y+1];
		}

		if(y > 0) 
		{
			//Check Left Neighbor
			aliveNeighbors += map[x][y-1];
		}

		if(x > 0 && y < cols-1)  
		{
			//Check top-right
			aliveNeighbors += map[x-1][y+1];
		}

		if(x > 0 && y > 0)  
		{
			//Check top-left
			aliveNeighbors += map[x-1][y-1];
		}

		if(x < rows-1 && y < cols-1)  
		{
			//Check bottom-right
			aliveNeighbors += map[x+1][y+1];
		}

		if(x < rows-1 && y > 0)  
		{
			//Check bottom-left
			aliveNeighbors += map[x+1][y-1];
		}

		return aliveNeighbors;
	}


