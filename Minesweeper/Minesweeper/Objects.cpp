#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "Minesweeper.hpp"

using namespace sf;

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
									//Rectangle

void Rectangle::setVisible(Type::Content mode) {
	visible = mode;
	rectangle.setTextureRect(*(rect + mode));
}

void Rectangle::reset() {
	setVisible(Type::Content::Field);
	hidden = Type::Content::Empty;
}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
									//Mine Counter

void MineCounter::setPosition(Vector2f position, Vector2f size) {
	rectangle0.setSize(size);
	rectangle0.setPosition(position);

	position.x += size.x + 3;
	rectangle1.setSize(size);
	rectangle1.setPosition(position);

	position.x += size.x + 3;
	rectangle2.setSize(size);
	rectangle2.setPosition(position);
}

void MineCounter::setCounter(Int16 counter) {
	this->counter = counter;

	Int16 number2 = counter % 10;
	Int16 number1 = int((counter % 100) / 10);
	Int16 number0 = int(counter / 100);

	rectangle0.setTextureRect(*(rect + number0));
	rectangle1.setTextureRect(*(rect + number1));
	rectangle2.setTextureRect(*(rect + number2));
}

void MineCounter::reset() {
	setCounter(0);
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
									//Emote

void Emote::setSmile() {
	rectangle.setTextureRect(*rect);
}

void Emote::setSad() {
	rectangle.setTextureRect(*(rect + 1));
}

void Emote::reset() {
	setSmile();
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
									//Banner

void Banner::setBanner(Type::Banner banner) {
	switch (banner) {
	case Type::Banner::Null:
		rectangle.setTextureRect(*rect);
		break;
	case Type::Banner::Win:
		rectangle.setTextureRect(*(rect + 1));
		break;
	case Type::Banner::Lose:
		rectangle.setTextureRect(*(rect + 2));
		break;
	}
}

void Banner::reset() {
	setBanner(Type::Banner::Null);
}