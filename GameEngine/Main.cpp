#include "Window.h"
#include "Lab1.h"
#include "Lab2.h"

int main(int argc, char **argv)
{
	Lab2 *game;

	game = new Lab2();
	game->init(argv);
	game->run();
}