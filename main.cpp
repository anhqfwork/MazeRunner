#include "Game.h"

//ZOOOOOOOOOOOM IN BEFORE RECORDING!

int main()
{
	Game game("MAZE RUNNER 3D GAME",
		800, 800,
		4, 4,
		false);

	//MAIN LOOP
	while (!game.getWindowShouldClose())
	{
		//UPDATE INPUT ---
		game.update();
		game.render();
	}

	return 0;
}