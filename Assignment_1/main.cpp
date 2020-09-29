#include <windows.h>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include "Simulation.h" //Simulation file

#include <iostream>//for printing
#include <string> // to convert integer to string
#include <vector> //for storing location of cancer cells during initilization

//macros for grid
#define COLUMNS 50
#define ROWS 50
//macro for fps
#define FPS 10

//access the sDirection found in Simulation.cpp
extern int sDirection;

using namespace std;

bool initialize;
double first_x = 0.0;
double first_y = 0.0;

//create a 2D array to hold the cell positions (initially all elements in the array are 0's which represent healthy cells)
int cell_array[ROWS][COLUMNS];

//vector to hold the strings of cancer cell location for initialization
vector<string> cancerCells;

//add cancer cells (represented by integer 1) to 25% of the array
int totalCells = COLUMNS * ROWS;
int cancerCellCount = totalCells * 0.25;

//create a random number generator from 0 to total cells
//then it starts picking numbers
//lets say it pick 67, so put at 1 at 67th index of array
//if same random number is picked again we redo it, 
//this whole process repeats 

/////////////////////////////////////////////////////////
//row (9) = 98/10 //get the row number
//column =  98 - (9 * 10) //get the column number
//
//cell_array[row][column] = 1;
//////////////////////////////////////////////////////////
void timer(int);
void display();
void reshape_callback(int, int);
void drawCells();
void keyboard(unsigned char key, int x, int y); //first argument is key press other two arguments are position of mouse
string randomCellSelector();
bool checkVector(string str);
void generateInitialCancerCells();

//initialized the grid with specific number of columns and rows
void initializeScreen()
{
	//he used glClearColor(0,0,0,1) to set back ground tpo balc 
	//might not actually need this function
	initializeGrid(COLUMNS, ROWS);
}



//int index = 0;
void display()
{
	//clear buffer at the begining of each frame
	glClear(GL_COLOR_BUFFER_BIT);




	drawCells();

	//draw grid every frame
	drawGrid();


	//drawSnake();

	//glRectd(index, 20, index + 1, 21); // (vertex , opposite vertex)
	//index++;
	//if (index > 40)
	//{
	//	index = 0;
	//}
	//display the contents of the array
	//for (int row = 0; row < 10; row++)
	//{
	//	for (int column = 0; column < 10; column++)
	//	{
	//		std::cout << cell_array[row][column] << ' ';
	//	}
	//}

	//initialize the cells
	/*while(initialize == true) {*/
		//create a 10 x 10 cells
		//for (int row = 0; row < 10; row++)
		//{
		//	for (int column = 0; column < 10; column++)
		//	{
		//		glTranslatef(0.0f + 0.02 * column, 0.0f + 0.02 * row, 0.0f);
		//		glBegin(GL_TRIANGLES);
		//		glColor3f(0.0f, 1.0f, 0.0f);

		//		glVertex3f(0.0f, 0.01f, 0.0f); //top point
		//		glVertex3f(-0.01f, -0.01f, 0.0f); //left point
		//		glVertex3f(0.01f, -0.01f, 0.0f); //right point

		//		glEnd();
		//	}
		//}
	/*	initialize == false;
	}*/

	//small triangle
	//glTranslatef(0.0f, 0.0f, 0.0f);
	//glBegin(GL_TRIANGLES);
	//glColor3f(0.0f, 1.0f, 0.0f);

	//glVertex3f(0.0f, 0.01f, 0.0f); //top point
	//glVertex3f(-0.01f, -0.01f, 0.0f); //left point
	//glVertex3f(0.01f, -0.01f, 0.0f); //right point

	//glEnd();


	//small triangle 2
	//glTranslatef(first_x + 0.03, first_y, 0.0);
	//glBegin(GL_TRIANGLES);
	//glColor3f(0.0f, 1.0f, 0.0f);

	//glVertex3f(0.0f, 0.01f, 0.0f); //top point
	//glVertex3f(-0.01f, -0.01f, 0.0f); //left point
	//glVertex3f(0.01f, -0.01f, 0.0f); //right point

	//glEnd();

	//Call swap buffers at end of display
	glutSwapBuffers();
	glutKeyboardFunc(keyboard);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800); //original 640 x 480
	glutCreateWindow("Assignment 1 - Cancer Cell Simulation");
	glutDisplayFunc(display);

	glutReshapeFunc(reshape_callback); //added myself (this does resizing and sets up orthograpic space for a proper grid)

	//registers a callback after a specified amount of ms have passed (here it only executes once)
	glutTimerFunc(0, timer, 0); //first argument is when the first frame is displayed, function, int value passed to timer

	//glutSpecialFunc(keyboard_callback);

	initializeScreen();//added this

	generateInitialCancerCells();


	//cout << totalCells << endl;
	//cout << cancerCellCount << endl;
	//for (auto it = cancerCells.cbegin(); it != cancerCells.cend(); it++)
	//{
	//	cout << *it << ' ';
	//}

	//cout << "printing main array" << endl;
	//for (int i = 0; i < ROWS; i++)
	//{
	//	for (int j = 0; j < COLUMNS; j++)
	//	{
	//		cout << cell_array[i][j] << endl;
	//	}
	//}

	glutMainLoop();
	return 0;
}


//called to setup the viewport(resize window) and to setup coordinate system
void reshape_callback(int w, int h)
{
	//viewport is used when we resize the window
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //top left corner of windows is (0,0) , bottom right corner is (w,h).
											  //The arguments take type GLsizei, so we need to cast the integer

	//----------Setup the projection for coordinate system----------
	//Switch to prjection matrix
	glMatrixMode(GL_PROJECTION);
	//Make sure no changes at the very beginning
	glLoadIdentity();
	//We will use orthographic projection because it is a 2D simulation (instead of perspective (3D))
	glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0);//leftmost point , rightmost point, bottommost point, topmost point, znear, zfar
	//Switch back to the modelview matrix
	glMatrixMode(GL_MODELVIEW);

}

void timer(int)
{
	glutPostRedisplay(); //tells opengl to call display function
	//re call the glutTimerFunc found in main
	//we want 10 fps, 1s = 1000ms ,  1s / 10frames = 10 frames per second 
	glutTimerFunc(1000 / FPS, timer, 0);
}


string randomCellSelector()
{
	int v1 = rand() % ROWS;
	int v2 = rand() % COLUMNS;
	string str1 = to_string(v1);
	string str2 = to_string(v2);
	string str = str1 + str2;
	return str;
}



bool checkVector(string str)
{
	if (count(cancerCells.begin(), cancerCells.end(), str))
		return false; //String already exists in the vector
	else
		return true;
}



void generateInitialCancerCells()
{
	int count = 0;
	while (count != cancerCellCount)
	{
		//string str = randomCellSelector();
		int row = rand() % ROWS;
		int column = rand() % COLUMNS;
		string str1 = to_string(row);
		string str2 = to_string(column);
		string str = str1 + str2;

		if (checkVector(str) == false)
		{
			continue;
		}
		else
		{
			cancerCells.push_back(str);
			cell_array[row][column] = 1;
			count++;
		}

	}
	
	
}



void keyboard(unsigned char key, int x, int y) //we dont need to know mouse position
{
	if (key == 27)
	{
		exit(0);
	}

	if (key == 32)
	{
		//make random injection around a cancer cell
	}
} 

void drawCells()
{
	
	for (int x = 0; x < COLUMNS; x++)
	{
		for (int y = 0; y < ROWS; y++)
		{
			if (cell_array[x][y] == 0) {
				glColor3f(0.2, 0.75, 0.2);//color of the healthy cells
				glRectd(x, y, x + 1, y + 1);  //(vertex coordinate, opposite vertex coordinate)
			}
			if (cell_array[x][y] == 1) {
				glColor3f(0.5, 0.0, 0.0);//color of the healthy cells
				glRectd(x, y, x + 1, y + 1);  //(vertex coordinate, opposite vertex coordinate)
			}
		}
	}
}

void cancerGrowth()
{
	for (int i = 0; i < COLUMNS; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			//for now skip first row, first column, last row, last column
			int cancerProximityCounter = 0;
			if (cell_array[i][j] == 0)
			{
				continue;
			}
			else
			{
				if (cell_array[i][j - 1] == 1) //left
					cancerProximityCounter++;
				if (cell_array[i - 1][j - 1] == 1) //top left
					cancerProximityCounter++;
				if (cell_array[i - 1][j] == 1) //top 
					cancerProximityCounter++;
				if (cell_array[i - 1][j + 1] == 1) //top right
					cancerProximityCounter++;
				if (cell_array[i][j + 1] == 1) //right
					cancerProximityCounter++;
				if (cell_array[i + 1][j + 1] == 1) //bottom right
					cancerProximityCounter++;
				if (cell_array[i + 1][j] == 1) //bottom
					cancerProximityCounter++;
				if (cell_array[i + 1][j - 1] == 1) //bottom left
					cancerProximityCounter++;

				if (cancerProximityCounter > 5)
				{
					cell_array[i][j] = 1;
				}
				
			}




		}
	}
}