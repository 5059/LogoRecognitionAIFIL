#include "stdafx.h"
#include "Image.h"


int main()
{
	Image *image = new Image("E:\\JOB\\DataSet_pic\\reference\\1.jpg", "testing");
	image->ShowImage();

	// ждём нажатия клавишиd
	cvWaitKey(0);

	cvDestroyAllWindows();
	return 0;
}