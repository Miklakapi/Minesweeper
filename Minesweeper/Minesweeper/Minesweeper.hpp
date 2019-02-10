#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

using namespace sf;

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

namespace Type {

	enum Content {
		Empty,
		One,
		Two,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Bomb,
		BombMiss,
		BombExplode,
		Field,
		Flag
	};

	enum Counter {
		ZeroC,
		OneC,
		TwoC,
		ThreeC,
		FourC,
		FiveC,
		SixC,
		SevenC,
		EightC,
		NineC
	};

	enum Face {
		Smile,
		Sad
	};

	enum Banner {
		Null,
		Win,
		Lose
	};
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

struct Field {

	//Position left-top corner
	Vector2f left;

	//Position right-bottom corner
	Vector2f right;
};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

class Rectangle {

public:

	//RectangleShape to draw
	RectangleShape rectangle;

	//Visible Content
	Type::Content visible;

	//Hidden Content
	Type::Content hidden;

	//Texture IntRect
	IntRect* rect;

	//

	//Set visible content
	void setVisible(Type::Content mode);

	//Reset to default options
	void reset();
};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

class MineCounter {

public:

	//RectangleShape (Left) to draw
	RectangleShape rectangle0;

	//RectangleShape (Middle) to draw
	RectangleShape rectangle1;

	//RectangleShape (Right) to draw
	RectangleShape rectangle2;

	//Stores the numbers of bombs
	Int16 counter;

	//Texture IntRect
	IntRect* rect;

	//

	//Sets position
	void setPosition(Vector2f position, Vector2f size);

	//Sets the numbers of bombs
	void setCounter(Int16 counter);

	//Reset to default options
	void reset();
};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

class Emote {

public:

	//RectangleShape to draw
	RectangleShape rectangle;

	//Texture IntRect
	IntRect* rect;

	//

	//Sets texture to Smile Face
	void setSmile();

	//Sets texture to Sad Face
	void setSad();

	//Reset to default options
	void reset();
};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

class Banner {

public:

	//RectangleShape to draw
	RectangleShape rectangle;

	//Texture IntRect
	IntRect* rect;

	//

	//Sets banner texture
	void setBanner(Type::Banner banner);

	//Reset to default options
	void reset();
};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

class Minesweeper {

	//Stores pointer to Rectangle Array
	Rectangle* pointer;

	//Stores the numbers of Rectangle Array elements
	int objectNr;

	//Stores the number of Rectangle without bomb
	int maxClick;

	//Stores the numbers of columns
	int colNr;

	//Stores the numbers of rows
	int rowNr;

	//MineCounter object
	MineCounter mineCounter;

	//Emote object
	Emote emote;

	//Banner object
	Banner banner;

	//Stores the result of the match
	bool winG;
	bool loseG;

	//Stores mine counter size
	Vector2f counterSize;

	//Stores field 
	Field areaField;
	Field counterField;
	Field emoteField;
	Field banerField;

	//

	//Lose function
	void lose();

	//Win function
	void win();

	//Refresh function
	void refresh();

public:

	//Construct
	Minesweeper(Vector2f position, Vector2f size, Vector2f textureSize);

	//

	//Sets elements textures
	void loadTextures(Texture* rectangle, Texture* counter, Texture* emoticon, Texture* baner);

	//Sets elements IntRects
	void loadIntRect(IntRect* rectangleRect, IntRect* counterRect, IntRect* emoteRect, IntRect* banerRect);

	//Sets elements size
	void setSize(Vector2f counterSize, Vector2f emoteSize, Vector2f banerSize);

	//Sets elements position
	void setPosition(Vector2f counterPos, Vector2f emotePos, Vector2f banerPos);

	//

	//Returns the number of element in Rectangle Array
	int getNrRectangle(Vector2i MousePosition);

	//Return 1 if clicked on element of Rectangle Array
	//Return 0 if clicked on face (Emote)
	//Return -1 if nothing is clicked
	int click(Vector2i MousePosition);

	//The function responsible for click on element of Rectangle Array
	void areaClick(int squareNr, Mouse::Button button);

	//Reset to default options
	void reset();

	//Render saper game
	void renderMape(RenderWindow& window);

	//

	//Friend class
	friend class Bot;

};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

struct Click {

	//Stores mouse button
	Mouse::Button button;

	//Stores number of element in Rectangle Array
	int number;

	//Construct
	Click(int number, Mouse::Button button);
};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

class Bot {

	//Stores pointer to Rectangle Array
	Rectangle *pointer;

	//Stores the numbers of Rectangle Array elements
	int objectNr;

	//Stores the numbers of columns
	int colNr;

	//Stores the numbers of rows
	int rowNr;

	//Stores the result of the match
	bool winG;
	bool loseG;

	//Stores time
	Clock clock;

	//Stores pointer to saper
	Minesweeper* minesweeper;

	//Stores delay
	float delay;

	//True - bot is stopped
	//False - bot is runned
	bool stop;

public:

	//Construct
	Bot(Minesweeper * saper);

	//
	Click checkRMB();

	//Changes bot speed
	void changeSpeed(float delay);

	//Sets mouse position on bot move
	void setMouse(int number, RenderWindow& window);

	//Turn on / off bot
	void switchBot();

	void turnOn();

	void turnOff();

};