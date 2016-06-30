#ifndef I_MATCHER
#define I_MATCHER

#include "stdafx.h"

struct Line {
	Point2f x;
	Point2f y;
	Scalar color;
	int lineType;
};

struct ROI {
	Line lines[4];
};

// Погрешность распознавания - распознаный объект 
// не может быть меньше 5х5 пикселей
const int EPSILON = 5;

class IMatcher {
protected:
	int                    m_matchingThresholdValue;
	Feature2D             *m_detector;
	DescriptorExtractor   *m_extractor;
	DescriptorMatcher     *m_matcher;
	Mat                    m_imagePattern;
	Mat                    m_imageScene;
	Mat                    m_descriptorsImagePattern;
	Mat                    m_descriptorsImageScene;
	std::vector<KeyPoint> *m_keypointsImagePattern;
	std::vector<KeyPoint> *m_keypointsImageScene;
	std::vector<DMatch>   *m_correctMatches;
	std::vector<ROI>      *m_resultROI;

	virtual void ComputeDescriptors();
	virtual void DetectKeyPoints();
	virtual void Localization();
public:
	IMatcher(Mat imagePattern_, Mat imageScene_);
	virtual ~IMatcher(){};

	virtual std::vector<ROI>* Match();
};

#endif
