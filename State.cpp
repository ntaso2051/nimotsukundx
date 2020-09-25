#include "State.h"
#include "Image.h"
#include "File.h"
#include "DxLib.h"

State::State() : mImage(0), stageData(0), canGetInput(true){
	canGetInput = true;
	File file("source/stage/stageData.txt");
	stageData = file.data();
	size = file.size();
	setSize();
	mObjects.setSize(mWidth, mHeight);
	mGoalFlags.setSize(mWidth, mHeight);

	for (int y = 0; y < mHeight; y++) {
		for (int x = 0; x < mWidth; x++) {
			mObjects(x, y) = OBJ_WALL;
			mGoalFlags(x, y) = false;
		}
	}
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; i++) {
		Object t;
		bool goalFlag = false;
		switch (stageData[i]) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK; goalFlag = true; break;
		case '.': t = OBJ_GOAL; goalFlag = true; break;
		case 'p': t = OBJ_PLAYER; break;
		case 'P': t = OBJ_PLAYER; goalFlag = true; break;
		case '\n': x = 0; y++; t = OBJ_UNKNOWN; break;
		default: t = OBJ_UNKNOWN; break;
		}

		if (t != OBJ_UNKNOWN) {
			mObjects(x, y) = t;
			mGoalFlags(x, y) = goalFlag;
			x++;
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
			mImage->draw(x, y, OBJ_SPACE);
			mImage->draw(x, y, mObjects(x, y));
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

void State::update() {
	int dx = 0;
	int dy = 0;
	if (canGetInput) {
		if (CheckHitKey(KEY_INPUT_W)) {
			DrawString(0, 0, "W input", GetColor(0, 255, 0));
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
			if (mObjects(i, j) == OBJ_PLAYER) {
				DrawFormatString(50, 100, GetColor(0, 255, 0), "(%d, %d), (%d, %d)", x, y, dx, dy);
				x = i; y = j;
				found = true;
				break;
			}
		}
		if (found)break;
	}
	
	int tx = x + dx;
	int ty = y + dy;

	DrawFormatString(50, 130, GetColor(0, 255, 0), "(%d, %d)", tx, ty);

	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}

	if (mObjects(tx, ty) == OBJ_SPACE) {
		mObjects(tx, ty) = OBJ_PLAYER;
		mObjects(x, y) = OBJ_SPACE;
	}
	else if (mObjects(tx, ty) == OBJ_BLOCK) {
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) {
			return;
		}
		if (mObjects(tx2, ty2) == OBJ_SPACE) {
			mObjects(tx2, ty2) = OBJ_BLOCK;
			mObjects(tx, ty) = OBJ_PLAYER;
			mObjects(x, y) = OBJ_SPACE;
		}
	}
}