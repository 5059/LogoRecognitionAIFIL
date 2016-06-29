#include "Image.h"


Image::Image(char *path_, char *nameWindowForShow_) :
	m_nameWindowForShow(nameWindowForShow_)
{
	m_iplImage = cvLoadImage(path_);
}

Image::~Image()
{	
	cvReleaseImage(&m_iplImage);
}


void Image::ShowImage()
{
	cvShowImage(m_nameWindowForShow, m_iplImage);
}


void Image::CreateWindowForShow(char *nameWindow_)
{
	strcpy_s(m_nameWindowForShow, strlen(m_nameWindowForShow), nameWindow_);
	cvNamedWindow(m_nameWindowForShow, CV_WINDOW_AUTOSIZE);
}


Mat* Image::GetMat()
{
	return new Mat(m_iplImage);
}


IplImage* Image::GetIplImage()
{
	return m_iplImage;
}
