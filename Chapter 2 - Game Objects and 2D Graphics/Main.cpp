#include "Game.h"

int main(int argc, char** argv) {
	Game game = Game();
	bool success = game.Initialize();
	if (success) {
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}