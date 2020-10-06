#include <windows.h>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>//for printing
#include <vector> //for storing location of medicine cells
#include <thread> 

using namespace std;

void timer(int);
void keyboard(unsigned char key, int x, int y); //first argument is key press other two arguments are position of mouse
void generateInitialCancerCells();
void moveMedicineCells(int i, int j);
void cellCounter();
void simulation(int start);
void display();
void moveExtraMedicine();
void storeExtraMedicine(int i, int j);
vector<int> fourCoordinates;
vector<int> fiveCoordinates;
vector<int> sixCoordinates;
vector<int> sevenCoordinates;

//macros for grid
#define ROWS 3
#define COLUMNS 3

//macro for fps
#define FPS 30

//create a 2D array to hold the cell positions (initially all elements in the array are 0's which represent healthy cells)
int cell[ROWS][COLUMNS];

//add cancer cells (represented by integer 1) to 25% of the array
int totalCells = ROWS * COLUMNS;
int cancerCellCount = totalCells * 0.40;



void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0); //background color
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-0.5f, COLUMNS - 0.5f, -0.5f, ROWS - 0.5f); //left right bottom top
	//gluOrtho2D(-0.5f, ROWS - 0.5f, -0.5f, COLUMNS - 0.5f);
}

void generateInitialCancerCells()
{
	int count = 0;
	while (count != cancerCellCount)
	{
		int row = rand() % ROWS;
		int column = rand() % COLUMNS;
		cell[row][column] = 1;

		count++;
	}
}

void setInitialCellColor()
{
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {

			if (cell[i][j] == 0) { //if cell value is 0 change to green
				glPointSize(10.0f);
				glColor3f(0.0f, 0.5f, 0.0f); //green
				glBegin(GL_POINTS);
				glVertex2i(i, j);
				glEnd();
			}
			else if (cell[i][j] == 1) { //if cell value is 1 change to red
				glPointSize(10.0f);
				glColor3f(0.85f, 0.07f, 0.2f); //red
				glBegin(GL_POINTS);
				glVertex2i(i, j);
				glEnd();
			}
			else {					//if cell value is any other number change to yellow
				glPointSize(10.0f);
				glColor3f(1.0f, 1.0f, 0.0f); //yellow
				glBegin(GL_POINTS);
				glVertex2i(i, j);
				glEnd();
			}
		}
	}
}



void cellHealthyCheck(int x, int y)
{
	int medicineNeighbour = 0;


	if (cell[x][y] == 1) //if cell is equal to cancer cell
	{
		//check if enough neighbours are medicine cells
		for (int i = (x - 1); i < (x + 2); i++)
		{
			if (cell[i][y - 1] == 2 || cell[i][y - 1] == 3 || cell[i][y - 1] == 4 || cell[i][y - 1] == 5 || cell[i][y - 1] == 6 || cell[i][y - 1] == 7 || cell[i][y - 1] == 8 || cell[i][y - 1] == 9)
			{
				medicineNeighbour++;
			}
			if (cell[i][y + 1] == 2 || cell[i][y + 1] == 3 || cell[i][y + 1] == 4 || cell[i][y + 1] == 5 || cell[i][y + 1] == 6 || cell[i][y + 1] == 7 || cell[i][y + 1] == 8 || cell[i][y + 1] == 9)
			{
				medicineNeighbour++;
			}
		}
		if (cell[x - 1][y] == 2 || cell[x - 1][y] == 3 || cell[x - 1][y] == 4 || cell[x - 1][y] == 5 || cell[x - 1][y] == 6 || cell[x - 1][y] == 7 || cell[x - 1][y] == 8 || cell[x - 1][y] == 9)
		{
			medicineNeighbour++;
		}
		if (cell[x + 1][y] == 2 || cell[x + 1][y] == 3 || cell[x + 1][y] == 4 || cell[x + 1][y] == 5 || cell[x + 1][y] == 6 || cell[x + 1][y] == 7 || cell[x + 1][y] == 8 || cell[x + 1][y] == 9)
		{
			medicineNeighbour++;
		}

		if (medicineNeighbour > 5)
		{
			cell[x][y] = 0;
		}
	}
}

void cellCancerCheck(int x, int y)
{
	int cancerousNeighbours = 0;

	if (cell[x][y] == 0) //if cell is equal to healthy cell
	{
		//check if enough neighbours are cancer cells
		for (int i = (x - 1); i < (x + 2); i++)
		{
			if (cell[i][y - 1] == 1)
			{
				cancerousNeighbours++;
			}
			if (cell[i][y + 1] == 1)
			{
				cancerousNeighbours++;
			}
		}
		if (cell[x - 1][y] == 1)
		{
			cancerousNeighbours++;
		}
		if (cell[x + 1][y] == 1)
		{
			cancerousNeighbours++;
		}

		if (cancerousNeighbours > 5)
		{
			cell[x][y] = 1;
		}
	}


}



void display()
{
	//clear buffer at the begining of each frame
	glClear(GL_COLOR_BUFFER_BIT);

	setInitialCellColor(); //set cell color given its current value

	//Multi-threading
	std::thread thread1(simulation, ROWS / 3 * 0);
	std::thread thread2(simulation, ROWS / 3 * 1);
	std::thread thread3(simulation, ROWS / 3 * 2);
	
	//Wait for threads to finish
	thread1.join();
	thread2.join();
	thread3.join();

	moveExtraMedicine(); //move those specific extra medcine cells to their new cell
	
	cellCounter(); //count number of each type of cell, display results

	glutKeyboardFunc(keyboard); //check for user input

	glutSwapBuffers();
}


void simulation(int start)
{
	for (int i = start; i < start + (ROWS / 3); i++) {
		for (int j = 0; j < COLUMNS; j++) {

			cellCancerCheck(i, j); //check if current cell should become cancerous

			cellHealthyCheck(i, j); //check if current cell should become healthy

			moveMedicineCells(i, j); //move medicine Cells radially outwards

			//if a cell == 4 || 5 || 6 || 7  ,keep its cell location in a global storage
			storeExtraMedicine(i, j);

		}
	}
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(900, 900); //original 640 x 480
	glutCreateWindow("Assignment 1 - Cancer Cell Simulation");
	init(); //initialize background color and 2D ortho grid
	//generateInitialCancerCells(); //randomly generate initial cancer cells in 2D array

	glutDisplayFunc(display);

	glutTimerFunc(0, timer, 0); //first argument is when the first frame is displayed, function, int value passed to timer (only executes once)

	//cout << "printing main array" << endl;
	//for (int i = 0; i < ROWS; i++)
	//{
	//	for (int j = 0; j < COLUMNS; j++)
	//	{
	//		cout << cell[i][j]<<" ";
	//	}
	//}
	
	glutMainLoop();
	return 0;
}

void generateMedicineCells() //this only deels with number and not colors
{
	//genrate a random row and column
	int acceptableRow = ROWS - 2;  //if ROWS = 100, then rand() % acceptableRow go from 0 to 98 (instead of 99)
	int acceptableColumn = COLUMNS - 2;
	int row = rand() % acceptableRow + 2; //in the range 1 to 99 (if ROWS == 100)
	int column = rand() % acceptableColumn + 2;

	if (cell[row][column] == 1)	//if that cell is a cancer cell
	{
		cell[row][column] = 0; //change that cancer cell to a healthy cell
		//change all the cells around it into healthy cells
		cell[row - 1][column] = 0; //top 
		cell[row - 1][column + 1] = 0; //top right
		cell[row][column + 1] = 0; //right
		cell[row + 1][column + 1] = 0; //bottom right
		cell[row + 1][column] = 0; //bottom
		cell[row + 1][column - 1] = 0; //bottom left
		cell[row][column - 1] = 0; //left
		cell[row - 1][column - 1] = 0; //top left
	}
	else //if its a healthy cell or a medicine cell
	{
		//make a square around the cell into medicine cells
		cell[row - 1][column] = 2; //top 
		cell[row - 1][column + 1] = 3; //top right
		cell[row][column + 1] = 4; //right
		cell[row + 1][column + 1] = 5; //bottom right
		cell[row + 1][column] = 6; //bottom
		cell[row + 1][column - 1] = 7; //bottom left
		cell[row][column - 1] = 8; //left
		cell[row - 1][column - 1] = 9; //top left

	}
}

void moveMedicineCells(int i, int j)
{
	

	if (cell[i][j] == 2) //if cell is a 2, switch with cell above
	{
		if (i - 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1 ) //if to close to the end of array disappear
		{
			cell[i][j] = 0;
		}
		else {
			int temp = cell[i - 1][j];
			cell[i][j] = temp;
			cell[i - 1][j] = 2;
		}
	}

	else if (cell[i][j] == 3) 
	{
		if (i - 1 == 0 || j + 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
		{
			cell[i][j] = 0;
		}
		else {//move cell top-right diagonal
			int temp = cell[i - 1][j + 1];
			cell[i][j] = temp;
			cell[i - 1][j + 1] = 3;
		}
	}

	//else if (cell[i][j] == 4) //not working properly!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//{
	//	if (j + 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
	//	{
	//		cell[i][j] = 0;
	//	}
	//	else {//move cell right
	//		int temp = cell[i][j + 1];
	//		cell[i][j] = temp;
	//		cell[i][j + 1] = 4;
	//	}
	//
	//}
	//else if (cell[i][j] == 5) //not working properly!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//{
	//	if (i + 1 == 0 || j + 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
	//	{
	//		cell[i][j] = 0;
	//	}
	//	else {//move cell bottom-right diagonal
	//		int temp = cell[i + 1][j + 1];
	//		cell[i][j] = temp;
	//		cell[i + 1][j + 1] = 5;
	//	}
	//	
	//}
	//else if (cell[i][j] == 6) //not working properly!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//{
	//	if (i + 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
	//	{
	//		cell[i][j] = 0;
	//	}
	//	else {//move cell down
	//		int temp = cell[i + 1][j];
	//		cell[i][j] = temp;
	//		cell[i + 1][j] = 6;
	//	}
	//
	//}
	//else if (cell[i][j] == 7) //not working properly!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//{
	//	if (i + 1 == 0 || j - 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
	//	{
	//		cell[i][j] = 0;
	//	}
	//	else { //move cell bottom-left down
	//		int temp = cell[i + 1][j - 1];
	//		cell[i][j] = temp;
	//		cell[i + 1][j - 1] = 7;
	//	}
	//	
	//}

	else if (cell[i][j] == 8) 
	{
		if (i - 1 == 0 || j - 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
		{
			cell[i][j] = 0;
		}
		else {
			int temp = cell[i][j - 1];
			cell[i][j] = temp;
			cell[i][j - 1] = 8;
		}
	}

	else if (cell[i][j] == 9) 
	{
		if (i - 1 == 0 || j - 1 == 0 || i == 0 || j == 0 || i==ROWS - 1 || j==COLUMNS - 1)
		{
			cell[i][j] = 0;
		}
		else {
			int temp = cell[i - 1][j - 1];
			cell[i][j] = temp;
			cell[i - 1][j - 1] = 9;
		}
	}

}

void storeExtraMedicine(int i, int j)
{
	if (cell[i][j] == 4)
	{
		fourCoordinates.push_back(j);
		fourCoordinates.push_back(i);
	}
	else if (cell[i][j] == 5)
	{
		fiveCoordinates.push_back(j);
		fiveCoordinates.push_back(i);
	}
	else if (cell[i][j] == 6)
	{
		sixCoordinates.push_back(j);
		sixCoordinates.push_back(i);
	}
	else if (cell[i][j] == 7)
	{
		sevenCoordinates.push_back(j);
		sevenCoordinates.push_back(i);


	}
}

void moveExtraMedicine()
{
	while (!fourCoordinates.empty())
	{
		int i = fourCoordinates.back();
		fourCoordinates.pop_back();
		int j = fourCoordinates.back();
		fourCoordinates.pop_back();

		if (j + 1 == 0 || i + 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || i == ROWS - 2 || j == COLUMNS - 2 || j == COLUMNS - 1 || i == ROWS || j == COLUMNS) //if to close to the end of array disappear
		{
			cell[i][j] = 0;
		}
		else {//move cell right
			int temp = cell[i][j + 1];
			cell[i][j] = temp;
			cell[i][j + 1] = 4;
		}
	}

	while (!fiveCoordinates.empty())
	{
		int i = fiveCoordinates.back();
		fiveCoordinates.pop_back();
		int j = fiveCoordinates.back();
		fiveCoordinates.pop_back();

		if (i + 1 == 0 || j + 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
		{
			cell[i][j] = 0;
		}
		else {//move cell bottom-right diagonal
			int temp = cell[i + 1][j + 1];
			cell[i][j] = temp;
			cell[i + 1][j + 1] = 5;
		}
	}

	while (!sixCoordinates.empty())
	{
		int i = sixCoordinates.back();
		sixCoordinates.pop_back();
		int j = sixCoordinates.back();
		sixCoordinates.pop_back();

		if (i + 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
		{
			cell[i][j] = 0;
		}
		else {//move cell down
			int temp = cell[i + 1][j];
			cell[i][j] = temp;
			cell[i + 1][j] = 6;
		}
	}

	while (!sevenCoordinates.empty())
	{
		int i = sevenCoordinates.back();
		sevenCoordinates.pop_back();
		int j = sevenCoordinates.back();
		sevenCoordinates.pop_back();

		if (i + 1 == 0 || j - 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
		{
			cell[i][j] = 0;
		}
		else { //move cell bottom-left down
			int temp = cell[i + 1][j - 1];
			cell[i][j] = temp;
			cell[i + 1][j - 1] = 7;
		}

	}
}



void cellCounter()
{
	int healthyCells = 0;
	int cancerCells = 0;
	int medicineCells = 0;

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++)
		{
			if (cell[i][j] == 0)
			{
				healthyCells++;
			}
			else if (cell[i][j] == 1)
			{
				cancerCells++;
			}
			else
			{
				medicineCells++;
			}
		}
	}
	cout << "Healthy: " << healthyCells;
	cout << ", Cancer: " << cancerCells;
	cout << ", Medicine: " << medicineCells <<endl;

}

void keyboard(unsigned char key, int x, int y) //we dont need to know mouse position
{
	if (key == 27) // pressing escape key will end the program
	{
		exit(0);
	}

	if (key == 32) //pressing spacebar will generate a new random injection of medicine cells
	{
		generateMedicineCells(); //make random injection of medicine cells
	}
}

void timer(int)
{
	glutPostRedisplay(); //tells opengl to call display function
	//re call the glutTimerFunc found in main
	//we want 10 fps, 1s = 1000ms ,  1s / 10frames = 10 frames per second 
	glutTimerFunc(1000 / FPS, timer, 0);
}
