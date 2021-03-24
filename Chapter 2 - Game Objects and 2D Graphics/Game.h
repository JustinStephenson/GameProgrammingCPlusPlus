#pragma once

#include <stdint.h>
#include <vector>
class SDL_Window;
class SDL_Renderer;

class Game
{
public:
	Game();
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();

	// Actor functions
	void AddActor(class Actor*);
	void RemoveActor(class Actor*);
private:
	// Helper functions foe the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer
	SDL_Renderer* mRenderer;
	// Game should continue to run
	bool mIsRunning;
	// Keep track of number of ticks
	uint32_t mTicksCount;

	// Actor variables
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	bool mUpdatingActors;
};