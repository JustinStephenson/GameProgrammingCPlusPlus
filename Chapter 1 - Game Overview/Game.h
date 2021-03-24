#pragma once

#include <stdint.h>
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

	// input for the paddle
	int mPaddleDir;
	// paddle height
	const int paddleH = 100;

	// for now
	const int thickness = 15;

	typedef struct Vector2 {
		float x;
		float y;
	} Vector2;
	Vector2 mPaddlePos;
	Vector2 mBallPos;
	Vector2 mBallVel;
};