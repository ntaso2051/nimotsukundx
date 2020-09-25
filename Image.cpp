#include "Image.h"
#include "DxLib.h"



Image::Image(const char* filename, int AllNum, int XNum, int YNum, int XSize, int YSize) {
	mWidth = XSize; //��}�X�̑傫��
	mHeight = YSize; //��}�X�̑傫��
	mData = new int[AllNum];
	LoadDivGraph(filename, AllNum, XNum, YNum, mWidth, mHeight, mData);
}

Image::~Image() {
	delete[] mData;
	mData = 0;
}

void Image::draw(int x, int y, int id) const {
	DrawGraph(x * mWidth, y * mHeight, mData[id], TRUE);
}