#ifndef INCLUDE_STATE_H
#define INCLUDE_STATE_H

#include "Array2D.h"

class Image;
class File;

class State {
public:
	State();
	~State();
	void update(int moveX, int moveY);
	void draw() const;
	void drawDebug() const;
	bool checkIsClear();
	void reset();
	
private:
	class Object;
	/*
	enum Object {
		OBJ_PLAYER,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_SPACE,
		OBJ_GOAL,

		OBJ_UNKNOWN,
	};

	enum ImageID {
		IMAGE_ID_PLAYER,
		IMAGE_ID_WALL,
		IMAGE_ID_BLOCK,
		IMAGE_ID_SPACE,
		IMAGE_ID_GOAL,
	};
	*/

	void setSize();

	int mWidth;
	int mHeight;
	Array2D<Object>mObjects;
	Image* mImage;
	const char* stageData;
	int size;
	bool canGetInput;
	int mMoveCount;
};

#endif