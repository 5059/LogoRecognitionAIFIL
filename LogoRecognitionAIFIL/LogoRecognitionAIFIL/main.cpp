#include "stdafx.h"
#include "Image.h"
#include "IMatcher.h"
#include "SURFMatcher.h"
#include "SIFTMatcher.h"
#include <windows.h>
#include <filesystem>

const int MAX_PATH_LENGTH = 250;


//! \brief                        Конвертирует wchar_t в char.
//! \param [in] str_              Входная строка в формате wchar_t.
//! \return                       Возвращает строку в формате char*.
char* Wchar_tToChar(wchar_t *str_)
{
	wchar_t *wchar_tStr = str_;
	char    *charStr    = new char[MAX_PATH_LENGTH];

	std::wcstombs(charStr, wchar_tStr, MAX_PATH_LENGTH);

	return charStr;
}


//! \brief                        Соединяет путь к каталогу хранения файла в формате string
//!                               с именем файла в формате char*.
//! \param [in] path_             Путь к файлу в формате std::string.
//! \param [in] fileName          Имя файла в формате char*.
//! \return                       Возвращает путь вместе с именем файла в формате char*.
char* ConcatenationPathAndFileName(std::string path_, char *fileName_)
{
	char *fullPath = new char[path_.length() + strlen(fileName_)];
	strcpy(fullPath, path_.c_str());
	strcat(fullPath, fileName_);

	return fullPath;
}


//! \brief                        Выполняет поиск следующего изображения в каталое.
//! \param [in] findData_         Путь к файлу в формате std::string.
//! \param [in] folder_           Имя файла в формате char*.
//! \return                       Возвращает путь вместе с именем файла в формате char*.
Image* FindImageInFolder(wchar_t *findData_, std::string folder_)
{
	// Обрабатываем каталог с изображениями-образцами
	char *pathImagePattern    = Wchar_tToChar(&*findData_);
	if (strcmp(pathImagePattern, ".") == 0 || strcmp(pathImagePattern, "..") == 0)
		nullptr;

	char *tempPathImagePattern = ConcatenationPathAndFileName(folder_, pathImagePattern);	

	return new Image(tempPathImagePattern);
}


//! \brief                        Выполняет сравнение изображений несколькими способами.
//! \param [in] matcher_          Интерфес сравнивателя.
//! \param [in] imagePattern_     Изображение, которое будем искать.
//! \param [in] imageScene_       Изображение на котором будем искать.
//! \return                       Возвращает void.
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

	// Тут указываем только путь к каталогу, 
	// т. к. имя файла берётся из поля класса.
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
		// Перебираем образцы.
		do
		{				
			Image *imagePattern = FindImageInFolder(&findDataImagePatern.cFileName[0], folderImagePattern);

			WIN32_FIND_DATAW tempFindDataImageScene = findDataImageScene;			
			findImageScene = FindFirstFileW(L".\\DataSet_pic\\*.jpg", &findDataImageScene);

			if (INVALID_HANDLE_VALUE != findImageScene)
			{
				// Перебираем картинки, сравнивая их с образцом.
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
	
	// ждём нажатия клавиши
	//cvWaitKey(0);

	cvDestroyAllWindows();

	return 0;
}

