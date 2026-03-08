#include <iostream>
#include <conio.h>

const int boardSize = 8;

char Board[boardSize][boardSize];
void initBoard();
void printBoard();

class Player {
public:
	int x{}, y{};
	Player(int startX, int startY) : x(startX), y(startY) {}	
	void drawStone() {Board[x][y] = 'O';}
	void MoveStone(int, int);
	
private:
};

int main()
{
	initBoard();
	Player p1(4, 4);

	while (true){
		system("cls");
		p1.drawStone();
		printBoard();

		int key = _getch();
		switch (key) {
		case 75:
			p1.MoveStone(p1.x - 1, p1.y);
			break;
		case 77:
			p1.MoveStone(p1.x + 1, p1.y);
			break;
		case 72:
			p1.MoveStone(p1.x, p1.y - 1);
			break;
		case 80:
			p1.MoveStone(p1.x, p1.y + 1);
			break;
		}
		printBoard();
	}
}

void initBoard()
{
	for (int i = 0; i < boardSize; ++i) {
		for (int j = 0; j < boardSize; ++j) {
			Board[j][i] = ' ';
		}
	}
}

void printBoard()
{
	for (int i = 0; i < boardSize; ++i)
	{
		std::cout << "---------------------------------" << '\n';
		for (int j = 0; j < boardSize; ++j)
		{
			std::cout << "| " << Board[j][i] << ' ';
		}
		std::cout << '|' << '\n';
	}
	std::cout << "---------------------------------" << '\n';
}

void Player::MoveStone(int newX, int newY)
{
	if (newX >= 0 && newX < boardSize && newY >= 0 && newY < boardSize) {
		Board[x][y] = ' ';
		x = newX;
		y = newY;
	}
}
