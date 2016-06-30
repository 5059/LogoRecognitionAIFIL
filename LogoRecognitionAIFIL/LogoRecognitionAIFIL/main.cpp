#include "stdafx.h"
#include "Image.h"
#include "IMatcher.h"
#include "SURFMatcher.h"
#include "SIFTMatcher.h"
#include<windows.h>
#include <filesystem>

const int MAX_PATH_LENGTH = 250;

char* Wchar_tToChar(wchar_t *str_)
{
	wchar_t *wchar_tStr = str_;
	char *charStr = new char[MAX_PATH_LENGTH];
	std::wcstombs(charStr, wchar_tStr, MAX_PATH_LENGTH);

	return charStr;
}

char* ConcatenationPathAndFileName(std::string path_, char *fileName_)
{
	char *fullPath = new char[path_.length() + strlen(fileName_)];
	strcpy(fullPath, path_.c_str());
	strcat(fullPath, fileName_);

	return fullPath;
}

int main()
{	
	IMatcher *matcher;

	WIN32_FIND_DATAW findDataImageScene;
	WIN32_FIND_DATAW findDataImagePatern;
	
	std::string folderImagePattern = "DataSet_pic\\reference\\";	
	std::string folderImageScene   = "DataSet_pic\\";	
	
	HANDLE findImagePattern = FindFirstFileW(L".\\DataSet_pic\\reference\\*.jpg", &findDataImagePatern);	
	HANDLE findImageScene   = FindFirstFileW(L".\\DataSet_pic\\*.jpg", &findDataImageScene);;

	setlocale(LC_ALL, "");	
	
	if (INVALID_HANDLE_VALUE != findImagePattern)
	{
		// Перебираем образцы.
		do
		{	
			char *pathImagePattern = Wchar_tToChar(&findDataImagePatern.cFileName[0]);						
			if (strcmp(pathImagePattern, ".") == 0 || strcmp(pathImagePattern, "..") == 0)			
				continue;			

			char *tempPathImagePattern = ConcatenationPathAndFileName(folderImagePattern, pathImagePattern);
			std::cout << "     TEMPLATE: " << pathImagePattern << std::endl;
			Image *imagePattern = new Image(tempPathImagePattern, "image");

			WIN32_FIND_DATAW tempFindDataImageScene = findDataImageScene;			
			findImageScene = FindFirstFileW(L".\\DataSet_pic\\*.jpg", &findDataImageScene);
			if (INVALID_HANDLE_VALUE != findImageScene)
			{
				// Перебираем картинки, сравнивая их с образцом.
				do
				{										
					char *pathImageScene = Wchar_tToChar(&tempFindDataImageScene.cFileName[0]);
					if (strcmp(pathImageScene, ".") == 0 || strcmp(pathImageScene, "..") == 0)
						continue;

					char *tempPathImageScene = ConcatenationPathAndFileName(folderImageScene, pathImageScene);
					std::cout << "SCENE: " << pathImageScene << std::endl;
					Image *imageScene = new Image(tempPathImageScene, "image");

					matcher = new SURFMatcher(*imagePattern->GetMat(), *imageScene->GetMat());
					std::vector<ROI> *roiSURFMethod = nullptr;					
					roiSURFMethod = matcher->Match();

					matcher = new SIFTMatcher(*imagePattern->GetMat(), *imageScene->GetMat());
					std::vector<ROI> *roiSIFTMethod = matcher->Match();
					if (roiSURFMethod->size() == 0 && roiSIFTMethod->size() == 0)
					{						
						continue;
					}
					else
					{
						if (roiSURFMethod->size() != 0)
							imageScene->DrawROI(roiSURFMethod->front());
						if (roiSIFTMethod->size() != 0)
							imageScene->DrawROI(roiSIFTMethod->front());
					}
					
					imageScene->SaveImage("result\\");					
				} while (NULL != FindNextFileW(findImageScene, &tempFindDataImageScene));
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

