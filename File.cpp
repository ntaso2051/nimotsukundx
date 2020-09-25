#include "File.h"
#include <fstream>

// エラー出力用
#include <iostream>
#include <DxLib.h>

using namespace std;

File::File(const char* filename) : mSize(0), mStageData(0) {
	ifstream in(filename, ifstream::binary);
	if (!in) {
		mStageData = 0;
		mSize = 0;
	}
	else {
		in.seekg(0, ifstream::end);
		mSize = static_cast<int>(in.tellg());
		in.seekg(0, ifstream::beg);
		mStageData = new char[mSize];
		in.read(mStageData, mSize);
	}
}

File::~File() {
	delete[] mStageData;
	mStageData = 0;
}

int File::size() const {
	return mSize;
}

const char* File::data() const {
	return mStageData;
}

void File::drawString() const {
	std::cout << mStageData << endl;
	if (mStageData) {
		DrawString(0, 100, mStageData, GetColor(0, 255, 0));
		DrawString(0, 10, "success", GetColor(0, 255, 0));
	}
	else {
		DrawString(10, 10, "cannnot open file", GetColor(0, 255, 0));
	}
}