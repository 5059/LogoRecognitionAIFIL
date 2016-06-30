#include "stdafx.h"
#include "Image.h"
#include "IMatcher.h"
#include "SURFMatcher.h"
#include "SIFTMatcher.h"
#include<windows.h>
#include <filesystem>

const int MAX_PATH_LENGTH = 250;

int main()
{	
	WIN32_FIND_DATAW findDataImageScene;
	WIN32_FIND_DATAW findDataImagePatern;
	
	std::string folderImagePattern = "DataSet_pic\\reference\\";	
	std::string folderImageScene   = "DataSet_pic\\";	
	
	HANDLE const findImagePattern = FindFirstFileW(L".\\DataSet_pic\\reference\\*", &findDataImagePatern);
	HANDLE const findImageScene   = FindFirstFileW(L".\\DataSet_pic\\*",   &findDataImageScene);

	setlocale(LC_ALL, "");

	IMatcher *matcher;
	
	if (INVALID_HANDLE_VALUE != findImagePattern)
	{
		// Перебираем образцы, сравнвая каждый образец с картинкой в куче.
		do
		{									
			wchar_t *fileNameImagePattern = &findDataImagePatern.cFileName[0];				
			char pathImagePattern[MAX_PATH_LENGTH];
			std::wcstombs(pathImagePattern, fileNameImagePattern, MAX_PATH_LENGTH);			
			if (strcmp(pathImagePattern, ".") == 0 || strcmp(pathImagePattern, "..") == 0)			
				continue;			

			char *tempPathImagePattern = new char[folderImagePattern.length() + strlen(pathImagePattern)];
			strcpy(tempPathImagePattern, folderImagePattern.c_str());
			strcat(tempPathImagePattern, pathImagePattern);

			Image *imagePattern = new Image(tempPathImagePattern, "image");//("DataSet_pic\\reference\\1.jpg", ";ll");//

			WIN32_FIND_DATAW tempFindDataImageScene = findDataImageScene;

			if (INVALID_HANDLE_VALUE != findImageScene)
			{
				do
				{
					wchar_t *fileNameImageScene = &findDataImageScene.cFileName[0];
					char pathImageScene[MAX_PATH_LENGTH];
					std::wcstombs(pathImageScene, fileNameImageScene, MAX_PATH_LENGTH);
					if (strcmp(pathImageScene, ".") == 0 || strcmp(pathImageScene, "..") == 0)
						continue;

					char *tempPathImageScene = new char[folderImageScene.length() + strlen(pathImageScene)];
					strcpy(tempPathImageScene, folderImageScene.c_str());
					strcat(tempPathImageScene, pathImageScene);

					Image *imageScene = new Image(tempPathImageScene, "image");

					matcher = new SURFMatcher(*imagePattern->GetMat(), *imageScene->GetMat());
					imageScene->DrawROI(matcher->Match().front());
					matcher = new SIFTMatcher(*imagePattern->GetMat(), *imageScene->GetMat());
					imageScene->DrawROI(matcher->Match().front());
					imageScene->SaveImage("result\\");
					delete matcher;

				} while (NULL != FindNextFileW(findImageScene, &findDataImageScene));
			}			
		} while (NULL != FindNextFileW(findImagePattern, &findDataImagePatern));

		FindClose(findImagePattern);
		FindClose(findImageScene);
	}

	// ждём нажатия клавиши
	//cvWaitKey(0);

	cvDestroyAllWindows();
	return 0;
}

