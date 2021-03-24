#include "Game.h"
#include <SDL.h>
#include <stdio.h>
#include <cstdlib>

Game::Game() {
	mWindow = nullptr;
	mIsRunning = true;
	mRenderer = nullptr;
	mTicksCount = 0;

	// input
	mPaddleDir = 0;

	// for now
	mPaddlePos = { 20, 342 };
	mBallPos = { 512, 384 };
	mBallVel = { -200.0f, 235.0f };
}

bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	mWindow = SDL_CreateWindow(
		"Game programming in C++ (Chapter 1)",	// Window title
		400,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	mRenderer = SDL_CreateRenderer(
		mWindow,	// Window to create renderer for
		-1,			// Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown() {
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::ProcessInput() {
	// Handle window based events (like resizing)
	SDL_Event event = SDL_Event();
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			// Handle different event types here
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}

	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// if escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mPaddleDir += 1;
	}
}

void Game::UpdateGame() {
	// Wait until 16ms has elsapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Clamp the maximum delta time value
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// Updates paddle
	if (mPaddleDir != 0) {
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen
		if (mPaddlePos.y < (paddleH / 2.0f + thickness)) {
			mPaddlePos.y = paddleH / 2.0f + thickness;
		} 
		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness)) {
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// Updates ball
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;
	
	// Ball top wall collision
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
		mBallVel.y *= -1;
	}
	// Ball bottom wall collision
	if (mBallPos.y >= 768 - thickness && mBallVel.y > 0.0f) {
		mBallVel.y *= -1;
	}
	// Ball right wall collision
	if (mBallPos.x >= 1024 - thickness && mBallVel.x > 0.0f) {
		mBallVel.x *= -1;
	}

	// Ball paddle collision
	int diff = abs(mBallPos.y - mPaddlePos.y);
	if (
		// Our y-difference is small enough
		diff <= paddleH / 2 &&
		// Ball is in correct x-position
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// The ball is moving to the left
		mBallVel.x < 0.0f) {
		mBallVel.x *= -1;
	}
}

void Game::GenerateOutput() {
	// Sets screen to blue, then clear buffer
	SDL_SetRenderDrawColor(
		mRenderer,
		0,			// R
		0,			// G
		255,		// B
		255			// A
	);
	SDL_RenderClear(mRenderer);
	
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	// Draw rectangle on screen, top
	SDL_Rect topWall{
		0,			// Top Left x
		0,			// Top Left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &topWall);

	// right-side
	SDL_Rect rightWall{
		1024 - thickness,
		0,
		thickness,
		768
	};
	SDL_RenderFillRect(mRenderer, &rightWall);

	// bottom
	SDL_Rect bottomWall{
		0,
		768 - thickness,			
		1024,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &bottomWall);

	// Draw paddle on screen
	SDL_Rect paddle{
		static_cast<int> (mPaddlePos.x - thickness / 2),
		static_cast<int> (mPaddlePos.y - paddleH / 2),
		thickness,
		paddleH
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	// Draw ball on screen (ball shape rectangle)
	SDL_Rect ball{
		static_cast<int> (mBallPos.x - thickness / 2),
		static_cast<int> (mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	// Swap the front and back buffers
	SDL_RenderPresent(mRenderer);
}