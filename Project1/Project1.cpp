#include <iostream>
#include <conio.h>
#include <windows.h>
#include <deque>
#include <string>

using namespace std;

struct Point { int x, y; };

void coutWithDelay(int delay, string text) {
	Sleep(delay);
	cout << text;
}

void countDown() {
	system("cls");
	cout << "3";
	Sleep(1000);

	system("cls");
	cout << "2";
	Sleep(1000);

	system("cls");
	cout << "1";
	Sleep(1000);

	system("cls");
}

class Game {
private:
	// Rozmiar planszy i stan początkowy
	const int W = 30, H = 20;
	// Wąż zaczyna na środku. deque – kolejka dwukierunkowa (double-ended queue)
	deque<Point> snake = { {W / 2, H / 2} };
	// Jedzenie na stałej pozycji (później będzie losowe)
	Point food = { 5, 5 };
	// Kierunek węża. Na początku w prawo
	int dx = 1, dy = 0;
	bool isGameEnd = false;
	// Częstotliwość klatek na sekundę. Z czasem się zmniejsza odstęp między klatkami - gra się przyspiesza
	int timeToNextFrame = 175;

	// Renderowanie planszy
	void render() {
		// Czyścimy cały ekran
		system("cls");
		for (int y = -1; y < H + 1; y++) {
			for (int x = -1; x < W + 1; x++) {
				bool drawn = false;

				if (snake.front().x == x && snake.front().y == y) {
					cout << "@ ";
					continue;
				}

				// Rysujemy granicę
				if (y == -1 || y == H) {
					cout << "- ";
					continue;
				}
				if (x == -1 || x == W) {
					cout << "| ";
					continue;
				}

				// Rysujemy jedzenie
				if (x == food.x && y == food.y) {
					cout << "* ";
					continue;
				}

				// Rysujemy węża
				for (auto& p : snake) {
					if (p.x == x && p.y == y) {
						cout << "O ";
						drawn = true;
						break;
					}
				}

				if (!drawn) cout << "  ";
			}
			cout << "\n";
		}
	}

	void displayGameOver() {
		cout << "\nGra ukonczona. Twoj score: " << getScore() << "\n";
	}

	// Obsługa sterowania. Ustawia move direction węża
	void keyboardHandler() {
		// _kbhit() sprawdza czy klawisz został naciśnięty
		if (_kbhit()) {
			// _getch() pobiera naciśnięty klawisz
			char c = _getch();
			if (c == 'w' && dy != 1) { dx = 0; dy = -1; }
			if (c == 's' && dy != -1) { dx = 0; dy = 1; }
			if (c == 'a' && dx != 1) { dx = -1; dy = 0; }
			if (c == 'd' && dx != -1) { dx = 1; dy = 0; }
		}
	}

	// Sprawdza kolizje z jedzeniem  i granicami, przesuwa weza
	void moveSnakeWithCollisions() {
		Point head = { snake.front().x + dx, snake.front().y + dy };
		snake.push_front(head);

		// Sprawdzanie czy nie weszla na siebie
		for (size_t i = 1; i < snake.size(); i++)
			if (head.x == snake[i].x && head.y == snake[i].y) { isGameEnd = true; }

		// Sprawdzanie granic.
		if (head.x < 0 || head.y < 0 || head.x >= W || head.y >= H) { isGameEnd = true; }

		// Sprawdzanie jedzenia
		// Jeśli głowa dotarła do jedzenia – tworzy się nowe jedzenie, ogon nie znika
		// Jeśli nie – ogon jest usuwany, długość zostaje taka sama
		if (head.x == food.x && head.y == food.y) {
			food = { rand() % W, rand() % H };

			// Przyspieszanie gry
			if (timeToNextFrame > 125) { timeToNextFrame -= 5; }
			else if (timeToNextFrame > 75) { timeToNextFrame -= 2; }
			else if (timeToNextFrame > 50) { timeToNextFrame -= 1; }
		}
		else {
			// jeśli nie zjedliśmy jedzenia, usuwamy jeden segment od strony ogona
			snake.pop_back();
		}
	}

public:
	void reset() {
		snake = { {W / 2, H / 2} };
		food = { 5, 5 };
		dx = 1, dy = 0;
		isGameEnd = false;
		timeToNextFrame = 175;
	}

	void startLoop() {
		while (true) {
			keyboardHandler();
			moveSnakeWithCollisions();
			render();

			if (isGameEnd == true)
			{
				displayGameOver();
				break;
			}

			Sleep(timeToNextFrame);
		}
	}

	int getScore() {
		return snake.size() - 1;
	}
};

int main() {
	string command;

	srand(time(0));

	// Wyświetlamy tytuł gry oraz je zasady
	cout << "Witam w grze \"Hungry Snake\"!\n";
	coutWithDelay(2000, "\nZasady gry:\n");
	coutWithDelay(1000, " 1. Waz porusza sie po polu i nie moze przechodzic przez sciany. Uderzenie w granice pola konczy gre.\n");
	coutWithDelay(300, " 2. Jedzenie pojawia sie w losowych wspolrzednych. Zjedzenie jedzenia powoduje wydluzenie weza.\n");
	coutWithDelay(300, " 3. Jesli waz uderzy sam w siebie, gra rowniez sie konczy.\n");
	coutWithDelay(300, " 4. Sterowanie: W A S D - ruch w gore, w lewo, w dol, w prawo.\n");
	coutWithDelay(300, " 5. Cel gry - przetrwac jak najdluzej i wyhodowac jak najdluzszego weza. Chociaz wszyscy wiemy, jak to sie konczy.\n");

	coutWithDelay(2000, "\nOznaczenia:\n");
	coutWithDelay(500, " O - waz\n");
	coutWithDelay(100, " * - jedzenie\n");
	coutWithDelay(100, " - lub | - granica\n");

	coutWithDelay(1000, "\nJezeli jestes gotow, wpisz \"start\": ");
	cin >> command;

	if (command == "start") { countDown(); }
	else {
		cout << "Nie wpisales prawidlowo \"start\", ale udam ze napisales prawidlowo.";
		Sleep(3000);
		countDown();
	}

	// Liczba zagranych gier
	int playedGames = 0;
	// Maksymalny score
	int maxScore = 0;
	// Score ostatniej zagranej gry
	int lastScore;
	Game game;

	// Póki gracz nie wpisze q, włancza grę ponownie
	while (true) {
		playedGames++;
		game.reset();
		game.startLoop(); // Po zakończeniu game loop sam się wyłączy

		// Update maksymalnego score'a gracza
		lastScore = game.getScore();
		if (lastScore > maxScore) { maxScore = lastScore; }

		cout << "\nZeby zaczac od nowa wpisz \"r\" lub \"R\".\n";
		cout << "Jezeli chcesz wyjsc wpisz \"q\" lub \"Q\".\n";
		cout << "Twoje polecenie: ";
		cin >> command;

		if (command == "Q" || command == "q") { break; }
		countDown();
	}

	// Wyświetlenie statystyki
	cout << "\n* Zagrales " << playedGames << " razy.\n";
	cout << "* Maksymalny score: " << maxScore << ".\n";
}
