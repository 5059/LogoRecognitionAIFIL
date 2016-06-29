#ifndef IMAGE_H
#define IMAGE_H

#include "stdafx.h"

class Image {
private:
	IplImage *m_iplImage;
	Mat      *m_mat;
	char     *m_nameWindowForShow;
public:
	Image(char *path_, char *nameWindowForShow_);
	~Image();
	void      ShowImage();
	void      CreateWindowForShow(char *nameWindow_);
	Mat*      GetMat();
	IplImage* GetIplImage();
};


#endif