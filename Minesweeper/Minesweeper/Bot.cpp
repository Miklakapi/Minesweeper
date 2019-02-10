#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "Minesweeper.hpp"

using namespace sf;

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
									//Click

Click::Click(int number, Mouse::Button button) {
	this->number = number;
	this->button = button;
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
									//Bot

Bot::Bot(Minesweeper* minesweeper) {
	this->minesweeper = minesweeper;
	pointer = minesweeper->pointer;
	objectNr = minesweeper->objectNr;
	colNr = minesweeper->colNr;
	rowNr = minesweeper->rowNr;
	stop = true;
}

Click Bot::checkRMB() {
	if (clock.getElapsedTime().asSeconds() < delay || minesweeper->winG == true || minesweeper->loseG == true || stop == true) return Click{ -1, Mouse::Button::Middle };

	clock.restart();

	for (int i = 0; i < objectNr; i++) {
		if ((pointer + i)->visible == Type::Content::Field || (pointer + i)->visible == Type::Content::Flag || (pointer + i)->visible == Type::Content::Empty) continue;

		int flags(0);
		int field(0);

		if (i > colNr && i < ((colNr*(rowNr - 1) - 1)) && i % colNr != 0 && (i + 1) % colNr != 0) {
			if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Field)		field++;
			if ((pointer + (i - colNr))->visible == Type::Content::Flag)			flags++;
			if ((pointer + (i - colNr))->visible == Type::Content::Field)			field++;
			if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Field)		field++;
			if ((pointer + (i - 1))->visible == Type::Content::Flag)				flags++;
			if ((pointer + (i - 1))->visible == Type::Content::Field)				field++;
			if ((pointer + (i + 1))->visible == Type::Content::Flag)				flags++;
			if ((pointer + (i + 1))->visible == Type::Content::Field)				field++;
			if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Field)		field++;
			if ((pointer + (i + colNr))->visible == Type::Content::Flag)			flags++;
			if ((pointer + (i + colNr))->visible == Type::Content::Field)			field++;
			if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Field)		field++;
		}
		else if (i > 0 && i < (colNr - 1)) {
			if ((pointer + (i - 1))->visible == Type::Content::Flag)				flags++;
			if ((pointer + (i - 1))->visible == Type::Content::Field)				field++;
			if ((pointer + (i + 1))->visible == Type::Content::Flag)				flags++;
			if ((pointer + (i + 1))->visible == Type::Content::Field)				field++;
			if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Field)		field++;
			if ((pointer + (i + colNr))->visible == Type::Content::Flag)			flags++;
			if ((pointer + (i + colNr))->visible == Type::Content::Field)			field++;
			if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Field)		field++;
		}
		else if (i % colNr == 0 && i != 0 && i != (colNr * (rowNr - 1))) {
			if ((pointer + (i - colNr))->visible == Type::Content::Flag)			flags++;
			if ((pointer + (i - colNr))->visible == Type::Content::Field)			field++;
			if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Field)		field++;
			if ((pointer + (i + 1))->visible == Type::Content::Flag)				flags++;
			if ((pointer + (i + 1))->visible == Type::Content::Field)				field++;
			if ((pointer + (i + colNr))->visible == Type::Content::Flag)			flags++;
			if ((pointer + (i + colNr))->visible == Type::Content::Field)			field++;
			if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Field)		field++;
		}
		else if ((i + 1) % colNr == 0 && i != (colNr - 1) && i != (objectNr - 1)) {
			if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Field)		field++;
			if ((pointer + (i - colNr))->visible == Type::Content::Flag)			flags++;
			if ((pointer + (i - colNr))->visible == Type::Content::Field)			field++;
			if ((pointer + (i - 1))->visible == Type::Content::Flag)				flags++;
			if ((pointer + (i - 1))->visible == Type::Content::Field)				field++;
			if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Field)		field++;
			if ((pointer + (i + colNr))->visible == Type::Content::Flag)			flags++;
			if ((pointer + (i + colNr))->visible == Type::Content::Field)			field++;
		}
		else if (i < (objectNr - 1) && i >(colNr * (rowNr - 1))) {
			if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Field)		field++;
			if ((pointer + (i - colNr))->visible == Type::Content::Flag)			flags++;
			if ((pointer + (i - colNr))->visible == Type::Content::Field)			field++;
			if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Field)		field++;
			if ((pointer + (i - 1))->visible == Type::Content::Flag)				flags++;
			if ((pointer + (i - 1))->visible == Type::Content::Field)				field++;
			if ((pointer + (i + 1))->visible == Type::Content::Flag)				flags++;
			if ((pointer + (i + 1))->visible == Type::Content::Field)				field++;
		}
		else if (i == 0) {
			if ((pointer + (i + 1))->visible == Type::Content::Flag)				flags++;
			if ((pointer + (i + 1))->visible == Type::Content::Field)				field++;
			if ((pointer + (i + colNr))->visible == Type::Content::Flag)			flags++;
			if ((pointer + (i + colNr))->visible == Type::Content::Field)			field++;
			if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Field)		field++;
		}
		else if (i == (colNr - 1)) {
			if ((pointer + (i - 1))->visible == Type::Content::Flag)				flags++;
			if ((pointer + (i - 1))->visible == Type::Content::Field)				field++;
			if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Field)		field++;
			if ((pointer + (i + colNr))->visible == Type::Content::Flag)			flags++;
			if ((pointer + (i + colNr))->visible == Type::Content::Field)			field++;
		}
		else if (i == (colNr * (rowNr - 1))) {
			if ((pointer + (i - colNr))->visible == Type::Content::Flag)			flags++;
			if ((pointer + (i - colNr))->visible == Type::Content::Field)			field++;
			if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Field)		field++;
			if ((pointer + (i + 1))->visible == Type::Content::Flag)				flags++;
			if ((pointer + (i + 1))->visible == Type::Content::Field)				field++;
		}
		else if (i == (objectNr - 1)) {
			if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Flag)		flags++;
			if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Field)		field++;
			if ((pointer + (i - colNr))->visible == Type::Content::Flag)			flags++;
			if ((pointer + (i - colNr))->visible == Type::Content::Field)			field++;
			if ((pointer + (i - 1))->visible == Type::Content::Flag)				flags++;
			if ((pointer + (i - 1))->visible == Type::Content::Field)				field++;
		}
		//////////////////
		if ((pointer + i)->visible == flags && field > 0) {
			return Click{ i, Mouse::Button::Right };
		}
		///////////////////
		if (((pointer + i)->visible - flags) == field) {

			if (i > colNr && i < ((colNr*(rowNr - 1) - 1)) && i % colNr != 0 && (i + 1) % colNr != 0) {
				if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Field)		return Click{ (i - (colNr + 1)), Mouse::Button::Right };
				if ((pointer + (i - colNr))->visible == Type::Content::Field)			return Click{ (i - colNr), Mouse::Button::Right };
				if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Field)		return Click{ (i - (colNr - 1)), Mouse::Button::Right };
				if ((pointer + (i - 1))->visible == Type::Content::Field)				return Click{ (i - 1), Mouse::Button::Right };
				if ((pointer + (i + 1))->visible == Type::Content::Field)				return Click{ (i + 1), Mouse::Button::Right };
				if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Field)		return Click{ (i + (colNr - 1)), Mouse::Button::Right };
				if ((pointer + (i + colNr))->visible == Type::Content::Field)			return Click{ (i + colNr), Mouse::Button::Right };
				if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Field)		return Click{ (i + (colNr + 1)), Mouse::Button::Right };
			}
			else if (i > 0 && i < (colNr - 1)) {
				if ((pointer + (i - 1))->visible == Type::Content::Field)				return Click{ (i - 1), Mouse::Button::Right };
				if ((pointer + (i + 1))->visible == Type::Content::Field)				return Click{ (i + 1), Mouse::Button::Right };
				if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Field)		return Click{ (i + (colNr - 1)), Mouse::Button::Right };
				if ((pointer + (i + colNr))->visible == Type::Content::Field)			return Click{ (i + colNr), Mouse::Button::Right };
				if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Field)		return Click{ (i + (colNr + 1)), Mouse::Button::Right };
			}
			else if (i % colNr == 0 && i != 0 && i != (colNr * (rowNr - 1))) {
				if ((pointer + (i - colNr))->visible == Type::Content::Field)			return Click{ (i - colNr), Mouse::Button::Right };
				if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Field)		return Click{ (i - (colNr - 1)), Mouse::Button::Right };
				if ((pointer + (i + 1))->visible == Type::Content::Field)				return Click{ (i + 1), Mouse::Button::Right };
				if ((pointer + (i + colNr))->visible == Type::Content::Field)			return Click{ (i + colNr), Mouse::Button::Right };
				if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Field)		return Click{ (i + (colNr + 1)), Mouse::Button::Right };
			}
			else if ((i + 1) % colNr == 0 && i != (colNr - 1) && i != (objectNr - 1)) {
				if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Field)		return Click{ (i - (colNr + 1)), Mouse::Button::Right };
				if ((pointer + (i - colNr))->visible == Type::Content::Field)			return Click{ (i - colNr), Mouse::Button::Right };
				if ((pointer + (i - 1))->visible == Type::Content::Field)				return Click{ (i - 1), Mouse::Button::Right };
				if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Field)		return Click{ (i + (colNr - 1)), Mouse::Button::Right };
				if ((pointer + (i + colNr))->visible == Type::Content::Field)			return Click{ (i + colNr), Mouse::Button::Right };
			}
			else if (i < (objectNr - 1) && i >(colNr * (rowNr - 1))) {
				if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Field)		return Click{ (i - (colNr + 1)), Mouse::Button::Right };
				if ((pointer + (i - colNr))->visible == Type::Content::Field)			return Click{ (i - colNr), Mouse::Button::Right };
				if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Field)		return Click{ (i - (colNr - 1)), Mouse::Button::Right };
				if ((pointer + (i - 1))->visible == Type::Content::Field)				return Click{ (i - 1), Mouse::Button::Right };
				if ((pointer + (i + 1))->visible == Type::Content::Field)				return Click{ (i + 1), Mouse::Button::Right };
			}
			else if (i == 0) {
				if ((pointer + (i + 1))->visible == Type::Content::Field)				return Click{ (i + 1), Mouse::Button::Right };
				if ((pointer + (i + colNr))->visible == Type::Content::Field)			return Click{ (i + colNr), Mouse::Button::Right };
				if ((pointer + (i + (colNr + 1)))->visible == Type::Content::Field)		return Click{ (i + (colNr + 1)), Mouse::Button::Right };
			}
			else if (i == (colNr - 1)) {
				if ((pointer + (i - 1))->visible == Type::Content::Field)				return Click{ (i - 1), Mouse::Button::Right };
				if ((pointer + (i + (colNr - 1)))->visible == Type::Content::Field)		return Click{ (i + (colNr - 1)), Mouse::Button::Right };
				if ((pointer + (i + colNr))->visible == Type::Content::Field)			return Click{ (i + colNr), Mouse::Button::Right };
			}
			else if (i == (colNr * (rowNr - 1))) {
				if ((pointer + (i - colNr))->visible == Type::Content::Field)			return Click{ (i - colNr), Mouse::Button::Right };
				if ((pointer + (i - (colNr - 1)))->visible == Type::Content::Field)		return Click{ (i - (colNr - 1)), Mouse::Button::Right };
				if ((pointer + (i + 1))->visible == Type::Content::Field)				return Click{ (i + 1), Mouse::Button::Right };
			}
			else if (i == (objectNr - 1)) {
				if ((pointer + (i - (colNr + 1)))->visible == Type::Content::Field)		return Click{ (i - (colNr + 1)), Mouse::Button::Right };
				if ((pointer + (i - colNr))->visible == Type::Content::Field)			return Click{ (i - colNr), Mouse::Button::Right };
				if ((pointer + (i - 1))->visible == Type::Content::Field)				return Click{ (i - 1), Mouse::Button::Right };
			}
		}

	}
	int k;
	do {
		k = rand() % objectNr;
	} while ((pointer + k)->visible != Type::Content::Field);
	return Click{ k, Mouse::Button::Left };
}

void Bot::changeSpeed(float delay) {
	this->delay = delay;
}

void Bot::setMouse(int number, RenderWindow& window) {
	Mouse::setPosition(Vector2i{ int((pointer + number)->rectangle.getPosition().x + (pointer + number)->rectangle.getSize().x / 2),
		int((pointer + number)->rectangle.getPosition().y + (pointer + number)->rectangle.getSize().y / 2) }, window);
}

void Bot::switchBot() {
	if (stop == false) stop = true;
	else stop = false;
}

void Bot::turnOn() {
	stop = false;
}

void Bot::turnOff() {
	stop = true;
}