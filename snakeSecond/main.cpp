#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <thread>
#include <deque>
#include <mutex>

#define test std::cout << "test";

bool isGameRunning = true;
std::mutex mtx;

void SC(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

void setCursor(int x, int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(hConsole, pos);
}

class Position {
public:
	int x, y;
	Position(int x_new, int y_new) : x(x_new), y(y_new) {}
};

class Snake {
public:
	int x = 5;
	int y = 5;
	static const int size = 10;
	char Map[size][size];
	std::deque <Position> allParts;
	int dir = 1;
	int score = 0;
	Position apple;
	
	Snake(): apple(0,0) {
		allParts.push_front(Position(7, 5));
		allParts.push_front(Position(6, 5));
		allParts.push_front(Position(5, 5));

		apple.x = randomPos(); apple.y = randomPos();

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				Map[i][j] = 'b';
			}
		}
	}

	int randomPos() {
		return (rand() % size + 0);
	}

	
	void changeDir(int newDir) {
		switch (newDir) {
		case 1:
			if ((allParts.front().x-1) != x) {
				dir = 1;
			}; break;
		case 2:
			if ((allParts.front().y-1) != y) {
				dir = 2;
			}; break;
		case 3:
			if ((allParts.front().x+1) != x) {
				dir = 3;
			}; break;
		case 4:
			if ((allParts.front().y+1) != y) {
				dir = 4;
			}; break;
		}
	}

	void keyLogic() {
		while (isGameRunning) {
			char key = _getch();
			switch (key) {
			case 'w':
				std::cout << "w";
				changeDir(1); break;
			case 'a':
				std::cout << "a";
				changeDir(2); break;
			case 's':
				std::cout << "s";
				changeDir(3); break;
			case 'd':
				std::cout << "d";
				changeDir(4); break;   
			}
		}
	}

	bool posCheckBad() {
		for (int i = 0; i < allParts.size(); i++) {
			if (apple.x == allParts[i].x && apple.y == allParts[i].y) {
				return true;
			}
			return false;
		}
	}
	
	void snakeMove(int& dir) {
		if (dir == 1) {
			allParts.push_front(Position(allParts.front().x-1, allParts.front().y)); // x--
		}
		else if (dir == 2) {
			allParts.push_front(Position(allParts.front().x, allParts.front().y-1)); // y--
		}
		else if (dir == 3) {
			allParts.push_front(Position(allParts.front().x+1, allParts.front().y)); // x++
		}
		else if (dir == 4) {
			allParts.push_front(Position(allParts.front().x, allParts.front().y+1)); // y++
		}
	}

	void whichDir(int& x, int& y) {
		if (dir == 1) { // Up
			if (allParts.front().x == 0) { isGameRunning = false; } else {x = (allParts.front().x); y = (allParts.front().y); }
		}
		else if (dir == 2) { // Left
			if (allParts.front().y == 0) { isGameRunning = false; } else { x = (allParts.front().x); y = (allParts.front().y); }
		}
		else if (dir == 3) { // Down
			if (allParts.front().x == size-1) { isGameRunning = false; } else {x = (allParts.front().x); y = (allParts.front().y); }
		}
		else if (dir == 4) { // Right
			if (allParts.front().y == size-1) { isGameRunning = false; }else { x = (allParts.front().x); y = (allParts.front().y); }
		}
	}

	void gameLogic() {
		Map[apple.x][apple.y] = 'a';
			whichDir(x, y);
			snakeMove(dir);
			if ((apple.x == allParts.front().x) && (apple.y == allParts.front().y)) {
				score++;
				apple.x = randomPos();
				apple.y = randomPos();
				while (posCheckBad()) {
					apple.x = randomPos();
					apple.y = randomPos();
					Map[apple.x][apple.y] = 'a';
				}
			}
			else {
				Map[allParts.back().x][allParts.back().y] = 'b';
				allParts.pop_back();
			}
			if (headInBody()) {
				isGameRunning = false;
			}
		}

	bool headInBody() {
		for (int i = 1; i < allParts.size(); i++) {
			if (allParts.front().x == allParts[i].x && allParts.front().y == allParts[i].y) {
				return true;
			}
		}
		return false;
	}

	void gameRender() {
		for (int i = 0; i < allParts.size(); i++) {
			Map[allParts[i].x][allParts[i].y] = 's';
		}
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				switch (Map[i][j]) {
				case 's': SC(10 + 15 * 10); break;
				case 'b': SC(7 + 15 * 7); break;
				case 'a': SC(12 + 15 * 12); break;
				}
				std::cout << "  ";
			}
			SC(0); std::cout << std::endl;
		}
	}

	void gameRunning() {
		while (isGameRunning) {

			SC(14); std::cout << "  -~ Snake ~-" << std::endl;
			SC(6);  std::cout << "Made by Advenes" << std::endl;

			gameLogic();

			gameRender();
			SC(8);  std::cout << "Score: " << score << std::endl;
			::Sleep(1000);
			setCursor(0, 0);
		}
	}

};

int main() {
	srand(time(NULL));
	Snake GameInstance;

	std::thread THREAD_game(&Snake::gameRunning, &GameInstance);
	std::thread THREAD_key(&Snake::keyLogic, &GameInstance);

	THREAD_game.join();
	THREAD_key.join();
}