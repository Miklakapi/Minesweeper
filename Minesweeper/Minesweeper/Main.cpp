#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "Minesweeper.hpp"

using namespace sf;

int main() {

	int speedLV(10);
	srand(unsigned int(time(NULL)));

	//////////////////////////////////////////////
	//Window
	const VideoMode appMode{ 1000, 800, 32 };
	RenderWindow app{ appMode,"Minesweeper",Style::Close };
	app.setFramerateLimit(144);
	const Vector2u appSize{ app.getSize() };

	//////////////////////////////////////////////
	//Texture
	Texture saperT;
	Texture clock;
	Texture winlose;
	Texture emote;

	saperT.loadFromFile("minesweeper.png");
	clock.loadFromFile("timer.png");
	winlose.loadFromFile("winlose.png");
	emote.loadFromFile("emote.png");

	//////////////////////////////////////////////
	//IntRect

	//Clock
	IntRect	zeroC{ 3 * 27,0,27,50 };
	IntRect oneC{ 0,0,27,50 };
	IntRect twoC{ 27,0,27,50 };
	IntRect threeC{ 2 * 27,0,27,50 };
	IntRect fourC{ 0,50,27,50 };
	IntRect fiveC{ 27,50,27,50 };
	IntRect sixC{ 2 * 27,50,27,50 };
	IntRect sevenC{ 3 * 27,50,27,50 };
	IntRect eightC{ 0,100,27,50 };
	IntRect nineC{ 27,100,27,50 };
	IntRect reTabC[10] = { zeroC, oneC, twoC, threeC, fourC, fiveC, sixC, sevenC, eightC, nineC };

	//Numbers
	IntRect	zero{ 0,0,50,50 };
	IntRect one{ 0,50,50,50 };
	IntRect two{ 50,50,50,50 };
	IntRect three{ 100,50,50,50 };
	IntRect four{ 150,50,50,50 };
	IntRect five{ 200,50,50,50 };
	IntRect six{ 250,50,50,50 };
	IntRect seven{ 300,50,50,50 };
	IntRect eight{ 300,0,50,50 };
	IntRect bomb{ 150,0,50,50 };
	IntRect bombMiss{ 200,0,50,50 };
	IntRect bombExplode{ 250,0,50,50 };
	IntRect field{ 100,0,50,50 };
	IntRect flag{ 50,0,50,50 };
	IntRect reTabR[14] = { zero, one, two, three, four, five, six, seven, eight, bomb, bombMiss, bombExplode, field, flag };

	//Baner
	IntRect winI{ 0,0,150,50 };
	IntRect loseI{ 0,50,150,50 };
	IntRect emptyI{ 0,100,150,50 };
	IntRect reTabB[3] = { emptyI, winI, loseI };

	//Emote
	IntRect sadI{ 0,0,50,50 };
	IntRect smileI{ 50,0,50,50 };
	IntRect reTabE[2] = { smileI, sadI };

	//////////////////////////////////////////////
	//Saper

	Minesweeper minesweeper{ Vector2f{0,50}, Vector2f{float(app.getSize().x),float(app.getSize().y - 50)}, Vector2f{50,50} };
	minesweeper.loadTextures(&saperT, &clock, &emote, &winlose);
	minesweeper.loadIntRect(reTabR, reTabC, reTabE, reTabB);
	minesweeper.setSize(Vector2f{ 27,50 }, Vector2f{ 50,50 }, Vector2f{ 150,50 });
	minesweeper.setPosition(Vector2f{ 0,0 }, Vector2f{ float(int((appSize.x / 2) - (50 / 2))),0 }, Vector2f{ float(appSize.x - 150),0 });

	//////////////////////////////////////////////
	//Bot

	Bot bot{ &minesweeper };
	float delay{ float(1) };
	bot.changeSpeed(delay);

	//////////////////////////////////////////////
	//Main loop

	while (app.isOpen()) {

		//Event loop
		Event event;

		while (app.pollEvent(event)) {

			if (event.type == Event::Closed) {
				app.close();
				break;
			}

			if (event.type == Event::KeyPressed) {

				if (event.key.code == Keyboard::Escape) {
					app.close();
					break;
				}

				if (event.key.code == Keyboard::Space) {
					bot.switchBot();
					break;
				}

				if (event.key.code == Keyboard::R) {
					minesweeper.reset();
					bot.turnOff();
					break;
				}

				if (event.key.code == Keyboard::Up) {
					speedLV++;
					delay += float(0.1);
					bot.changeSpeed(delay);
					break;
				}

				if (event.key.code == Keyboard::Down && speedLV > 0) {
					speedLV--;
					if (speedLV == 0)delay = 0;
					else delay -= float(0.1);
					bot.changeSpeed(delay);
					break;
				}
			}

			//Mouse button click
			Vector2i mousePos;

			if (event.type == Event::MouseButtonPressed) {
				mousePos = Mouse::getPosition(app);

				if (mousePos.x < 0 || mousePos.x > app.getSize().x || mousePos.y < 0 || mousePos.y > app.getSize().y) break;

				if (event.mouseButton.button == Mouse::Button::Right) {

					if (minesweeper.click(mousePos) == 1) {
						minesweeper.areaClick(minesweeper.getNrRectangle(mousePos), Mouse::Button::Right);
					}
				}

				if (event.mouseButton.button == Mouse::Button::Left) {

					if (minesweeper.click(mousePos) == 1) {
						minesweeper.areaClick(minesweeper.getNrRectangle(mousePos), Mouse::Button::Left);
					}
					else if (minesweeper.click(mousePos) == 0) {
						minesweeper.reset();
						bot.turnOff();
					}
				}
			}
		}

		//Bot
		Click click = bot.checkRMB();
		if (click.number >= 0) {
			bot.setMouse(click.number, app);
			minesweeper.areaClick(click.number, click.button);
		}

		//Draw
		app.clear();
		minesweeper.renderMape(app);
		app.display();
	}
}