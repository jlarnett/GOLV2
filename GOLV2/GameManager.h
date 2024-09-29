#include <SDL.h>
#include <iostream>
#include <vector>

class GameManager {

	public:
		// Constructor to accept dynamic 2D array
		GameManager(bool** startingMap, int rows, int cols);
		void EnforceRules(SDL_Renderer* renderer);
		void CheckClickCollision(int x, int y, SDL_Renderer* renderer);
		void DrawMap(SDL_Renderer* renderer);
		~GameManager();
	private:
		int CheckNeighbors(int x, int y);
		bool** map;  // Pointer to dynamically allocated 2D array
		int rows, cols;  // Store the dimensions of the array


};


