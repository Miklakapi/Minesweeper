#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "Minesweeper.hpp"
#include <time.h>

using namespace sf;

Minesweeper::Minesweeper(Vector2f position, Vector2f size, Vector2f textureSize) {

	winG = false;
	loseG = false;

	float xS = position.x;
	float yS = position.y;

	int width = int(size.x / textureSize.x);
	int height = int(size.y / textureSize.y);

	colNr = width;
	rowNr = height;

	objectNr = width * height;
	pointer = new Rectangle[objectNr];

	maxClick = objectNr - int(objectNr * 0.15);

	int i(0);
	for (int y = 0; y < height; y++) {

		for (int x = 0; x < width; x++) {
			(pointer + i)->rectangle.setSize(textureSize);
			(pointer + i)->rectangle.setPosition(Vector2f{ (x*textureSize.x) + xS , (y*textureSize.y) + yS });

			i++;
		}
	}

	counterSize = Vector2f{ 0,0 };

	areaField.left = position;
	areaField.right.x = position.x + (textureSize.x * width);
	areaField.right.y = position.y + (textureSize.y * height);
}

void Minesweeper::loadTextures(Texture* rectangle, Texture* counter, Texture* emoticon, Texture* baner) {
	for (int i = 0; i < objectNr; i++) {
		(pointer + i)->rectangle.setTexture(rectangle);
	}
	mineCounter.rectangle0.setTexture(counter);
	mineCounter.rectangle1.setTexture(counter);
	mineCounter.rectangle2.setTexture(counter);
	emote.rectangle.setTexture(emoticon);
	this->banner.rectangle.setTexture(baner);
}

void Minesweeper::loadIntRect(IntRect* rectangleRect, IntRect* counterRect, IntRect* emoteRect, IntRect* banerRect) {
	for (int i = 0; i < objectNr; i++) {
		(pointer + i)->rect = rectangleRect;
	}
	mineCounter.rect = counterRect;
	emote.rect = emoteRect;
	banner.rect = banerRect;
	reset();
}

void Minesweeper::setSize(Vector2f counterSize, Vector2f emoteSize, Vector2f banerSize) {
	this->counterSize = counterSize;
	mineCounter.rectangle1.setSize(counterSize);
	mineCounter.rectangle2.setSize(counterSize);
	emote.rectangle.setSize(emoteSize);
	banner.rectangle.setSize(banerSize);
}

void Minesweeper::setPosition(Vector2f counterPos, Vector2f emotePos, Vector2f banerPos) {
	if (counterSize == Vector2f{ 0,0 }) return;
	mineCounter.setPosition(counterPos, counterSize);
	emote.rectangle.setPosition(emotePos);
	banner.rectangle.setPosition(banerPos);

	counterField.left = counterPos;
	counterField.right.x = counterPos.x + (2 * mineCounter.rectangle1.getSize().x) + 3;
	counterField.right.y = counterPos.y + mineCounter.rectangle1.getSize().y;

	emoteField.left = emotePos;
	emoteField.right.x = emotePos.x + emote.rectangle.getSize().x;
	emoteField.right.y = emotePos.y + emote.rectangle.getSize().y;

	banerField.left = banerPos;
	banerField.right.x = banerPos.x + banner.rectangle.getSize().x;
	banerField.right.y = banerPos.y + banner.rectangle.getSize().y;
}

int Minesweeper::getNrRectangle(Vector2i MousePosition) {
	int Nr;
	Nr = int(MousePosition.x / (pointer->rectangle.getSize().x));
	Nr += int((MousePosition.y - areaField.left.y) / (pointer->rectangle.getSize().y)) * (colNr);
	return Nr;
}

int Minesweeper::click(Vector2i MousePosition) {
	if (MousePosition.x > areaField.left.x && MousePosition.x < areaField.right.x &&
		MousePosition.y > areaField.left.y && MousePosition.y < areaField.right.y) {

		return 1;
	}
	else if (MousePosition.x > emoteField.left.x && MousePosition.x < emoteField.right.x &&
		MousePosition.y > emoteField.left.y && MousePosition.y < emoteField.right.y) {

		return 0;
	}
	else return -1;
}

void Minesweeper::areaClick(int squareNr, Mouse::Button button) {
	if (winG == true || loseG == true) return;

	if (button == Mouse::Button::Right) {

		if ((pointer + squareNr)->visible == Type::Content::Field && mineCounter.counter > 0) {
			mineCounter.setCounter(mineCounter.counter - 1);
			(pointer + squareNr)->setVisible(Type::Content::Flag);
		}
		else if ((pointer + squareNr)->visible == Type::Content::Flag) {
			mineCounter.setCounter(mineCounter.counter + 1);
			(pointer + squareNr)->setVisible(Type::Content::Field);
		}
		else if ((pointer + squareNr)->visible != Type::Content::Empty) {
			int flags(0);
			if (squareNr > colNr && squareNr < ((colNr*(rowNr - 1) - 1)) && squareNr % colNr != 0 && (squareNr + 1) % colNr != 0) {
				if ((pointer + (squareNr - (colNr + 1)))->visible == Type::Content::Flag)	flags++;
				if ((pointer + (squareNr - colNr))->visible == Type::Content::Flag)			flags++;
				if ((pointer + (squareNr - (colNr - 1)))->visible == Type::Content::Flag)	flags++;
				if ((pointer + (squareNr - 1))->visible == Type::Content::Flag)				flags++;
				if ((pointer + (squareNr + 1))->visible == Type::Content::Flag)				flags++;
				if ((pointer + (squareNr + (colNr - 1)))->visible == Type::Content::Flag)	flags++;
				if ((pointer + (squareNr + colNr))->visible == Type::Content::Flag)			flags++;
				if ((pointer + (squareNr + (colNr + 1)))->visible == Type::Content::Flag)	flags++;
			}
			else if (squareNr > 0 && squareNr < (colNr - 1)) {
				if ((pointer + (squareNr - 1))->visible == Type::Content::Flag)				flags++;
				if ((pointer + (squareNr + 1))->visible == Type::Content::Flag)				flags++;
				if ((pointer + (squareNr + (colNr - 1)))->visible == Type::Content::Flag)	flags++;
				if ((pointer + (squareNr + colNr))->visible == Type::Content::Flag)			flags++;
				if ((pointer + (squareNr + (colNr + 1)))->visible == Type::Content::Flag)	flags++;
			}
			else if (squareNr % colNr == 0 && squareNr != 0 && squareNr != (colNr * (rowNr - 1))) {
				if ((pointer + (squareNr - colNr))->visible == Type::Content::Flag)			flags++;
				if ((pointer + (squareNr - (colNr - 1)))->visible == Type::Content::Flag)	flags++;
				if ((pointer + (squareNr + 1))->visible == Type::Content::Flag)				flags++;
				if ((pointer + (squareNr + colNr))->visible == Type::Content::Flag)			flags++;
				if ((pointer + (squareNr + (colNr + 1)))->visible == Type::Content::Flag)	flags++;
			}
			else if ((squareNr + 1) % colNr == 0 && squareNr != (colNr - 1) && squareNr != (objectNr - 1)) {
				if ((pointer + (squareNr - (colNr + 1)))->visible == Type::Content::Flag)	flags++;
				if ((pointer + (squareNr - colNr))->visible == Type::Content::Flag)			flags++;
				if ((pointer + (squareNr - 1))->visible == Type::Content::Flag)				flags++;
				if ((pointer + (squareNr + (colNr - 1)))->visible == Type::Content::Flag)	flags++;
				if ((pointer + (squareNr + colNr))->visible == Type::Content::Flag)			flags++;
			}
			else if (squareNr < (objectNr - 1) && squareNr >(colNr * (rowNr - 1))) {
				if ((pointer + (squareNr - (colNr + 1)))->visible == Type::Content::Flag)	flags++;
				if ((pointer + (squareNr - colNr))->visible == Type::Content::Flag)			flags++;
				if ((pointer + (squareNr - (colNr - 1)))->visible == Type::Content::Flag)	flags++;
				if ((pointer + (squareNr - 1))->visible == Type::Content::Flag)				flags++;
				if ((pointer + (squareNr + 1))->visible == Type::Content::Flag)				flags++;
			}
			else if (squareNr == 0) {
				if ((pointer + (squareNr + 1))->visible == Type::Content::Flag)				flags++;
				if ((pointer + (squareNr + colNr))->visible == Type::Content::Flag)			flags++;
				if ((pointer + (squareNr + (colNr + 1)))->visible == Type::Content::Flag)	flags++;
			}
			else if (squareNr == (colNr - 1)) {
				if ((pointer + (squareNr - 1))->visible == Type::Content::Flag)				flags++;
				if ((pointer + (squareNr + (colNr - 1)))->visible == Type::Content::Flag)	flags++;
				if ((pointer + (squareNr + colNr))->visible == Type::Content::Flag)			flags++;
			}
			else if (squareNr == (colNr * (rowNr - 1))) {
				if ((pointer + (squareNr - colNr))->visible == Type::Content::Flag)			flags++;
				if ((pointer + (squareNr - (colNr - 1)))->visible == Type::Content::Flag)	flags++;
				if ((pointer + (squareNr + 1))->visible == Type::Content::Flag)				flags++;
			}
			else if (squareNr == (objectNr - 1)) {
				if ((pointer + (squareNr - (colNr + 1)))->visible == Type::Content::Flag)	flags++;
				if ((pointer + (squareNr - colNr))->visible == Type::Content::Flag)			flags++;
				if ((pointer + (squareNr - 1))->visible == Type::Content::Flag)				flags++;
			}

			int bomb(0);

			if ((pointer + squareNr)->visible <= flags) {
				if (squareNr > colNr && squareNr < ((colNr*(rowNr - 1) - 1)) && squareNr % colNr != 0 && (squareNr + 1) % colNr != 0) {
					if ((pointer + (squareNr - (colNr + 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - (colNr + 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - (colNr + 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - (colNr + 1)))->setVisible((pointer + (squareNr - (colNr + 1)))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr - colNr))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - colNr))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - colNr))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - colNr))->setVisible((pointer + (squareNr - colNr))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr - (colNr - 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - (colNr - 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - (colNr - 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - (colNr - 1)))->setVisible((pointer + (squareNr - (colNr - 1)))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr - 1))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - 1))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - 1))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - 1))->setVisible((pointer + (squareNr - 1))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + 1))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + 1))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + 1))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + 1))->setVisible((pointer + (squareNr + 1))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + (colNr - 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + (colNr - 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + (colNr - 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + (colNr - 1)))->setVisible((pointer + (squareNr + (colNr - 1)))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + colNr))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + colNr))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + colNr))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + colNr))->setVisible((pointer + (squareNr + colNr))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + (colNr + 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + (colNr + 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + (colNr + 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + (colNr + 1)))->setVisible((pointer + (squareNr + (colNr + 1)))->hidden);
							maxClick--;
						}
					}
				}
				else if (squareNr > 0 && squareNr < (colNr - 1)) {
					if ((pointer + (squareNr - 1))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - 1))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - 1))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - 1))->setVisible((pointer + (squareNr - 1))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + 1))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + 1))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + 1))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + 1))->setVisible((pointer + (squareNr + 1))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + (colNr - 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + (colNr - 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + (colNr - 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + (colNr - 1)))->setVisible((pointer + (squareNr + (colNr - 1)))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + colNr))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + colNr))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + colNr))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + colNr))->setVisible((pointer + (squareNr + colNr))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + (colNr + 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + (colNr + 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + (colNr + 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + (colNr + 1)))->setVisible((pointer + (squareNr + (colNr + 1)))->hidden);
							maxClick--;
						}
					}
				}
				else if (squareNr % colNr == 0 && squareNr != 0 && squareNr != (colNr * (rowNr - 1))) {
					if ((pointer + (squareNr - colNr))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - colNr))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - colNr))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - colNr))->setVisible((pointer + (squareNr - colNr))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr - (colNr - 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - (colNr - 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - (colNr - 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - (colNr - 1)))->setVisible((pointer + (squareNr - (colNr - 1)))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + 1))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + 1))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + 1))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + 1))->setVisible((pointer + (squareNr + 1))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + colNr))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + colNr))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + colNr))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + colNr))->setVisible((pointer + (squareNr + colNr))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + (colNr + 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + (colNr + 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + (colNr + 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + (colNr + 1)))->setVisible((pointer + (squareNr + (colNr + 1)))->hidden);
							maxClick--;
						}
					}
				}
				else if ((squareNr + 1) % colNr == 0 && squareNr != (colNr - 1) && squareNr != (objectNr - 1)) {
					if ((pointer + (squareNr - (colNr + 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - (colNr + 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - (colNr + 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - (colNr + 1)))->setVisible((pointer + (squareNr - (colNr + 1)))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr - colNr))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - colNr))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - colNr))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - colNr))->setVisible((pointer + (squareNr - colNr))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr - 1))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - 1))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - 1))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - 1))->setVisible((pointer + (squareNr - 1))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + (colNr - 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + (colNr - 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + (colNr - 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + (colNr - 1)))->setVisible((pointer + (squareNr + (colNr - 1)))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + colNr))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + colNr))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + colNr))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + colNr))->setVisible((pointer + (squareNr + colNr))->hidden);
							maxClick--;
						}
					}
				}
				else if (squareNr < (objectNr - 1) && squareNr >(colNr * (rowNr - 1))) {
					if ((pointer + (squareNr - (colNr + 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - (colNr + 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - (colNr + 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - (colNr + 1)))->setVisible((pointer + (squareNr - (colNr + 1)))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr - colNr))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - colNr))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - colNr))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - colNr))->setVisible((pointer + (squareNr - colNr))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr - (colNr - 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - (colNr - 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - (colNr - 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - (colNr - 1)))->setVisible((pointer + (squareNr - (colNr - 1)))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr - 1))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - 1))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - 1))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - 1))->setVisible((pointer + (squareNr - 1))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + 1))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + 1))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + 1))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + 1))->setVisible((pointer + (squareNr + 1))->hidden);
							maxClick--;
						}
					}
				}
				else if (squareNr == 0) {
					if ((pointer + (squareNr + 1))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + 1))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + 1))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + 1))->setVisible((pointer + (squareNr + 1))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + colNr))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + colNr))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + colNr))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + colNr))->setVisible((pointer + (squareNr + colNr))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + (colNr + 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + (colNr + 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + (colNr + 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + (colNr + 1)))->setVisible((pointer + (squareNr + (colNr + 1)))->hidden);
							maxClick--;
						}
					}
				}
				else if (squareNr == (colNr - 1)) {
					if ((pointer + (squareNr - 1))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - 1))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - 1))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - 1))->setVisible((pointer + (squareNr - 1))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + (colNr - 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + (colNr - 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + (colNr - 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + (colNr - 1)))->setVisible((pointer + (squareNr + (colNr - 1)))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + colNr))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + colNr))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + colNr))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + colNr))->setVisible((pointer + (squareNr + colNr))->hidden);
							maxClick--;
						}
					}
				}
				else if (squareNr == (colNr * (rowNr - 1))) {
					if ((pointer + (squareNr - colNr))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - colNr))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - colNr))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - colNr))->setVisible((pointer + (squareNr - colNr))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr - (colNr - 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - (colNr - 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - (colNr - 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - (colNr - 1)))->setVisible((pointer + (squareNr - (colNr - 1)))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr + 1))->visible == Type::Content::Field) {
						if ((pointer + (squareNr + 1))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr + 1))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr + 1))->setVisible((pointer + (squareNr + 1))->hidden);
							maxClick--;
						}
					}
				}
				else if (squareNr == (objectNr - 1)) {
					if ((pointer + (squareNr - (colNr + 1)))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - (colNr + 1)))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - (colNr + 1)))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - (colNr + 1)))->setVisible((pointer + (squareNr - (colNr + 1)))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr - colNr))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - colNr))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - colNr))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - colNr))->setVisible((pointer + (squareNr - colNr))->hidden);
							maxClick--;
						}
					}
					if ((pointer + (squareNr - 1))->visible == Type::Content::Field) {
						if ((pointer + (squareNr - 1))->hidden == Type::Content::Bomb) {
							(pointer + (squareNr - 1))->setVisible(Type::Content::BombExplode);
							bomb++;
						}
						else {
							(pointer + (squareNr - 1))->setVisible((pointer + (squareNr - 1))->hidden);
							maxClick--;
						}
					}
				}

				if (bomb > 0) {
					lose();
				}
				else {
					refresh();
				}
			}
		}
	}
	else if (button == Mouse::Button::Left) {
		if ((pointer + squareNr)->visible == Type::Content::Field) {
			if ((pointer + squareNr)->hidden != Type::Content::Bomb) {
				(pointer + squareNr)->setVisible((pointer + squareNr)->hidden);
				maxClick--;

				if ((pointer + squareNr)->visible == Type::Content::Empty) {
					refresh();
				}
			}
			else {
				(pointer + squareNr)->setVisible(Type::Content::BombExplode);
				lose();
			}
		}
	}
	if (maxClick == 0) {
		win();
	}
}

void Minesweeper::reset() {
	winG = false;
	loseG = false;

	for (int i = 0; i < objectNr; i++) {
		(pointer + i)->reset();
	}
	mineCounter.reset();
	emote.reset();
	banner.reset();

	mineCounter.setCounter(int(objectNr * 0.15));
	maxClick = objectNr - int(objectNr * 0.15);
	int l = mineCounter.counter;
	int a;
	while (l > 0) {
		do {
			a = rand() % (objectNr);
		} while ((pointer + a)->hidden == Type::Content::Bomb);
		(pointer + a)->hidden = Type::Content::Bomb;
		l--;
	}
	for (int i = 0; i < objectNr; i++) {
		if ((pointer + i)->hidden == Type::Content::Bomb) continue;

		int bombs(0);
		if (i > colNr && i < ((colNr*(rowNr - 1) - 1)) && i % colNr != 0 && (i + 1) % colNr != 0) {
			if ((pointer + (i - (colNr + 1)))->hidden == Type::Content::Bomb)	bombs++;
			if ((pointer + (i - colNr))->hidden == Type::Content::Bomb)			bombs++;
			if ((pointer + (i - (colNr - 1)))->hidden == Type::Content::Bomb)	bombs++;
			if ((pointer + (i - 1))->hidden == Type::Content::Bomb)				bombs++;
			if ((pointer + (i + 1))->hidden == Type::Content::Bomb)				bombs++;
			if ((pointer + (i + (colNr - 1)))->hidden == Type::Content::Bomb)	bombs++;
			if ((pointer + (i + colNr))->hidden == Type::Content::Bomb)			bombs++;
			if ((pointer + (i + (colNr + 1)))->hidden == Type::Content::Bomb)	bombs++;
		}
		else if (i > 0 && i < (colNr - 1)) {
			if ((pointer + (i - 1))->hidden == Type::Content::Bomb)				bombs++;
			if ((pointer + (i + 1))->hidden == Type::Content::Bomb)				bombs++;
			if ((pointer + (i + (colNr - 1)))->hidden == Type::Content::Bomb)	bombs++;
			if ((pointer + (i + colNr))->hidden == Type::Content::Bomb)			bombs++;
			if ((pointer + (i + (colNr + 1)))->hidden == Type::Content::Bomb)	bombs++;
		}
		else if (i % colNr == 0 && i != 0 && i != (colNr * (rowNr - 1))) {
			if ((pointer + (i - colNr))->hidden == Type::Content::Bomb)			bombs++;
			if ((pointer + (i - (colNr - 1)))->hidden == Type::Content::Bomb)	bombs++;
			if ((pointer + (i + 1))->hidden == Type::Content::Bomb)				bombs++;
			if ((pointer + (i + colNr))->hidden == Type::Content::Bomb)			bombs++;
			if ((pointer + (i + (colNr + 1)))->hidden == Type::Content::Bomb)	bombs++;
		}
		else if ((i + 1) % colNr == 0 && i != (colNr - 1) && i != (objectNr - 1)) {
			if ((pointer + (i - (colNr + 1)))->hidden == Type::Content::Bomb)	bombs++;
			if ((pointer + (i - colNr))->hidden == Type::Content::Bomb)			bombs++;
			if ((pointer + (i - 1))->hidden == Type::Content::Bomb)				bombs++;
			if ((pointer + (i + (colNr - 1)))->hidden == Type::Content::Bomb)	bombs++;
			if ((pointer + (i + colNr))->hidden == Type::Content::Bomb)			bombs++;
		}
		else if (i < (objectNr - 1) && i >(colNr * (rowNr - 1))) {
			if ((pointer + (i - (colNr + 1)))->hidden == Type::Content::Bomb)	bombs++;
			if ((pointer + (i - colNr))->hidden == Type::Content::Bomb)			bombs++;
			if ((pointer + (i - (colNr - 1)))->hidden == Type::Content::Bomb)	bombs++;
			if ((pointer + (i - 1))->hidden == Type::Content::Bomb)				bombs++;
			if ((pointer + (i + 1))->hidden == Type::Content::Bomb)				bombs++;
		}
		else if (i == 0) {
			if ((pointer + (i + 1))->hidden == Type::Content::Bomb)				bombs++;
			if ((pointer + (i + colNr))->hidden == Type::Content::Bomb)			bombs++;
			if ((pointer + (i + (colNr + 1)))->hidden == Type::Content::Bomb)	bombs++;
		}
		else if (i == (colNr - 1)) {
			if ((pointer + (i - 1))->hidden == Type::Content::Bomb)				bombs++;
			if ((pointer + (i + (colNr - 1)))->hidden == Type::Content::Bomb)	bombs++;
			if ((pointer + (i + colNr))->hidden == Type::Content::Bomb)			bombs++;
		}
		else if (i == (colNr * (rowNr - 1))) {
			if ((pointer + (i - colNr))->hidden == Type::Content::Bomb)			bombs++;
			if ((pointer + (i - (colNr - 1)))->hidden == Type::Content::Bomb)	bombs++;
			if ((pointer + (i + 1))->hidden == Type::Content::Bomb)				bombs++;
		}
		else if (i == (objectNr - 1)) {
			if ((pointer + (i - (colNr + 1)))->hidden == Type::Content::Bomb)	bombs++;
			if ((pointer + (i - colNr))->hidden == Type::Content::Bomb)			bombs++;
			if ((pointer + (i - 1))->hidden == Type::Content::Bomb)				bombs++;
		}

		switch (bombs) {
		case 0:
			(pointer + i)->hidden = Type::Content::Empty;
			break;
		case 1:
			(pointer + i)->hidden = Type::Content::One;
			break;
		case 2:
			(pointer + i)->hidden = Type::Content::Two;
			break;
		case 3:
			(pointer + i)->hidden = Type::Content::Three;
			break;
		case 4:
			(pointer + i)->hidden = Type::Content::Four;
			break;
		case 5:
			(pointer + i)->hidden = Type::Content::Five;
			break;
		case 6:
			(pointer + i)->hidden = Type::Content::Six;
			break;
		case 7:
			(pointer + i)->hidden = Type::Content::Seven;
			break;
		case 8:
			(pointer + i)->hidden = Type::Content::Eight;
			break;
		}
	}
}

void Minesweeper::renderMape(RenderWindow& window) {
	for (int i = 0; i < objectNr; i++) {
		window.draw((pointer + i)->rectangle);
	}
	window.draw(mineCounter.rectangle0);
	window.draw(mineCounter.rectangle1);
	window.draw(mineCounter.rectangle2);
	window.draw(emote.rectangle);
	window.draw(banner.rectangle);
}

void Minesweeper::lose() {
	emote.setSad();
	banner.setBanner(Type::Banner::Lose);
	loseG = true;
	for (int i = 0; i < objectNr; i++) {
		if ((pointer + i)->visible != Type::Content::Field && (pointer + i)->visible != Type::Content::Flag) continue;
		else if ((pointer + i)->visible == Type::Field) {
			(pointer + i)->setVisible((pointer + i)->hidden);
		}
		else if ((pointer + i)->visible == Type::Content::Flag && (pointer + i)->hidden != Type::Content::Bomb) {
			(pointer + i)->setVisible(Type::Content::BombMiss);
		}
	}
}

void Minesweeper::win() {
	banner.setBanner(Type::Banner::Win);
	winG = true;
	if (mineCounter.counter != 0) {
		for (int i = 0; i < objectNr; i++) {
			if ((pointer + i)->visible == Type::Content::Field) {
				(pointer + i)->setVisible(Type::Content::Flag);
				mineCounter.setCounter(mineCounter.counter - 1);
			}
		}
	}
}

void Minesweeper::refresh() {
	int a;
	do {
		a = 0;

		for (int i = 0; i < objectNr; i++) {
			if ((pointer + i)->visible != Type::Content::Empty) continue;

			if (i > colNr && i < ((colNr*(rowNr - 1) - 1)) && i % colNr != 0 && (i + 1) % colNr != 0) {
				if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Field) {
					(pointer + (i - (colNr + 1)))->setVisible((pointer + (i - (colNr + 1)))->hidden);
					a++;
				}
				if ((pointer + (i - colNr))->visible == Type::Content::Field) {
					(pointer + (i - colNr))->setVisible((pointer + (i - colNr))->hidden);
					a++;
				}
				if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Field) {
					(pointer + (i - (colNr - 1)))->setVisible((pointer + (i - (colNr - 1)))->hidden);
					a++;
				}
				if ((pointer + (i - 1))->visible == Type::Content::Field) {
					(pointer + (i - 1))->setVisible((pointer + (i - 1))->hidden);
					a++;
				}
				if ((pointer + (i + 1))->visible == Type::Content::Field) {
					(pointer + (i + 1))->setVisible((pointer + (i + 1))->hidden);
					a++;
				}
				if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Field) {
					(pointer + (i + (colNr - 1)))->setVisible((pointer + (i + (colNr - 1)))->hidden);
					a++;
				}
				if ((pointer + (i + colNr))->visible == Type::Content::Field) {
					(pointer + (i + colNr))->setVisible((pointer + (i + colNr))->hidden);
					a++;
				}
				if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Field) {
					(pointer + (i + (colNr + 1)))->setVisible((pointer + (i + (colNr + 1)))->hidden);
					a++;
				}
			}
			else if (i > 0 && i < (colNr - 1)) {
				if ((pointer + (i - 1))->visible == Type::Content::Field) {
					(pointer + (i - 1))->setVisible((pointer + (i - 1))->hidden);
					a++;
				}
				if ((pointer + (i + 1))->visible == Type::Content::Field) {
					(pointer + (i + 1))->setVisible((pointer + (i + 1))->hidden);
					a++;
				}
				if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Field) {
					(pointer + (i + (colNr - 1)))->setVisible((pointer + (i + (colNr - 1)))->hidden);
					a++;
				}
				if ((pointer + (i + colNr))->visible == Type::Content::Field) {
					(pointer + (i + colNr))->setVisible((pointer + (i + colNr))->hidden);
					a++;
				}
				if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Field) {
					(pointer + (i + (colNr + 1)))->setVisible((pointer + (i + (colNr + 1)))->hidden);
					a++;
				}
			}
			else if (i % colNr == 0 && i != 0 && i != (colNr * (rowNr - 1))) {
				if ((pointer + (i - colNr))->visible == Type::Content::Field) {
					(pointer + (i - colNr))->setVisible((pointer + (i - colNr))->hidden);
					a++;
				}
				if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Field) {
					(pointer + (i - (colNr - 1)))->setVisible((pointer + (i - (colNr - 1)))->hidden);
					a++;
				}
				if ((pointer + (i + 1))->visible == Type::Content::Field) {
					(pointer + (i + 1))->setVisible((pointer + (i + 1))->hidden);
					a++;
				}
				if ((pointer + (i + colNr))->visible == Type::Content::Field) {
					(pointer + (i + colNr))->setVisible((pointer + (i + colNr))->hidden);
					a++;
				}
				if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Field) {
					(pointer + (i + (colNr + 1)))->setVisible((pointer + (i + (colNr + 1)))->hidden);
					a++;
				}
			}
			else if ((i + 1) % colNr == 0 && i != (colNr - 1) && i != (objectNr - 1)) {
				if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Field) {
					(pointer + (i - (colNr + 1)))->setVisible((pointer + (i - (colNr + 1)))->hidden);
					a++;
				}
				if ((pointer + (i - colNr))->visible == Type::Content::Field) {
					(pointer + (i - colNr))->setVisible((pointer + (i - colNr))->hidden);
					a++;
				}
				if ((pointer + (i - 1))->visible == Type::Content::Field) {
					(pointer + (i - 1))->setVisible((pointer + (i - 1))->hidden);
					a++;
				}
				if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Field) {
					(pointer + (i + (colNr - 1)))->setVisible((pointer + (i + (colNr - 1)))->hidden);
					a++;
				}
				if ((pointer + (i + colNr))->visible == Type::Content::Field) {
					(pointer + (i + colNr))->setVisible((pointer + (i + colNr))->hidden);
					a++;
				}
			}
			else if (i < (objectNr - 1) && i >(colNr * (rowNr - 1))) {
				if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Field) {
					(pointer + (i - (colNr + 1)))->setVisible((pointer + (i - (colNr + 1)))->hidden);
					a++;
				}
				if ((pointer + (i - colNr))->visible == Type::Content::Field) {
					(pointer + (i - colNr))->setVisible((pointer + (i - colNr))->hidden);
					a++;
				}
				if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Field) {
					(pointer + (i - (colNr - 1)))->setVisible((pointer + (i - (colNr - 1)))->hidden);
					a++;
				}
				if ((pointer + (i - 1))->visible == Type::Content::Field) {
					(pointer + (i - 1))->setVisible((pointer + (i - 1))->hidden);
					a++;
				}
				if ((pointer + (i + 1))->visible == Type::Content::Field) {
					(pointer + (i + 1))->setVisible((pointer + (i + 1))->hidden);
					a++;
				}
			}
			else if (i == 0) {
				if ((pointer + (i + 1))->visible == Type::Content::Field) {
					(pointer + (i + 1))->setVisible((pointer + (i + 1))->hidden);
					a++;
				}
				if ((pointer + (i + colNr))->visible == Type::Content::Field) {
					(pointer + (i + colNr))->setVisible((pointer + (i + colNr))->hidden);
					a++;
				}
				if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Field) {
					(pointer + (i + (colNr + 1)))->setVisible((pointer + (i + (colNr + 1)))->hidden);
					a++;
				}
			}
			else if (i == (colNr - 1)) {
				if ((pointer + (i - 1))->visible == Type::Content::Field) {
					(pointer + (i - 1))->setVisible((pointer + (i - 1))->hidden);
					a++;
				}
				if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Field) {
					(pointer + (i + (colNr - 1)))->setVisible((pointer + (i + (colNr - 1)))->hidden);
					a++;
				}
				if ((pointer + (i + colNr))->visible == Type::Content::Field) {
					(pointer + (i + colNr))->setVisible((pointer + (i + colNr))->hidden);
					a++;
				}
			}
			else if (i == (colNr * (rowNr - 1))) {
				if ((pointer + (i - colNr))->visible == Type::Content::Field) {
					(pointer + (i - colNr))->setVisible((pointer + (i - colNr))->hidden);
					a++;
				}
				if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Field) {
					(pointer + (i - (colNr - 1)))->setVisible((pointer + (i - (colNr - 1)))->hidden);
					a++;
				}
				if ((pointer + (i + 1))->visible == Type::Content::Field) {
					(pointer + (i + 1))->setVisible((pointer + (i + 1))->hidden);
					a++;
				}
			}
			else if (i == (objectNr - 1)) {
				if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Field) {
					(pointer + (i - (colNr + 1)))->setVisible((pointer + (i - (colNr + 1)))->hidden);
					a++;
				}
				if ((pointer + (i - colNr))->visible == Type::Content::Field) {
					(pointer + (i - colNr))->setVisible((pointer + (i - colNr))->hidden);
					a++;
				}
				if ((pointer + (i - 1))->visible == Type::Content::Field) {
					(pointer + (i - 1))->setVisible((pointer + (i - 1))->hidden);
					a++;
				}
			}
		}
		maxClick -= a;
	} while (a > 0);
}