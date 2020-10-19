# CancerCellSimulation

Class COMP 426 – NN
Instructor: Professor R. Jayakumar
Student ID: 27739656
Name: Said-Mansour Maqsoudi
Assignment 1 Documentation


Design Documentation


Goal of the assignment
This assignment required to implementation of a cancer cell simulation. There are three types of cells and each cell has its own properties. A green cell is a healthy cell, and will become a cancer cell if it is surrounded by at least six cancer cells. A red cell is a cancer cell. A cancer cell will become a healthy cell if it is surrounded by at least six medicine cells. A medicine cell is yellow. Medicine cells are injected in the soup of healthy and cancerous cells. Medicine cells spawn in a group of one to eight cells that surround a random cell. If surrounded with enough of these medicine cells a cancer cell will be cured into a healthy cell. Otherwise if the cell is a healthy cell then the medicine cells will radiate outwards in a circular pattern around the targeted cell.

My implementation
In order to implement such a system of cells, I opted to use a two-dimensional array as the main way to store all the cells. By doing this I could create a grid of integer values, where each type of cell could be represented by an integer. This 2D array would then be translated into an OpenGL grid. The reason I decided to make this array of type integer,  was so that integers 0 through 9 could represent a different type of cell. If an element in the array was a 0, that would mean that it is a healthy cell. If a 1 was stored that would mean the cell is a cancer cell. Then the eight numbers left, (number 2 to 9) would represent the medicine cells that have differing motion. 2 would be a medicine cell that moves upwards, 3 would move diagonally in the top/right direction, 4 would move to the right, 5 would move diagonally in the bottom-right direction, 6 would move downwards, 7 would move diagonally in the bottom-left direction, 8 would move to the left, and finally 9 would only move diagonally in the top-left direction. All eight of these medicine cells could surround a single cell as depicted in the illustration below.

Medicine Cells				Healthy cell			Cancer Cell
9
2
3

0

1
8

4
7
6
5

Implementing the grid
In order to make this grid I first needed to setup a coordinate system. In OpenGL you can setup a coordinate system by first switching to a projection matrix. This is done by using the code glMatrixMode() and using GL_PROJECTION as an argument. Because this simulation will be in two dimensions rather than 3 dimensions I needed to set it to an orthographic projection instead of a perspective projection. To use the orthographic projection, I needed to input several arguments that included where the left most and right most parts of the screen were, as well as where the bottom most and top most parts of the screen where. I set the left most part to 0, and the bottom most part to 0, since this makes more sense in terms of a grid. The right most and top most parts I set to a macro using #define that assigned the number of columns and rows.

Implementing control over frames per second

The first step to control how many frames per second I could achieve was to create a macro constant called FPS using C++’s #define and set that constant to 30. Next I used the built in function called glutTimerFunc() in the main() method. glutTimerFunc() registers a call back function when a certain amount of time has passed. We set the first argument to 0, because initially we want it to execute right away. The second argument we put a custom function called timer() which is implement outside the main(). When timer is requested it calls glutPostRedisplay() which tells OpenGL to re call the display function. Next, inside timer we call the glutTimer() function found in main. However this time since we want 30 frames per second we know that it is 1 second / 30 frames. So for the first argument we pass 1000 (ms) / FPS . 1000ms since that is precisely 1 second. This will make it recall the display function 30 times in one second. Therefore because at the end of the display function we swap buffers this will ensure that it will display new frames at the appropriate speed.





Keyboard inputs

In main() I  added a new function called keyboard(). This function takes three arguments, first argument is a key that the user presses, the other two arguments are for mouse position. Since I will not be using the mouse in my implementation I did not need to worry about those arguments. Inside this function I can check if the user has pressed the escape key. This function requires the ASCII code for the escape key which is 27. So if key == 27 then I can run  the program termination code which is exit(0). In order to check for input every frame I have included a built in function called glutKeyboardFunc() and pass keyboard function as an argument. This ensures that the program is checking for user input every frame. Later I also assigned the function to check for asci code 32 which is the spacebar. This intended to be the key pressed when a user wants to make a medicine cell injection on a random cell.














Medicine cells
I opted to have medicine cells spawn in a random location when I press the space bar. This gave me more unusual ways to see if the proper behavior was achieved. The final product was to have a working cell simulator that followed the given rules and at the same time have multithreading, such that each thread would work on a specific portion of the grid. In order to move the medicine cells, I opted to create a stack for each type of medicine cell. Namely medicine cells that travel up go into one stack, medicine cells that travel right go into another stack, etc. More precisely the location on the grid (coordinates) are stored in their own designated stack. Then every frame these coordinates get updated and this is reflected in the display. 
Each type of medicine cell is stored in its own designated stack.
2: move up stack
3: move top-right diagonal stack
4: move right stack
5: move bottom-right diagonal stack
6: move down stack
7: move bottom-left diagonal stack
8: move left stack
9: move top-left diagonal stack

For a total of nine different stacks.
Later these coordinates could easily be popped from the stack and updated to ensure that the medicine cells were moving in the proper direction.

Important/ Difficult parts 
The most important and difficult part of the assignment was being able to achieve multithreading in this environment. For this part of the assignment because I was using C++ in visual studio I opted to use the <thread> library in order to implement the multithreading. Initially, I aimed to have the main thread as the control thread where I would create each thread that would be used on a specific portion of the grid. Therefore, if I were to create 3 computational threads then that would imply that one thread would be working solely on one third of the grid. Naturally the work to divide up between the threads would be the display() method. This display() method however had to be run in main() as an argument to the function glutDisplayFunc(). For me this presented a problem. When creating a thread, you can pass a method and a parameter as arguments. However, this meant that I could only pass glutDisplayFunc as a method and display as an argument. But doing this threw out the possibility of giving display any arguments, which meant that I could not multithread the display function. To deal with this issue I decided to pull out the core methods inside the display method and put them into a separate method called simulation. From then on I could call the display function normally from main(), but this time I would create the threads within the display function. After doing this the multithreading worked properly and each thread was doing computation for a third of the grid. Therefore, my display function became the control thread while the computational threads dealt with the simulation function.
Testing
The first test scenario I designed was to check if the medicine cells were correctly converting a cancer cell into a healthy cell. To do this I manually created a 3 x 3 grid and manually placed 8 yellow cells around 1 red cell. Running the program makes every cell turn green which means that the program was working correctly. I repeated this test with 6, 7, and 9 medicine cells where the results were the same and every cell turned green. Choosing to place less than 6 medicine cells resulted in the targeted cell not changing, which is the desired outcome.

The second test scenario involved testing if a healthy cell would turn cancerous with the proper amount of cancer cells around it. This test was done in the same fashion as the original test scenario except this time I surrounded a single healthy cell with cancerous cells. The results of these tests once again shows that the cell transformation work properly with various number of surrounding cancer cells.

Various testing procedures were carried throughout the creation of the program to ensure that it functions reliably and as intended. 

