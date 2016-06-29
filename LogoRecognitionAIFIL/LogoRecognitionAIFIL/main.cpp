#include "stdafx.h"
#include "Image.h"
#include "IMatcher.h"
#include "SURFMatcher.h"
#include "SIFTMatcher.h"

int main()
{
	// TODO: Убрать пути.
	Image *template_image = new Image("..\\..\\..\\DataSet_pic\\reference\\chupa.jpg", "testing");
	template_image->ShowImage();

	Image *image = new Image("..\\..\\..\\DataSet_pic\\1305023417875.jpg", "testing2");
	//Image *image = new Image("E:\\JOB\\DataSet_pic\\MiniChupaChups-1-Sindy.jpg", "testing2");
	image->ShowImage();


	IMatcher *matcher = new SIFTMatcher(*template_image->GetMat(), *image->GetMat());
	matcher->Match();
	// ждём нажатия клавишиd
	cvWaitKey(0);

	cvDestroyAllWindows();
	return 0;
}
