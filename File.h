#ifndef INCLUDE_FILE_H
#define INCLUDE_FILE_H

class File {
public:
	File(const char* filename);
	~File();
	int size() const;
	const char* data() const;
	void drawString() const;
private:
	int mSize;
	char* mStageData;
};

#endif