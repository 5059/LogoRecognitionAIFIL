#include "stdafx.h"
#include "Image.h"
#include "IMatcher.h"
#include "SURFMatcher.h"
#include "SIFTMatcher.h"
#include<windows.h>
#include <filesystem>

int main()
{
	// TODO: Убрать пути.
	Image *template_image = new Image("DataSet_pic\\reference\\chupa.jpg", "testing");
	template_image->ShowImage();
	std::cout << "!!!!!!" << std::endl;

	/*
	
	WIN32_FIND_DATAW findDataImageScene, findDataImagePatern;

	HANDLE const findImagePattern = FindFirstFileW(L".\\DataSet_pic\\*", &findDataImagePatern);
	HANDLE const findImageScene = FindFirstFileW(L".\\DataSet_pic\\*", &findDataImageScene);
	
	setlocale(LC_ALL, "");

	//IMatcher *matcher = new SIFTMatcher(*template_image->GetMat(), *image->GetMat());

	if (INVALID_HANDLE_VALUE != findImagePattern)
	{
		do
		{						
			wchar_t *wstr = &findDataImagePatern.cFileName[0];						
			char* ascii = new char[wcslen(wstr) + 1];			
			wcstombs(ascii, wstr, wcslen(wstr));			
			if ((*ascii) == '.')
				continue;

			Image *imagePattern = new Image(ascii, "image");

			WIN32_FIND_DATAW tempFindDataImageScene = findDataImageScene;

			if (INVALID_HANDLE_VALUE != findImageScene)
			{
				do
				{
					wchar_t *wstr = &findDataImageScene.cFileName[0];					
					char* ascii = new char[wcslen(wstr) + 1];
					wcstombs(ascii, wstr, wcslen(wstr));
					if ((*ascii) == '.')
						continue;

					Image *imageScene = new Image(ascii, "image");

					IMatcher *matcher = new SURFMatcher(*imagePattern->GetMat(), *imageScene->GetMat());
					matcher->Match();
					delete matcher;

				} while (NULL != FindNextFileW(findImageScene, &findDataImageScene));
			}			
		} while (NULL != FindNextFileW(findImagePattern, &findDataImagePatern));

		FindClose(findImagePattern);
		FindClose(findImageScene);
	}
*/

	///*
	Image *image = new Image("DataSet_pic\\1305023417875.jpg", "testing2");
	//Image *image = new Image("E:\\JOB\\DataSet_pic\\MiniChupaChups-1-Sindy.jpg", "testing2");
	image->ShowImage();	

	IMatcher *matcher = new SIFTMatcher(*template_image->GetMat(), *image->GetMat());
	matcher->Match();
	
	//*/

	// ждём нажатия клавиши
	cvWaitKey(0);

	cvDestroyAllWindows();
	return 0;
}

