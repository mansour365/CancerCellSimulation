#include "Simulation.h"

#include <windows.h>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

int gridX;
int gridY;

//variable for direction of snake
int sDirection = RIGHT;

//keep track of poistion of snake (initial position of snake is (20,20) center of screen)
int posX = 20;
int posY = 20;


//Function to initialize the grid
void initializeGrid(int x, int y)
{
	gridX = x;
	gridY = y;
}

//here we draw the square shape multiple times to form a grid
void drawGrid()
{
	for (int x = 0; x < gridX; x++)
	{
		for (int y = 0; y < gridY; y++)
		{
			unit(x, y);
		}
	}
}



//function to draw a single square unit
void unit(int x, int y)
{
	glLineWidth(1.0); //width of the line
	glColor3f(0.5, 0.5, 0.5); //color of the lines (grey)

	//check if the square is at the edge of the screen
	if (x == 0 || y == 0 || x == gridX - 1 || y == gridY - 1)
	{
		glLineWidth(3.0);
	}

	glBegin(GL_LINE_LOOP); //begin drawing
	glVertex2f(x, y); //bottom left vertex
	glVertex2f(x + 1, y); //bottom right vertex
	glVertex2f(x + 1, y + 1); //top right vertex
	glVertex2f(x, y + 1); //top left vertex
	glEnd(); //end drawing
}

void drawSnake()
{
	glRectd(posX, posY, posX + 1, posY + 1);
}