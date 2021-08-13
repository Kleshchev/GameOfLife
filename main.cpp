#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#define CELL_WIDTH 4
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 1000
#define MOUSE_WIDTH 15
sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                        "It's alive");

const int X_CELLS            = WINDOW_WIDTH / CELL_WIDTH;
const int Y_CELLS            = WINDOW_HEIGHT / CELL_WIDTH;
const int CELLS_ARRAY_LENGTH = Y_CELLS * X_CELLS;
bool active_cells[Y_CELLS][X_CELLS];
bool next_state[Y_CELLS][X_CELLS];
void drawGrid() {
	for (int i = 0; i <= X_CELLS; i++) {
		sf::RectangleShape line(sf::Vector2f(WINDOW_HEIGHT, 1));
		line.setPosition(sf::Vector2f(CELL_WIDTH * i, 0));
		line.setRotation(90);
		line.setFillColor(sf::Color::Black);
		window.draw(line);
	}
	for (int i = 0; i <= Y_CELLS; i++) {
		sf::RectangleShape line(sf::Vector2f(WINDOW_WIDTH, 1));
		line.setPosition(sf::Vector2f(0, CELL_WIDTH * i));
		line.setFillColor(sf::Color::Black);
		window.draw(line);
	}
}
void drawPixel(int x_pos, int y_pos, sf::Color color) {
	sf::RectangleShape pixel(sf::Vector2f(CELL_WIDTH, CELL_WIDTH));
	pixel.setPosition(sf::Vector2f(x_pos * CELL_WIDTH, y_pos * CELL_WIDTH));
	pixel.setFillColor(color);
	window.draw(pixel);
}
int mapToColor(int value, int min, int max) {
	return ((value - min) / (float)(max - min)) * 255;
}
void drawCells(int r, int g, int b) {
	for (int i = 0; i < Y_CELLS; i++)
		for (int j = 0; j < X_CELLS; j++) {
			if (active_cells[i][j]) drawPixel(j, i, sf::Color(r, g, b, 255));
		}
}
int getFixedCoords(int pos, int max) {
	return (pos >= max ? pos % max : (pos < 0 ? max + pos % max : pos));
}

int getNeighborsCount(int x_pos, int y_pos) {
	int count = 0;

	count += active_cells[getFixedCoords(y_pos - 1, Y_CELLS)]
	                     [getFixedCoords(x_pos - 1, X_CELLS)];

	count += active_cells[getFixedCoords(y_pos - 1, Y_CELLS)]
	                     [getFixedCoords(x_pos, X_CELLS)];

	count += active_cells[getFixedCoords(y_pos - 1, Y_CELLS)]
	                     [getFixedCoords(x_pos + 1, X_CELLS)];

	count += active_cells[getFixedCoords(y_pos, Y_CELLS)]
	                     [getFixedCoords(x_pos - 1, X_CELLS)];

	count += active_cells[getFixedCoords(y_pos, Y_CELLS)]
	                     [getFixedCoords(x_pos + 1, X_CELLS)];

	count += active_cells[getFixedCoords(y_pos + 1, Y_CELLS)]
	                     [getFixedCoords(x_pos - 1, X_CELLS)];

	count += active_cells[getFixedCoords(y_pos + 1, Y_CELLS)]
	                     [getFixedCoords(x_pos, X_CELLS)];

	count += active_cells[getFixedCoords(y_pos + 1, Y_CELLS)]
	                     [getFixedCoords(x_pos + 1, X_CELLS)];
	return count;
}
void getNewState() {
	for (int i = 0; i < Y_CELLS; i++)
		for (int j = 0; j < X_CELLS; j++) {
			next_state[i][j] = 0;
		}

	for (int i = 0; i < Y_CELLS; i++)
		for (int j = 0; j < X_CELLS; j++) {
			int neighboursCount = getNeighborsCount(j, i);
			if (!active_cells[i][j]) next_state[i][j] = 0;
			if (active_cells[i][j]) next_state[i][j] = 1;
			if ((neighboursCount > 3 || neighboursCount < 2) &&
			    active_cells[i][j])
				next_state[i][j] = 0;
			if (neighboursCount == 3 && !active_cells[i][j])
				next_state[i][j] = 1;
		}
	for (int i = 0; i < Y_CELLS; i++)
		for (int j = 0; j < X_CELLS; j++) {
			active_cells[i][j] = next_state[i][j];
		}
}
void randIntColor(int &value, int &direction, int diff) {
	int offset = rand() % diff;
	if (value + direction * offset > 255 || value + direction * offset < 0) {
		direction *= -1;
		value += direction * offset;
	} else
		value += direction * offset;
}
int main() {
	int R, G, B;
	R = rand() % 256;
	G = rand() % 256;
	B = rand() % 256;
	int RD, GD, BD;
	RD = GD = BD = -1;
	srand(time(NULL));
	for (int i = 0; i < Y_CELLS; i++)
		for (int j = 0; j < X_CELLS; j++) active_cells[i][j] = rand() % 2;
	window.setFramerateLimit(60);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				auto releasePos = sf::Mouse::getPosition(window);
				for (int i = -MOUSE_WIDTH / 2; i <= MOUSE_WIDTH / 2; i++)
					for (int j = -MOUSE_WIDTH / 2; j <= MOUSE_WIDTH / 2; j++)
						if (rand() % 2) {
							int x = getFixedCoords(
							    releasePos.x / CELL_WIDTH + i, X_CELLS);
							int y = getFixedCoords(
							    releasePos.y / CELL_WIDTH + j, Y_CELLS);
							active_cells[y][x] = !active_cells[y][x];
						}
			}
		}
		randIntColor(R, RD, 4);
		randIntColor(G, GD, 4);
		randIntColor(B, BD, 4);
		window.clear(sf::Color::Black);
		drawCells(R, G, B);
		// drawGrid();
		getNewState();
		window.display();
	}

	return 0;
}