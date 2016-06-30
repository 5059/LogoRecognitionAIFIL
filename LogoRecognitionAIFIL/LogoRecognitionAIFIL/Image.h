#ifndef IMAGE_H
#define IMAGE_H

#include "IMatcher.h"

static std::string SplitFilename(const std::string& str)
{
	std::size_t found = str.find_last_of("/\\");
	return str.substr(found + 1);
}

class Image {
public:
	Image(char *path_);
	~Image();	
	void      SaveImage(char *path_);
	void      DrawROI(ROI roi_);
	Mat*      GetMat();
	IplImage* GetIplImage();
private:
	IplImage *m_iplImage;
	Mat      *m_mat;
	char     *m_nameWindowForShow;
	char     *m_nameImage;
};

#endif