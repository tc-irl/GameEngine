#pragma once
#include "Game.h"

class Lab2 : public Game
{
public:
	Lab2(void);
	~Lab2(void);

	void run() override;
	void init() override;
	void initShaders() override;
};

