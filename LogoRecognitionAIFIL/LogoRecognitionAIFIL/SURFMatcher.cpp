#include "SURFMatcher.h"

SURFMatcher::SURFMatcher(Mat imagePattern_, Mat imageScene_) :
	IMatcher(imagePattern_, imageScene_)
	
{
	m_matchingThresholdValue = 1500;
	m_detector = new SurfFeatureDetector();
}


SURFMatcher::~SURFMatcher()
{	
}


void SURFMatcher::DetectKeyPoints()
{
	m_detector->detect(m_imagePattern, m_keypointsImagePattern);
	m_detector->detect(m_imageScene, m_keypointsImageScene);
}


void SURFMatcher::ComputeDescriptors()
{	
	m_extractor->compute(m_imagePattern, m_keypointsImagePattern, m_descriptorsImagePattern);
	m_extractor->compute(m_imageScene,   m_keypointsImageScene,   m_descriptorsImageScene);
}

void SURFMatcher::Localization()
{
	// Локализация объектов
	vector<Point2f> imagePattern;
	vector<Point2f> imageScene;

	for (int i = 0; i < m_correctMatches.size(); i++)
	{
		imagePattern.push_back(m_keypointsImagePattern[m_correctMatches[i].queryIdx].pt);
		imageScene.push_back(m_keypointsImageimageScene[m_correctMatches[i].trainIdx].pt);
	}

	Mat H = findHomography(imagePattern, imageScene, CV_RANSAC);
	
	//-- Получить "углы" изображения с целевым объектом
	std::vector<Point2f> imagePatternCorners(4);
	imagePatternCorners[0] = cvPoint(0,                  0);
	imagePatternCorners[1] = cvPoint(imageTemplate.cols, 0);
	imagePatternCorners[2] = cvPoint(imageTemplate.cols, imageTemplate.rows);
	imagePatternCorners[3] = cvPoint(0,                  imageTemplate.rows);

	std::vector<Point2f> imageCorners(4);
	//-- Отобразить углы целевого объекта, используя найденное преобразование, на сцену
	perspectiveTransform(imagePatternCorners, imageCorners, H);
	
	//-- Соеденить отображенные углы
	line(imgageMatches, imageCorners[0] + Point2f(imageTemplate.cols, 0), imageCorners[1] + Point2f(imageTemplate.cols, 0), Scalar(0, 255, 0), 4);
	line(imgageMatches, imageCorners[1] + Point2f(imageTemplate.cols, 0), imageCorners[2] + Point2f(imageTemplate.cols, 0), Scalar(0, 255, 0), 4);
	line(imgageMatches, imageCorners[2] + Point2f(imageTemplate.cols, 0), imageCorners[3] + Point2f(imageTemplate.cols, 0), Scalar(0, 255, 0), 4);
	line(imgageMatches, imageCorners[3] + Point2f(imageTemplate.cols, 0), imageCorners[0] + Point2f(imageTemplate.cols, 0), Scalar(0, 255, 0), 4);
}

void SURFMatcher::Match()
{		
	std::vector<DMatch> matches;
	m_matcher->match(m_descriptorsImagePattern, m_descriptorsImageScene, matches);

	double maxDistanse = 0; double minDistanse = 150;

	// Вычисление максимального и минимального расстояния среди всех дескрипторов
	// в пространстве признаков
	for (int i = 0; i < m_descriptorsImageScene.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < minDistanse) 
			minDistanse = dist;
		if (dist > maxDistanse) 
			maxDistanse = dist;
	}

	double coefficient = 3.5;
	for (int i = 0; i < m_descriptorsImagePattern.rows; i++)
	{
		if (matches[i].distance < coefficient * minDistanse)
		{
			m_correctMatches.push_back(matches[i]);
		}
	}
}
