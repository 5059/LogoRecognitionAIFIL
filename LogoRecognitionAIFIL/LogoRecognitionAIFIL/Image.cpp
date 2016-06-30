#include "Image.h"


Image::Image(char *path_, char *nameWindowForShow_) :
	m_nameWindowForShow(nameWindowForShow_)
{
	m_iplImage = cvLoadImage(path_);
	m_mat      = new Mat(m_iplImage);
	// Имя будет в дальнейшем использовано для записи изображения в файл.
	string fileName = SplitFilename("DataSet_pic\\reference\\chupa.jpg");
	m_nameImage = new char[fileName.length() + 1];
	// Приводим к char* - так удобнее.
	strcpy(m_nameImage, fileName.c_str());
}

Image::~Image()
{	
	cvReleaseImage(&m_iplImage);
}


void Image::ShowImage()
{
	cvShowImage(m_nameWindowForShow, m_iplImage);
}


void Image::SaveImage(char *path_)
{
	char *fullPath = new char[strlen(path_) + strlen(m_nameImage) + 1];
	fullPath[0] = '\0';
	strcat(fullPath, path_);
	strcat(fullPath, m_nameImage);
	imwrite(fullPath, *GetMat());
}


void Image::DrawROI(ROI roi_)
{
	line(*m_mat, roi_.lines[0].x, roi_.lines[0].y, roi_.lines[0].color, roi_.lines[0].lineType);
	line(*m_mat, roi_.lines[1].x, roi_.lines[1].y, roi_.lines[1].color, roi_.lines[1].lineType);
	line(*m_mat, roi_.lines[2].x, roi_.lines[2].y, roi_.lines[2].color, roi_.lines[2].lineType);
	line(*m_mat, roi_.lines[3].x, roi_.lines[3].y, roi_.lines[3].color, roi_.lines[3].lineType);
}

void Image::CreateWindowForShow(char *nameWindow_)
{
	strcpy_s(m_nameWindowForShow, strlen(m_nameWindowForShow), nameWindow_);
	cvNamedWindow(m_nameWindowForShow, CV_WINDOW_AUTOSIZE);
}


Mat* Image::GetMat()
{
	return m_mat;
}


IplImage* Image::GetIplImage()
{
	return m_iplImage;
}
