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


void SURFMatcher::Match()
{
	DetectKeyPoints();
	ComputeDescriptors();

	// Локализация объектов
	vector<Point2f> obj;
	vector<Point2f> scene;

	for (int i = 0; i < goodMatches.size(); i++)
	{
		obj.push_back(keypointsForTemplateImage[goodMatches[i].queryIdx].pt);
		scene.push_back(keypointsForImage[goodMatches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, scene, CV_RANSAC);

	//-- Получить "углы" изображения с целевым объектом
	std::vector<Point2f> templateImageCorners(4);
	templateImageCorners[0] = cvPoint(0, 0); templateImageCorners[1] = cvPoint(imageTemplate.cols, 0);
	templateImageCorners[2] = cvPoint(imageTemplate.cols, imageTemplate.rows); templateImageCorners[3] = cvPoint(0, imageTemplate.rows);
	std::vector<Point2f> imageCorners(4);

	//-- Отобразить углы целевого объекта, используя найденное преобразование, на сцену
	perspectiveTransform(templateImageCorners, imageCorners, H);

	//-- Соеденить отображенные углы
	line(imgageMatches, imageCorners[0] + Point2f(imageTemplate.cols, 0), imageCorners[1] + Point2f(imageTemplate.cols, 0), Scalar(0, 255, 0), 4);
	line(imgageMatches, imageCorners[1] + Point2f(imageTemplate.cols, 0), imageCorners[2] + Point2f(imageTemplate.cols, 0), Scalar(0, 255, 0), 4);
	line(imgageMatches, imageCorners[2] + Point2f(imageTemplate.cols, 0), imageCorners[3] + Point2f(imageTemplate.cols, 0), Scalar(0, 255, 0), 4);
	line(imgageMatches, imageCorners[3] + Point2f(imageTemplate.cols, 0), imageCorners[0] + Point2f(imageTemplate.cols, 0), Scalar(0, 255, 0), 4);

}