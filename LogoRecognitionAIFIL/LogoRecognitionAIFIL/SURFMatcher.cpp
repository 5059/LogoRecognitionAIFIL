#include "SURFMatcher.h"

SURFMatcher::SURFMatcher(Mat imagePattern_, Mat imageScene_) :
	IMatcher(imagePattern_, imageScene_)
	
{
	m_matchingThresholdValue = 1500;
	m_detector               = new SurfFeatureDetector(m_matchingThresholdValue);
	m_extractor              = new SurfDescriptorExtractor();
	m_matcher                = new FlannBasedMatcher();
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
		imageScene.push_back(m_keypointsImageScene[m_correctMatches[i].trainIdx].pt);
	}	

	Mat H = findHomography(imagePattern, imageScene, CV_RANSAC);
	
	//-- Получить "углы" изображения с целевым объектом
	std::vector<Point2f> imagePatternCorners(4);
	imagePatternCorners[0] = cvPoint(0,                   0);
	imagePatternCorners[1] = cvPoint(m_imagePattern.cols, 0);
	imagePatternCorners[2] = cvPoint(m_imagePattern.cols, m_imagePattern.rows);
	imagePatternCorners[3] = cvPoint(0,                   m_imagePattern.rows);

	std::vector<Point2f> imageCorners(4);
	//-- Отобразить углы целевого объекта, используя найденное преобразование, на сцену
	perspectiveTransform(imagePatternCorners, imageCorners, H);
	

	ROI resultROI;
	resultROI.lines[0] = { imageCorners[0], imageCorners[1], Scalar(0, 255, 0), 4 };
	resultROI.lines[1] = { imageCorners[1], imageCorners[2], Scalar(0, 255, 0), 4 };
	resultROI.lines[2] = { imageCorners[2], imageCorners[3], Scalar(0, 255, 0), 4 };
	resultROI.lines[3] = { imageCorners[3], imageCorners[0], Scalar(0, 255, 0), 4 };

	m_resultROI.push_back(resultROI);
}

std::vector<ROI> SURFMatcher::Match()
{		
	DetectKeyPoints();
	ComputeDescriptors();

	// HACK: Не удаляю вектор - утекает память!!!
	std::vector<DMatch> *matches = new std::vector<DMatch>();
	m_matcher->match(m_descriptorsImagePattern, m_descriptorsImageScene, *matches);

	double maxDistanse = 0; double minDistanse = 150;

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
	double coefficient = 3.5;
	for (int i = 0; i < m_descriptorsImagePattern.rows; i++)
	{
		if (matches->at(i).distance < coefficient * minDistanse)
		{
			m_correctMatches.push_back(matches->at(i));
		}
	}
	
	Localization();

	return m_resultROI;
}
