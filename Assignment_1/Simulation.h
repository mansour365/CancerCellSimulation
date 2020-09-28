#pragma once
#include <windows.h>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"



//define constants fpr the snake movement
#define UP 1
#define DOWN -1
#define RIGHT 2
#define LEFT -2

void initializeGrid(int, int);
void drawGrid();
void unit(int, int);
void drawSnake();