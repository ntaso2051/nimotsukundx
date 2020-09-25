#ifndef INCLUDE_IMAGE_H
#define INCLUDE_IMAGE_H

class Image {
public:
	Image(const char* filename, int AllNum, int XNum, int YNum, int XSize, int YSize);
	~Image();
	void draw(int x, int y, int id) const;
private:
	int mWidth;
	int mHeight;
	int* mData;
};


#endif