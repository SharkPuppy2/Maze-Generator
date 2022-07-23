#include <iostream>
#include<vector>
#include<set>
#include<unordered_set>
#include<Windows.h>
#include<stack>
#include<utility>
#include<iterator>
#include<time.h>

using namespace std;
const int rows = 30;
const int cols = 60;
int nScreenWidth = 120;
int nScreenHeight = 60;
stack<pair<int, int>> ans;

// Function to create the maze
void createMaze(char maze[rows][cols]) 
{	
	stack<pair<int, int>> pos; //Stack to hold the positions at every step 
	set <pair<int, int>> visited; //Stack to hold already visited boxes
	pair<int, int> current(1, 1); //current position
	int neighbors;
	maze[1][1] = ' ';
	pos.push(current);

	//initializing the maze to # - maze consists of only walls
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			maze[i][j] = '#';

	// Backtracking to create the path between the walls.
	while (!pos.empty()) 
	{
		set <pair<int, int>> neighbor_set; //set of pair to hold all position of all neighbors.
		int i = current.first;
		int j = current.second;
		visited.insert(current); //current box has been visited
		neighbors = 0;

		// Positions of top, down, left and right boxes to current
		pair<int, int> top(i - 1, j);
		pair<int, int> down(i + 1, j);
		pair<int, int> left(i, j - 1);
		pair<int, int> right(i, j + 1);

		//top box 
		if (i - 2 >= 0)
			if (visited.find(top) == visited.end()) {
				if (maze[i - 2][j] != ' ' && maze[i - 1][j - 1] != ' ' && maze[i - 1][j + 1] != ' ') {
					neighbors++;
					neighbor_set.insert(top);
				}
			}

		//down box
		if (i + 2 < rows)
			if (visited.find(down) == visited.end()) {
				if (maze[i + 2][j] != ' ' && maze[i + 1][j - 1] != ' ' && maze[i + 1][j + 1] != ' ') {
					neighbors++;
					neighbor_set.insert(down);
				}
			}

		//right box
		if (j + 2 < cols)
			if (visited.find(right) == visited.end()) {
				if (maze[i][j + 2] != ' ' && maze[i - 1][j + 1] != ' ' && maze[i + 1][j + 1] != ' ') {
					neighbors++;
					neighbor_set.insert(right);
				}
			}

		//left box
		if (j - 2 >= 0)
			if (visited.find(left) == visited.end()) {
				if (maze[i][j - 2] != ' ' && maze[i - 1][j - 1] != ' ' && maze[i + 1][j - 1] != ' ') {
					neighbors++;
					neighbor_set.insert(left);
				}
			}

		// If 1 or more neighbors then select a random neighbor and move to it
		if (neighbors != 0) {
			auto it = neighbor_set.begin();
			for (int a = 0; a < rand() % neighbor_set.size(); a++) { it++; }
			pair<int, int> next_cell = *it;
			pos.push(current);
			current = next_cell;
			// the box is now open so set to ' '
			maze[current.first][current.second] = ' ';
			
		}
		else {
			//If the current box is the end destination, then copy stack as it holds the entire path from begin to end
			if (current.first == rows - 2 && current.second == cols - 2) {
				ans = pos;
			}

			current = pos.top();
			pos.pop();
		}
	}

	//if end is a wall then redraw maze
	if (maze[rows - 2][cols - 2] == '#')
		createMaze(maze);

	// set start and end
	maze[rows - 2][cols - 2] = '0';
	maze[current.first][current.second] = '*';
}

	//Driver code =========================================================


int main()
{
	srand(time(0));

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	
	char maze[rows][cols]; //maze array

	pair<int, int> current(1, 1); 
	bool displayAnswer = false;
	bool solve = false;
	createMaze(maze); // creating maze



	//GAME LOOP ===================================================


	while (1)
	{
		//Create new maze
		if (GetAsyncKeyState((unsigned short)'1') & 0x8000) {
			createMaze(maze);
			current = { 1,1 };
			solve = false;
			displayAnswer = false;
		}

		if (GetAsyncKeyState((unsigned short)'2') & 0x8000)
			solve = true;

		// If game in play mode, then allow user to move * using W, A, S, D
		if (solve && !displayAnswer) {

			//Move down
			if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
			{
				if (maze[current.first + 1][current.second] != '#') {
					maze[current.first][current.second] = ' ';
					maze[current.first + 1][current.second] = '*';
					current = { current.first + 1, current.second };
				}
			}

			//Move up
			if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
			{
				if (maze[current.first - 1][current.second] != '#') {
					maze[current.first][current.second] = ' ';
					maze[current.first - 1][current.second] = '*';
					current = { current.first - 1, current.second };
				}
			}

			//Move left
			if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			{
				if (maze[current.first][current.second - 1] != '#') {
					maze[current.first][current.second] = ' ';
					maze[current.first][current.second - 1] = '*';
					current = { current.first, current.second - 1 };
				}

			}

			//Move right
			if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			{
				if (maze[current.first][current.second + 1] != '#') {
					maze[current.first][current.second] = ' ';
					maze[current.first][current.second + 1] = '*';
					current = { current.first, current.second + 1 };
				}
			}
		}

		// Show Answer
		if (GetAsyncKeyState((unsigned short)'3') & 0x8000) 
		{
			displayAnswer = true;
			while (!ans.empty()) {
				maze[ans.top().first][ans.top().second] = '*';
				ans.pop();
			}
		}

		//Quit
		if (GetAsyncKeyState((unsigned short)'4') & 0x8000) {
			CloseHandle(hConsole);
			cout << "Game Over!!" << endl;
			system("pause");
			return 0;
		}

		Sleep(80);		
		
		//DISPLAY ============================

		//DISPLAY MAZE
		for (int x = 0; x < nScreenWidth; x++) 
		{
			for (int y = 0; y < nScreenHeight; y++) 
			{
				if (x < cols && y < rows) 
				{
					screen[nScreenWidth * y + x] = maze[y][x];
				}
				else screen[y * nScreenWidth + x] = ' ';
			}
		}

		// Display message when solved
		if (current.first == rows - 2 && current.second == cols - 2) 
		{
			swprintf_s(&screen[nScreenWidth * (rows + 1) + cols/2], 11, L"Well Done!");
		}

		// Display options
		swprintf_s(&screen[nScreenWidth * (rows / 2) + cols + 6], 47, L"Press 1, 2, 3, or 4 for the following actions:");
		swprintf_s(&screen[nScreenWidth*(rows/2 + 1) + cols +6], 12, L"1. New Maze");
		swprintf_s(&screen[nScreenWidth * (rows/2 + 3) + cols + 6], 14, L"3. Solve Maze");
		swprintf_s(&screen[nScreenWidth * (rows/2 + 2) + cols + 6], 13, L"2. Play Maze");
		swprintf_s(&screen[nScreenWidth * (rows/2 + 4) + cols + 6], 8, L"4. Quit");
		screen[nScreenWidth * nScreenHeight - 1] = '\0';

		//Display frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight*nScreenWidth, { 0,0 }, &dwBytesWritten);
	}
}
