#include "Window.h"
#include "Lab1.h"
#include "Lab2.h"
#include "Lab3.h"
#include "RenderingLab1.h"

int main(int argc, char **argv)
{

	Lab3 *game;

	game = new Lab3();
	game->init(argv);
	game->run();

	//RenderingLab1 *game;

	//game = new RenderingLab1();
	//game->init(argv);
	//game->run();
}