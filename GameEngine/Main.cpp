#include "Window.h"
#include "Lab1.h"
#include "Lab2.h"

int main()
{
	Lab1 *game;

	game = new Lab1();
	game->init();
	game->run();
}