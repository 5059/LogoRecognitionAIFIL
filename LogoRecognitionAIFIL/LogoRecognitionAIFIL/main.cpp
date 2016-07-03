#include "stdafx.h"
#include "Image.h"
#include "IMatcher.h"
#include "SURFMatcher.h"
#include "SIFTMatcher.h"
#include <windows.h>
#include <filesystem>

const int MAX_PATH_LENGTH = 250;


//! \brief                        ������������ wchar_t � char.
//! \param [in] str_              ������� ������ � ������� wchar_t.
//! \return                       ���������� ������ � ������� char*.
char* Wchar_tToChar(wchar_t *str_)
{
	wchar_t *wchar_tStr = str_;
	char    *charStr    = new char[MAX_PATH_LENGTH];

	std::wcstombs(charStr, wchar_tStr, MAX_PATH_LENGTH);

	return charStr;
}


//! \brief                        ��������� ���� � �������� �������� ����� � ������� string
//!                               � ������ ����� � ������� char*.
//! \param [in] path_             ���� � ����� � ������� std::string.
//! \param [in] fileName          ��� ����� � ������� char*.
//! \return                       ���������� ���� ������ � ������ ����� � ������� char*.
char* ConcatenationPathAndFileName(std::string path_, char *fileName_)
{
	char *fullPath = new char[path_.length() + strlen(fileName_)];
	strcpy(fullPath, path_.c_str());
	strcat(fullPath, fileName_);

	return fullPath;
}


//! \brief                        ��������� ����� ���������� ����������� � �������.
//! \param [in] findData_         ���� � ����� � ������� std::string.
//! \param [in] folder_           ��� ����� � ������� char*.
//! \return                       ���������� ���� ������ � ������ ����� � ������� char*.
Image* FindImageInFolder(wchar_t *findData_, std::string folder_)
{
	// ������������ ������� � �������������-���������
	char *pathImagePattern    = Wchar_tToChar(&*findData_);
	if (strcmp(pathImagePattern, ".") == 0 || strcmp(pathImagePattern, "..") == 0)
		nullptr;

	char *tempPathImagePattern = ConcatenationPathAndFileName(folder_, pathImagePattern);	

	return new Image(tempPathImagePattern);
}


//! \brief                        ��������� ��������� ����������� ����������� ���������.
//! \param [in] matcher_          �������� ������������.
//! \param [in] imagePattern_     �����������, ������� ����� ������.
//! \param [in] imageScene_       ����������� �� ������� ����� ������.
//! \return                       ���������� void.
void Matching(IMatcher *matcher_, Image *imagePattern_, Image *imageScene_)
{
	matcher_ = new SURFMatcher(*imagePattern_->GetMat(), *imageScene_->GetMat());
	std::vector<ROI> *roiSURFMethod = nullptr;
	roiSURFMethod = matcher_->Match();

	matcher_ = new SIFTMatcher(*imagePattern_->GetMat(), *imageScene_->GetMat());
	std::vector<ROI> *roiSIFTMethod = matcher_->Match();
	if (roiSURFMethod->size() == 0 && roiSIFTMethod->size() == 0)
	{
		return;
	}
	else
	{
		if (roiSURFMethod->size() != 0)
			imageScene_->DrawROI(roiSURFMethod->front());
		if (roiSIFTMethod->size() != 0)
			imageScene_->DrawROI(roiSIFTMethod->front());
	}

	// ��� ��������� ������ ���� � ��������, 
	// �. �. ��� ����� ������ �� ���� ������.
	imageScene_->SaveImage("result\\");
}


int main()
{	
	IMatcher *matcher = nullptr;

	WIN32_FIND_DATAW findDataImageScene;
	WIN32_FIND_DATAW findDataImagePatern;
	
	std::string folderImagePattern = "DataSet_pic\\reference\\";	
	std::string folderImageScene   = "DataSet_pic\\";	
	
	HANDLE findImagePattern        = FindFirstFileW(L".\\DataSet_pic\\reference\\*.jpg", &findDataImagePatern);	
	HANDLE findImageScene          = FindFirstFileW(L".\\DataSet_pic\\*.jpg", &findDataImageScene);;

	setlocale(LC_ALL, "");	
	
	if (INVALID_HANDLE_VALUE != findImagePattern)
	{
		// ���������� �������.
		do
		{				
			Image *imagePattern = FindImageInFolder(&findDataImagePatern.cFileName[0], folderImagePattern);

			WIN32_FIND_DATAW tempFindDataImageScene = findDataImageScene;			
			findImageScene = FindFirstFileW(L".\\DataSet_pic\\*.jpg", &findDataImageScene);

			if (INVALID_HANDLE_VALUE != findImageScene)
			{
				// ���������� ��������, ��������� �� � ��������.
				do
				{									
					Image *imageScene = FindImageInFolder(&tempFindDataImageScene.cFileName[0], folderImageScene);
					if (imageScene == nullptr)
						continue;

					Matching(matcher, imagePattern, imageScene);
									
				} while (NULL != FindNextFileW(findImageScene, &tempFindDataImageScene));
			}			
		} while (NULL != FindNextFileW(findImagePattern, &findDataImagePatern));

		FindClose(findImagePattern);
		FindClose(findImageScene);
	}
	
	// ��� ������� �������
	//cvWaitKey(0);

	cvDestroyAllWindows();

	return 0;
}

