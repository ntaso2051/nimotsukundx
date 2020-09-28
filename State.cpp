#include "State.h"
#include "Image.h"
#include "File.h"
#include "DxLib.h"

class State::Object {
public:
	enum Type {
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

	Object() : mType(OBJ_WALL), mGoalFlag(false), mMoveX(0), mMoveY(0) {}

	void set(char c) {
		switch (c) {
		case '#': mType = OBJ_WALL; break;
		case ' ': mType = OBJ_SPACE; break;
		case 'o': mType = OBJ_BLOCK; break;
		case 'O': mType = OBJ_BLOCK; mGoalFlag = true; break;
		case '.': mType = OBJ_SPACE; mGoalFlag = true; break;
		case 'p': mType = OBJ_PLAYER; break;
		case 'P': mType = OBJ_PLAYER; mGoalFlag = true; break;
		// case '\n': x = 0; y++; t = OBJ_UNKNOWN; break;
		// default: t = OBJ_UNKNOWN; break;
		}
	}

	void drawBackGround(int x, int y, Image* image) const {
		ImageID id = IMAGE_ID_SPACE;
		if (mType == OBJ_WALL) {
			image->draw(x, y, 0, 0, IMAGE_ID_WALL);
		}
		else {
			if (mGoalFlag) {
				image->draw(x, y, 0, 0, IMAGE_ID_SPACE);
				image->draw(x, y, 0, 0, IMAGE_ID_GOAL);
			}
			else {
				image->draw(x, y, 0, 0, IMAGE_ID_SPACE);
			}
		}
	}

	void drawForeground(int x, int y, const Image* image, int moveCount) const {
		ImageID id = IMAGE_ID_SPACE;
		if (mType == OBJ_BLOCK) {
			id = IMAGE_ID_BLOCK;
		}
		else if (mType == OBJ_PLAYER) {
			id = IMAGE_ID_PLAYER;
		}
		if (id != IMAGE_ID_SPACE) {
			int dx = mMoveX * (32 - moveCount);
			int dy = mMoveY * (32 - moveCount);
			image->draw(x, y, dx, dy, id);
		}
	}

	void move(int dx, int dy) {
		mMoveX = dx;
		mMoveY = dy;
	}

	Type mType;
	bool mGoalFlag;
	int mMoveX;
	int mMoveY;
};

State::State() : mImage(0), stageData(0), canGetInput(true), mMoveCount(0){
	canGetInput = true;
	File file("source/stage/stageData.txt");
	stageData = file.data();
	size = file.size();
	setSize();
	mObjects.setSize(mWidth, mHeight);
	/*
	for (int y = 0; y < mHeight; y++) {
		for (int x = 0; x < mWidth; x++) {
			mObjects(x, y) = OBJ_WALL;
			mGoalFlags(x, y) = false;
		}
	}
	*/
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; i++) {
		Object t;
		bool goalFlag = false;
		switch (stageData[i]) {
		case '#': case ' ': case 'o': case 'O':
		case '.': case 'p': case 'P':
			mObjects(x, y).set(stageData[i]);
			++x;
			break;
		case '\n': x = 0; ++y; break;
		}
	}
	mImage = new Image("source/image/nimotsukun.png", 5, 5, 1, 32, 32);
}

State::~State() {
	delete[] mImage;
	mImage = 0;
	delete[] stageData;
	stageData = 0;
}

void State::setSize() {
	mWidth = mHeight = 0;
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; i++) {
		switch (stageData[i]) {
		case '#': case ' ': case 'o': case 'O': case '.': case 'p': case 'P':
			x++;
			break;
		case '\n':
			y++;
			mWidth = (mWidth > x) ? mWidth : x;
			mHeight = (mHeight > y) ? mHeight : y;
			x = 0;
			break;
		}
	}
}

void State::draw() const {
	for (int y = 0; y < mHeight; y++) {
		for (int x = 0; x < mWidth; x++) {
			mObjects(x, y).drawBackGround(x,y,mImage);
		}
	}

	for (int y = 0; y < mHeight; y++) {
		for (int x = 0; x < mWidth; x++) {
			mObjects(x, y).drawForeground(x, y, mImage, mMoveCount);
		}
	}
}

void State::drawDebug() const {
	for (int y = 0; y < mHeight; y++) {
		for (int x = 0; x < mWidth; x++) {
			DrawFormatString(x * 15, y * 15, GetColor(0, 255, 0), "%d", mObjects(x, y));
		}
	}
}

void State::update(int moveX, int moveY) {
	if (mMoveCount == 32) {
		mMoveCount = 0;
		for (int y = 0; y < mHeight; y++) {
			for (int x = 0; x < mWidth; x++) {
				mObjects(x, y).mMoveX = 0;
				mObjects(x, y).mMoveY = 0;
			}
		}
	}
	if (mMoveCount > 0) {
		mMoveCount++;
		return;
	}
	int dx = 0;
	int dy = 0;
	if (canGetInput) {
		if (CheckHitKey(KEY_INPUT_W)) {
			// DrawString(0, 0, "W input", GetColor(0, 255, 0));
			dy = -1;
			canGetInput = false;
		}
		else if (CheckHitKey(KEY_INPUT_A)) {
			dx = -1;
			canGetInput = false;
		}
		else if (CheckHitKey(KEY_INPUT_S)) {
			dy = 1;
			canGetInput = false;
		}
		else if (CheckHitKey(KEY_INPUT_D)) {
			dx = 1;
			canGetInput = false;
		}
	}

	if (!CheckHitKeyAll())canGetInput = true;

	int w = mWidth;
	int h = mHeight;

	int x = -1;
	int y = -1;
	bool found = false;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (mObjects(i, j).mType == Object::OBJ_PLAYER) {
				// DrawFormatString(50, 100, GetColor(0, 255, 0), "(%d, %d), (%d, %d)", x, y, dx, dy);
				x = i; y = j;
				found = true;
				break;
			}
		}
		if (found)break;
	}
	
	int tx = x + dx;
	int ty = y + dy;

	// DrawFormatString(50, 130, GetColor(0, 255, 0), "(%d, %d)", tx, ty);

	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}

	if (mObjects(tx, ty).mType == Object::OBJ_SPACE) {
		mObjects(tx, ty).mType = Object::OBJ_PLAYER;
		mObjects(tx, ty).move(dx, dy);
		mMoveCount++;
		mObjects(x, y).mType = Object::OBJ_SPACE;
	}
	else if (mObjects(tx, ty).mType == Object::OBJ_BLOCK) {
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) {
			return;
		}
		if (mObjects(tx2, ty2).mType == Object::OBJ_SPACE) {
			mObjects(tx2, ty2).mType = Object::OBJ_BLOCK;
			mObjects(tx2, ty2).move(dx, dy);
			mObjects(tx, ty).mType = Object::OBJ_PLAYER;
			mObjects(tx, ty).move(dx, dy);
			mMoveCount++;
			mObjects(x, y).mType = Object::OBJ_SPACE;
		}
	}
}

bool State::checkIsClear() {
	for (int y = 0; y < mHeight; y++) {
		for (int x = 0; x < mWidth; x++) {
			if (mObjects(x, y).mType == Object::OBJ_BLOCK) {
				if (!(mObjects(x,y).mGoalFlag))return false;
			}
		}
	}
	canGetInput = false;
	return true;
}

void State::reset() {

}