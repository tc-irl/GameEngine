#include "Window.h"
#include "Lab1.h"
#include "Lab2.h"
#include "Lab3.h"
#include "Lab4.h"
#include "ClothSimulation.h"
#include "RenderingLab1.h"
#include "RenderingLab2.h"
#include "RenderingLab3.h"
#include "LoaderLab.h"

int main(int argc, char **argv)
{
	//Lab4 *game;

	//game = new Lab4();
	//game->init(argv);
	//game->run();

	//RenderingLab3 *game;

	//game = new RenderingLab3();
	//game->init(argv);
	//game->run();

	ClothSimulation *game;

	game = new ClothSimulation();
	game->init(argv);
	game->run();
}