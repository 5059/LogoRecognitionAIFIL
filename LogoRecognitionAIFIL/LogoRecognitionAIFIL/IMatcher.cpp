#include "IMatcher.h"

IMatcher::IMatcher(Mat imagePattern_, Mat imageScene_) :
	m_imagePattern(imagePattern_),
	m_imageScene(imageScene_)
{
	// Защита от слишком большого изображения
	if (imagePattern_.cols > 1200 || imagePattern_.rows > 1200)
	{
		Size newSize(1000, 1200);
		resize(imagePattern_, imagePattern_, newSize);
	}

	if (m_imageScene.cols > 1200 || m_imageScene.rows > 1200)
	{
		Size newSize(1000, 1200);
		resize(m_imageScene, m_imageScene, newSize);
	}

};

void IMatcher::DetectKeyPoints()
{
	m_detector->detect(m_imagePattern, *m_keypointsImagePattern);
	m_detector->detect(m_imageScene,    *m_keypointsImageScene);
}


void IMatcher::ComputeDescriptors()
{
	m_extractor->compute(m_imagePattern, *m_keypointsImagePattern, m_descriptorsImagePattern);
	m_extractor->compute(m_imageScene, *m_keypointsImageScene, m_descriptorsImageScene);
}


void IMatcher::Localization()
{
	// Локализация объектов
	vector<Point2f> imagePattern;
	vector<Point2f> imageScene;

	for (int i = 0; i < m_correctMatches->size(); i++)
	{
		imagePattern.push_back(m_keypointsImagePattern->at(m_correctMatches->at(i).queryIdx).pt);
		imageScene.push_back(m_keypointsImageScene->at(m_correctMatches->at(i).trainIdx).pt);
	}

	Mat H = findHomography(imagePattern, imageScene, CV_RANSAC);

	//-- Получить "углы" изображения с целевым объектом
	std::vector<Point2f> imagePatternCorners(4);
	imagePatternCorners[0] = cvPoint(0, 0);
	imagePatternCorners[1] = cvPoint(m_imagePattern.cols, 0);
	imagePatternCorners[2] = cvPoint(m_imagePattern.cols, m_imagePattern.rows);
	imagePatternCorners[3] = cvPoint(0, m_imagePattern.rows);

	std::vector<Point2f> imageCorners(4);
	//-- Отобразить углы целевого объекта, используя найденное преобразование, на сцену
	perspectiveTransform(imagePatternCorners, imageCorners, H);

	// Устранить погрешность, когда объект выраждается в точку
	if (abs(imageCorners[0].x - imageCorners[1].x) < EPSILON ||
		abs(imageCorners[0].y - imageCorners[1].y) < EPSILON ||
		abs(imageCorners[1].y - imageCorners[2].y) < EPSILON ||
		abs(imageCorners[1].y - imageCorners[2].y) < EPSILON ||
		abs(imageCorners[2].y - imageCorners[3].y) < EPSILON ||
		abs(imageCorners[2].y - imageCorners[3].y) < EPSILON ||
		abs(imageCorners[3].y - imageCorners[0].y) < EPSILON ||
		abs(imageCorners[3].y - imageCorners[0].y) < EPSILON )
	{
		return;
	}

	ROI resultROI;
	resultROI.lines[0] = { imageCorners[0], imageCorners[1], Scalar(0, 255, 0), 4 };
	resultROI.lines[1] = { imageCorners[1], imageCorners[2], Scalar(0, 255, 0), 4 };
	resultROI.lines[2] = { imageCorners[2], imageCorners[3], Scalar(0, 255, 0), 4 };
	resultROI.lines[3] = { imageCorners[3], imageCorners[0], Scalar(0, 255, 0), 4 };

	m_resultROI->push_back(resultROI);
}

std::vector<ROI>* IMatcher::Match()
{
	DetectKeyPoints();
	ComputeDescriptors();

	// HACK: Не удаляю вектор - утекает память!!!
	std::vector<DMatch> *matches = new std::vector<DMatch>();
	m_matcher->match(m_descriptorsImagePattern, m_descriptorsImageScene, *matches);

	double maxDistanse = 0; double minDistanse = 300;

	// Вычисление максимального и минимального расстояния среди всех дескрипторов
	// в пространстве признаков
	for (int i = 0; i < m_descriptorsImagePattern.rows; i++)
	{
		double dist = matches->at(i).distance;
		if (dist < minDistanse)
			minDistanse = dist;
		if (dist > maxDistanse)
			maxDistanse = dist;
	}

	// Отобрать только хорошие матчи, расстояние меньше чем coefficient * min_dist
	// TODO: Поиграться со значением coefficient
	double coefficient = 5.5;
	for (int i = 0; i < m_descriptorsImagePattern.rows; i++)
	{
		if (matches->at(i).distance < coefficient * minDistanse)
		{
			m_correctMatches->push_back(matches->at(i));
		}
	}
	if (m_correctMatches->size() == 0)
	{
		return nullptr;
	}

	Localization();

	return m_resultROI;
}
