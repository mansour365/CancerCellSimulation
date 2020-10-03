#include <windows.h>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
//#include "Simulation.h" //Simulation file
#include <iostream>//for printing
#include <string> // to convert integer to string
#include <vector> //for storing location of cancer cells during initilization

using namespace std;

void timer(int);
void display();
void keyboard(unsigned char key, int x, int y); //first argument is key press other two arguments are position of mouse
void generateInitialCancerCells();
void generateMedicineCells();
void moveMedicineCells(int i, int j);
void cellCounter();

//macros for grid
#define ROWS 60
#define COLUMNS 60

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

//this should only be used for changing the values in array not for displaying
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

void setCellColor()
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

static void display()
{
	//clear buffer at the begining of each frame
	glClear(GL_COLOR_BUFFER_BIT);

	setCellColor(); //set cell color given its current value

	for (int i = 0; i < ROWS ; i++) {
		for (int j = 0; j < COLUMNS ; j++) {

			cellCancerCheck(i, j); //check if current cell should become cancerous

			cellHealthyCheck(i, j); //check if current cell should become healthy

			moveMedicineCells(i, j); //move medicine Cells radially outwards

		}
	}

	cellCounter(); //count number of each type of cell, display results

	glutKeyboardFunc(keyboard); //check for user input

	glutSwapBuffers();

}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(900, 900); //original 640 x 480
	glutCreateWindow("Assignment 1 - Cancer Cell Simulation");

	init(); //initialize background color and 2D ortho grid

	generateInitialCancerCells(); //randomly generate initial cancer cells in 2D array

	glutDisplayFunc(display);

	//registers a callback after a specified amount of ms have passed (here it only executes once)
	glutTimerFunc(0, timer, 0); //first argument is when the first frame is displayed, function, int value passed to timer

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
	//		cout << cell[i][j]<<" ";
	//	}
	//}
	
	glutMainLoop();
	return 0;
}

void moveMedicineCells(int i, int j)
{
	int lastTravelledMedicine = 0;

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
			lastTravelledMedicine = 2;
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
			lastTravelledMedicine = 3;
		}
	}

	else if (cell[i][j] == 4) //not working properly
	{
		if (j + 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
		{
			cell[i][j] = 0;
		}
		else {//move cell right
			if (lastTravelledMedicine == 4)
			{
				lastTravelledMedicine = 0;
			}
			else {
				int temp = cell[i][j + 1];
				cell[i][j] = temp;
				cell[i][j + 1] = 4;
				lastTravelledMedicine = 4;
			}
			
		}

	}
	else if (cell[i][j] == 5) //not working properly
	{
		if (i + 1 == 0 || j + 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
		{
			cell[i][j] = 0;
		}
		else {//move cell bottom-right diagonal
			if (lastTravelledMedicine == 5)
			{
				lastTravelledMedicine = 0;
			}
			else {
				int temp = cell[i + 1][j + 1];
				cell[i][j] = temp;
				cell[i + 1][j + 1] = 5;
				lastTravelledMedicine = 5;
			}
			
		}
		
	}
	else if (cell[i][j] == 6) //not working properly
	{
		if (i + 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
		{
			cell[i][j] = 0;
		}
		else {//move cell down
			if (lastTravelledMedicine == 6)
			{
				lastTravelledMedicine = 0;
			}
			else {
				int temp = cell[i + 1][j];
				cell[i][j] = temp;
				cell[i + 1][j] = 6;
				lastTravelledMedicine = 6;
			}

		}

	}
	else if (cell[i][j] == 7) //not working properly
	{
		if (i + 1 == 0 || j - 1 == 0 || i == 0 || j == 0 || i == ROWS - 1 || j == COLUMNS - 1) //if to close to the end of array disappear
		{
			cell[i][j] = 0;
		}
		else { //move cell bottom-left down
			if (lastTravelledMedicine == 7)
			{
				lastTravelledMedicine = 0;
			}
			else {
				int temp = cell[i + 1][j - 1];
				cell[i][j] = temp;
				cell[i + 1][j - 1] = 7;
				lastTravelledMedicine = 7;
			}

		}
		
	}
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
			lastTravelledMedicine = 8;
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
			lastTravelledMedicine = 9;
		}
	}

}

void generateMedicineCells() //this only deels with number and not colors
{
	//genrate a random row and column
	int acceptableRow = ROWS - 1;  //if ROWS = 100, then rand() % acceptableRow go from 0 to 98 (instead of 99)
	int acceptableColumn = COLUMNS - 1;
	int row = rand() % acceptableRow + 1; //in the range 1 to 99 (if ROWS == 100)
	int column = rand() % acceptableColumn + 1; 
	
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


//bool checkVector(string str)
//{
//	if (count(cancerCells.begin(), cancerCells.end(), str))
//		return false; //String already exists in the vector
//	else
//		return true;
//}

//void cancerGrowth()
//{
//	for (int i = 0; i < COLUMNS; i++)
//	{
//		for (int j = 0; j < ROWS; j++)
//		{
//			//for now skip first row, first column, last row, last column
//			int cancerProximityCounter = 0;
//			if ((cell[i][j] == 0) || (i = 0) || (j = 0) || (i = COLUMNS - 1) || (j = ROWS - 1))
//			{
//				continue; //hopefully this brings it back to the top of the inner loop
//			}
//			else
//			{
//				if (cell[i][j - 1] == 1) //left
//					cancerProximityCounter++;
//				if (cell[i - 1][j - 1] == 1) //top left
//					cancerProximityCounter++;
//				if (cell[i - 1][j] == 1) //top 
//					cancerProximityCounter++;
//				if (cell[i - 1][j + 1] == 1) //top right
//					cancerProximityCounter++;
//				if (cell[i][j + 1] == 1) //right
//					cancerProximityCounter++;
//				if (cell[i + 1][j + 1] == 1) //bottom right
//					cancerProximityCounter++;
//				if (cell[i + 1][j] == 1) //bottom
//					cancerProximityCounter++;
//				if (cell[i + 1][j - 1] == 1) //bottom left
//					cancerProximityCounter++;
//
//				if (cancerProximityCounter > 5)
//				{
//					cell[i][j] = 1;
//				}
//
//			}
//
//
//
//
//		}
//	}
//}

//initialized the grid with specific number of columns and rows
//void initializeScreen()
//{
//	//he used glClearColor(0,0,0,1) to set back ground tpo balc 
//	//might not actually need this function
//	initializeGrid(COLUMNS, ROWS);
//}

//called to setup the viewport(resize window) and to setup coordinate system

//string randomCellSelector()
//{
//	int v1 = rand() % ROWS;
//	int v2 = rand() % COLUMNS;
//	string str1 = to_string(v1);
//	string str2 = to_string(v2);
//	string str = str1 + str2;
//	return str;
//}



//void drawCells()
//{
//
//	for (int x = 0; x < COLUMNS; x++)
//	{
//		for (int y = 0; y < ROWS; y++)
//		{
//			if (cell[x][y] == 0) {
//				glColor3f(0.2, 0.75, 0.2);//color of the healthy cells
//				glRectd(x, y, x + 1, y + 1);  //(vertex coordinate, opposite vertex coordinate)
//			}
//			if (cell[x][y] == 1) {
//				glColor3f(0.5, 0.0, 0.0);//color of the healthy cells
//				glRectd(x, y, x + 1, y + 1);  //(vertex coordinate, opposite vertex coordinate)
//			}
//		}
//	}
//}

//void reshape_callback(int w, int h)
//{
//	//viewport is used when we resize the window
//	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //top left corner of windows is (0,0) , bottom right corner is (w,h).
//											  //The arguments take type GLsizei, so we need to cast the integer
//
//	//----------Setup the projection for coordinate system----------
//	//Switch to prjection matrix
//	glMatrixMode(GL_PROJECTION);
//	//Make sure no changes at the very beginning
//	glLoadIdentity();
//	//We will use orthographic projection because it is a 2D simulation (instead of perspective (3D))
//	glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0);//leftmost point , rightmost point, bottommost point, topmost point, znear, zfar
//	//Switch back to the modelview matrix
//	glMatrixMode(GL_MODELVIEW);
//
//}

//void changeColor(GLfloat red, GLfloat green, GLfloat blue) {
//	r = red;
//	g = green;
//	b = blue;
//}

//Check status of individual cell and apply the game rules.
//static boolean checkStatus(int x, int y) {
//	int cancerNeighbour = 0;
//	bool status = false;
//
//	if (cell[x][y] == 2 || cell[x][y] == 3 || cell[x][y] == 4 || cell[x][y] == 5 || cell[x][y] == 6 || cell[x][y] == 7 || cell[x][y] == 8 || cell[x][y] == 9) 
//	{
//		status = false;
//	}
//	else 
//	{
//		for (int i = (x - 1); i < (x + 2); i++)
//		{
//			if (cell[i][y - 1] == true)
//			{
//				cancerNeighbour++;
//			}
//			if (cell[i][y + 1] == true)
//			{
//				cancerNeighbour++;
//			}
//		}
//		if (cell[x - 1][y] == true)
//		{
//			cancerNeighbour++;
//		}
//		if (cell[x + 1][y] == true)
//		{
//			cancerNeighbour++;
//		}
//
//		if (cancerNeighbour > 5)
//		{
//			status = true;
//		}
//	}
//
//	return status;
//}