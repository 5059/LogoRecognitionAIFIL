#ifndef IMAGE_H
#define IMAGE_H

#include "stdafx.h"
#include "IMatcher.h"

static std::string SplitFilename(const std::string& str)
{
	std::size_t found = str.find_last_of("/\\");
	return str.substr(found + 1);
}

class Image {
private:
	IplImage *m_iplImage;
	Mat      *m_mat;
	char     *m_nameWindowForShow;
	char     *m_nameImage;	
public:
	Image(char *path_, char *nameWindowForShow_);
	~Image();
	void      ShowImage();
	void      SaveImage(char *path_);
	void      DrawROI(ROI roi_);
	void      CreateWindowForShow(char *nameWindow_);
	Mat*      GetMat();
	IplImage* GetIplImage();
};


#endif