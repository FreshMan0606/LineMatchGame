#include <sfml\Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

sf::Vector2f getPixelPosition(const sf::Vector2i& pos, const sf::Vector2u& blockSize) {
	return sf::Vector2f(float(pos.x * blockSize.x), float(pos.y * blockSize.y));
}

int main() {

	srand(time(0));

	const int fieldWidth = 6;
	const int fieldHeight = 10;
	bool field[fieldWidth][fieldHeight] = {};

	for (int i = 8; i < fieldHeight; ++i) {
		for (int j = 1; j < 3; ++j) {
			field[rand() % fieldWidth][i] = true;
		}
	}


	sf::Texture blockTexture;
	if (!blockTexture.loadFromFile("block.png")) {
		return EXIT_FAILURE;
	}
	sf::Vector2u blockSize(blockTexture.getSize());
	const int windowWidth = blockSize.x * fieldWidth;
	const int windowHeight = blockSize.y * fieldHeight;
	sf::String title("LineMatchGame");
	sf::VideoMode mode(windowWidth, windowHeight);
	sf::RenderWindow window(mode, title);

	sf::Sprite block(blockTexture);
	sf::Vector2i origin(fieldWidth / 2, 0);
	sf::Vector2i pos(origin);
	block.setPosition(getPixelPosition(pos, blockSize));
	sf::Clock clock;
	while (window.isOpen()) {
		enum class Action {
			HOLD,
			MOVE_LEFT,
			MOVE_RIGHT,
			MOVE_DOWN,
			SPACE
		};
		Action action = Action::HOLD;
		sf::Event event;
		sf::Vector2i nextPos = pos;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Left) {
					action = Action::MOVE_LEFT;
				}
				if (event.key.code == sf::Keyboard::Right) {
					action = Action::MOVE_RIGHT;
				}
				if (event.key.code == sf::Keyboard::Down) {
					nextPos.y++;
				}
				if (event.key.code == sf::Keyboard::Space) {
					action = Action::SPACE;
				}
			}
		}
		if (clock.getElapsedTime().asSeconds() >= 0.8f) {
			action = Action::MOVE_DOWN;
			clock.restart();
		}
		
		switch (action) {
			case Action::HOLD :
				break;
			case Action::MOVE_LEFT :
				nextPos.x--;
				break;
			case Action::MOVE_RIGHT :
				nextPos.x++;
				break;
			case Action::MOVE_DOWN :
				nextPos.y++;
				break;
			
		}

		if (action == Action::SPACE) {
			for (int y = nextPos.y + 1; y < fieldHeight; ++y) {
				if (field[nextPos.x][y]) {
					nextPos.y = y - 1;
					break;
				}
				else {
					nextPos.y = y;
				}
			}
		}

		if (nextPos.x >= 0 && nextPos.x <= fieldWidth && nextPos.y >= 0 && nextPos.y < fieldHeight && field[nextPos.x][nextPos.y] == false) {
			pos = nextPos;
		}
		else {
			if (action == Action::MOVE_DOWN) {
				field[pos.x][pos.y] = true;
				bool isfull = true;
				for (int x = 0; x < fieldWidth; x++) {
					if (field[x][pos.y] == false) {
						isfull = false;
					}
				}
				if (isfull) {
					for (int y = pos.y; y > 0; y--) {
						for (int x = 0; x < fieldWidth; ++x) {
							field[x][y] = field[x][y - 1];
						}
					}
					for (int x = 0; x < fieldWidth; ++x) {
						field[x][0] = false;
					}
				}
				pos = origin;
			}
		}
		

		bool isempty = true;
		for (int y = 0; y < fieldHeight; ++y) {
			for (int x = 0; x < fieldWidth; ++x) {
				if (field[x][y] == true) {
					isempty = false;
					break;
				}
				
			}
		}

		if (isempty) {
			return EXIT_SUCCESS;
		}

		window.clear();
		block.setPosition(getPixelPosition(pos, blockSize));
		window.draw(block);

		for (int x = 0; x < fieldWidth; ++x) {
			for (int y = 0; y < fieldHeight; ++y) {
				if (field[x][y] == true) {
					sf::Vector2i p(x, y);
					block.setPosition(getPixelPosition(p, blockSize));
					window.draw(block);
					
				}
			}
		}

		window.display();

	}
	return EXIT_SUCCESS;
}