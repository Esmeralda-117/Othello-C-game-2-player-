#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

const int rows = 8;
const int cols = 8;

void selectColor();

void gameRules();

void welcomeScreen(char&, char&);

void displayBoard(char[rows][cols]);

void playerTurn(char[rows][cols], char, char);

bool possibleMoves(char[rows][cols], char, char);

bool flip(char[rows][cols], char, char, bool, int, int);

int calculateScore(char[rows][cols], char);

int main()
{
	selectColor();
	char player1;
	char player2;

	gameRules(); // displays rules

	welcomeScreen(player1, player2); // displays welcome screen on start of game

	char gameBoard[rows][cols];

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			gameBoard[i][j] = ' ';
		}
	}
	gameBoard[3][3] = player2, gameBoard[4][4] = player2;
	gameBoard[3][4] = player1, gameBoard[4][3] = player1;

	// main loop of the game
	while (possibleMoves(gameBoard, player1, player2) || possibleMoves(gameBoard, player2, player1))
	{
		if (possibleMoves(gameBoard, player1, player2))
		{
			cout << "\nPlayer " << player1 << "'s turn\n";
			playerTurn(gameBoard, player1, player2);
		}
		else
		{
			cout << "\nNo available moves left";
			cout << "\nPlayer " << player1 << "'s turn has been forfeited";
		}
		if (possibleMoves(gameBoard, player2, player1))
		{
			cout << "\nPlayer " << player2 << "'s turn\n";
			playerTurn(gameBoard, player2, player1);
		}
		else
		{
			cout << "\nNo available moves left";
			cout << "\nPlayer " << player2 << "'s turn has been forfeited";
		}
	}

	// calculate score at the end
	int player1Score = calculateScore(gameBoard, player1);
	int player2Score = calculateScore(gameBoard, player2);

	cout << "Player " << player1 << " score: " << player1Score << endl;
	cout << "Player " << player2 << " score: " << player2Score << endl;

	if (player1Score > player2Score)
	{
		cout << "Player " << player1 << " wins!!";
	}
	if (player1Score < player2Score)
	{
		cout << "Player " << player2 << " wins!!";
	}
	if (player1Score == player2Score)
	{
		cout << "Tie!";
	}

	return 0;
}

/*
	This function displays the rules of Othello
	It can be called if needed throughout the game
*/
void gameRules()
{
	cout << "1) The player must place disk adjacent to the\n"
		<< "opponents disk so that the opponent's disk\n"
		<< "is sandwiched between the two dics\n"
		<< "in which the oppoenent's discs will be flipped\n\n"

		<< "2) Available moves are marked with a dot '.'\n\n"

		<< "3) A disc is only flipped when it is the direct\n"
		<< "result of a move\n\n"

		<< "4) If the player has no available moves\n"
		<< "(cannot flip opponent's), then they must\n"
		<< "forfeit their turn to the other player\n\n"

		<< "5) The player with the most discs wins\n\n";
}

/*
	This function lets the user select the color
*/
void selectColor()
{
	int choice;
	cout << "Select a color: \n";
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 1; i <= 20; i++)
	{
		SetConsoleTextAttribute(color, i);
		cout << i << ") option\n";
	}
	cin >> choice;
	SetConsoleTextAttribute(color, choice);
}

/*
	This function welcomes the user to the game
	and it lets each player choose their symbol
*/
void welcomeScreen(char& p1, char& p2)
{
	char player1Choice;
	char player2Choice;

	string options = "XOABWN@Z&*%#";

	cout << "Welcome to Othello ! ! !\n\n";

	bool flag = false;

	int index;

	cout << "Player 1 pick your symbol from the following:\n";
	for (int i = 0; i < options.length(); i++)
	{
		cout << options[i] << "  ";
	}
	cout << endl;
	cin >> player1Choice;
	for (int i = 0; i < options.length(); i++)
	{
		if (player1Choice == options[i])
		{
			index = i;
			flag = true;
		}
	}
	while (!flag)
	{
		cout << "Error... must be out of the options\n";
		cout << "Player 1 pick your symbol from the following:\n";
		for (int i = 0; i < options.length(); i++)
		{
			cout << options[i] << "  ";
		}
		cout << endl;
		cin >> player1Choice;
		for (int i = 0; i <= options.length(); i++)
		{
			if (player1Choice == options[i])
			{
				index = i;
				flag = true;
			}
		}
	}
	options.erase(index, 1);
	flag = false;
	cout << "\nPlayer 2 pick your symbol from the following:\n";
	for (int i = 0; i < options.length(); i++)
	{
		cout << options[i] << "  ";
	}
	cout << endl;
	cin >> player2Choice;
	for (int i = 0; i < options.length(); i++)
	{
		if (player2Choice == options[i])
		{
			flag = true;
		}
	}
	while (!flag)
	{
		cout << "Error... must be out of the options\n";
		cout << "Player 2 pick your symbol from the following:\n";
		for (int i = 0; i < options.length(); i++)
		{
			cout << options[i] << "  ";
		}
		cout << endl;
		cin >> player2Choice;
		for (int i = 0; i < options.length(); i++)
		{
			if (player2Choice == options[i])
			{
				flag = true;
			}
		}
	}
	p1 = player1Choice;
	p2 = player2Choice;
}

/*
	This function displays the board to the users
	throughout the game
*/
void displayBoard(char gameBoard[rows][cols])
{
	bool flag = true;

	cout << "\n  +---+---+---+---+---+---+---+---+\n";

	for (int i = 0; i < rows; i++)
	{
		cout << i + 1 << " | ";
		for (int j = 0; j < cols; j++)
		{
			cout << gameBoard[i][j] << " | ";
			if (j == rows - 1)
			{
				flag = false;
			}
		}
		if (flag)
		{
			cout << " | ";
		}
		cout << "\n  +---+---+---+---+---+---+---+---+\n";
	}
	cout << "    1   2   3   4   5   6   7   8\n";
}

/*
	This function is for the player turns
	It asks user for row and column, validates, and flips discs
*/
void playerTurn(char gameBoard[rows][cols], char player, char otherPlayer)
{
	displayBoard(gameBoard);

	int row;
	int col;
	while (1)
	{
		cout << "Enter a row and then column to place " << player << endl;
		cin >> row;
		while (row < 1 || row > rows)
		{
			cout << "Error... must be in range\n";
			cout << "Enter a row and then column to place " << player << endl;
			cin >> row;
		}
		cin >> col;
		while (col < 1 || row > cols)
		{
			cout << "Error... must be in range\n";
			cout << "Enter a row and then column to place " << player << endl;
			cin >> col;
		}
		--row;
		--col;
		if (gameBoard[row][col] == player || gameBoard[row][col] == otherPlayer)
		{
			cout << "Error... must be in available location\n";
			return playerTurn(gameBoard, player, otherPlayer);
		}
		if (!flip(gameBoard, player, otherPlayer, true, row, col))
		{
			cout << "Location not valid\n";
			return playerTurn(gameBoard, player, otherPlayer);
		}
		else
		{
			gameBoard[row][col] = player;
			flip(gameBoard, player, otherPlayer, false, row, col);
			return;
		}
	}
}
/*
	This function will check if there are any possible moves for the player
	It will forfeit the turn to the other player if it returns false
*/
bool possibleMoves(char gameBoard[rows][cols], char player, char otherPlayer)
{
	bool possible = false;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (flip(gameBoard, player, otherPlayer, true, i, j))
			{
				if (gameBoard[i][j] == ' ')
				{
					gameBoard[i][j] = '.';
				}
				possible = true;
			}
			else
			{
				if (gameBoard[i][j] == '.')
				{
					gameBoard[i][j] = ' ';
				}
			}
		}
	}

	return possible;
}

/*
	This function has two purposes:
	1:
	Check if a move is valid. A move is valid if it results in the flipping
	of opponent's discs
	2:
	This function will flip the disks if the move is found
	to be valid
	It will only flip discs as the direct result of a move
*/
bool flip(char gameBoard[rows][cols], char player, char otherPlayer, bool test, int row, int col)
{
	int count, i, j;

	if (test)
	{
		if (gameBoard[row][col] == player || gameBoard[row][col] == otherPlayer)
		{
			return false;
		}
	}

	// right side
	count = 0;
	for (i = col + 1; i < cols; i++)
	{
		if (gameBoard[row][i] == otherPlayer)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	if (count > 0 && gameBoard[row][col + count + 1] == player)
	{
		if (test)
		{
			return true;
		}
		for (i = col + 1; i < cols; i++)
		{
			if (gameBoard[row][i] == otherPlayer)
			{
				gameBoard[row][i] = player;
			}
		}
	}

	// left side
	count = 0;
	for (i = col - 1; i > 0; i--)
	{
		if (gameBoard[row][i] == otherPlayer)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	if (count > 0 && gameBoard[row][col - count - 1] == player)
	{
		if (test)
		{
			return true;
		}
		for (i = col - 1; i > 0; i--)
		{
			if (gameBoard[row][i] == otherPlayer)
			{
				gameBoard[row][i] = player;
			}
		}
	}

	// down side
	count = 0;
	for (i = row + 1; i < rows; i++)
	{
		if (gameBoard[i][col] == otherPlayer)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	if (count > 0 && gameBoard[row + count + 1][col] == player)
	{
		if (test)
		{
			return true;
		}
		for (i = row + 1; i < rows; i++)
		{
			if (gameBoard[i][col] == otherPlayer)
			{
				gameBoard[i][col] = player;
			}
		}
	}

	// up side
	count = 0;
	for (i = row - 1; i > 0; i--)
	{
		if (gameBoard[i][col] == otherPlayer)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	if (count > 0 && gameBoard[row - count - 1][col] == player)
	{
		if (test)
		{
			return true;
		}
		for (i = row - 1; i > 0; i--)
		{
			if (gameBoard[i][col] == otherPlayer)
			{
				gameBoard[i][col] = player;
			}
		}
	}

	// down right side
	count = 0;
	for (i = row + 1, j = col + 1; i < rows && j < cols; i++, j++)
	{
		if (gameBoard[i][j] == otherPlayer)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	if (count > 0 && gameBoard[row + count + 1][col + count + 1] == player)
	{
		if (test)
		{
			return true;
		}
		for (i = row + 1, j = col + 1; i < rows && j < cols; i++, j++)
		{
			if (gameBoard[i][j] == otherPlayer)
			{
				gameBoard[i][j] = player;
			}
		}
	}

	// down left side
	count = 0;
	for (i = row + 1, j = col - 1; i < rows && j > 0; i++, j--)
	{
		if (gameBoard[i][j] == otherPlayer)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	if (count > 0 && gameBoard[row + count + 1][col - count - 1] == player)
	{
		if (test)
		{
			return true;
		}
		for (i = row + 1, j = col - 1; i < rows && j > 0; i++, j--)
		{
			if (gameBoard[i][j] == otherPlayer)
			{
				gameBoard[i][j] = player;
			}
		}
	}

	// up right side
	count = 0;
	for (i = row - 1, j = col + 1; i > 0 && j < cols; i--, j++)
	{
		if (gameBoard[i][j] == otherPlayer)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	if (count > 0 && gameBoard[row - count - 1][col + count + 1] == player)
	{
		if (test)
		{
			return true;
		}
		for (i = row - 1, j = col + 1; i > 0 && j < cols; i--, j++)
		{
			if (gameBoard[i][j] == otherPlayer)
			{
				gameBoard[i][j] = player;
			}
		}
	}

	// up left side
	count = 0;
	for (i = row - 1, j = col - 1; i > 0 && j > 0; i--, j--)
	{
		if (gameBoard[i][j] == otherPlayer)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	if (count > 0 && gameBoard[row - count - 1][col - count - 1] == player)
	{
		if (test)
		{
			return true;
		}
		for (i = row - 1, j = col - 1; i > 0 && j > 0; i--, j--)
		{
			if (gameBoard[i][j] == otherPlayer)
			{
				gameBoard[i][j] = player;
			}
		}
	}
	if (test)
	{
		return false;
	}
	return false;
}

/*
	This function will calculate the scores of each player
	by counting the number of discs each player has at the end
	of the game
	The player with the most discs wins
*/
int calculateScore(char gameBoard[rows][cols], char player)
{
	int score = 0;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (gameBoard[i][j] == player)
			{
				score++;
			}
		}
	}
	return score;
}