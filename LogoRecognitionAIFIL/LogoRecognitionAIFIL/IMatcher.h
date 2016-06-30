#ifndef I_MATCHER
#define I_MATCHER

#include "stdafx.h"
#include "Image.h"

struct Line {
	Point2f x;
	Point2f y;
	Scalar color;
	int lineType;
};

struct ROI {
	Line lines[4];
};

class IMatcher {
protected:
	int                   m_matchingThresholdValue;
	Feature2D            *m_detector;
	DescriptorExtractor  *m_extractor;
	DescriptorMatcher    *m_matcher;
	Mat                   m_imagePattern;
	Mat                   m_imageScene;
	Mat                   m_descriptorsImagePattern;
	Mat                   m_descriptorsImageScene;
	std::vector<KeyPoint> m_keypointsImagePattern;
	std::vector<KeyPoint> m_keypointsImageScene;
	std::vector<DMatch>   m_correctMatches;
	std::vector<ROI>      m_resultROI;

	virtual void ComputeDescriptors() = 0;
	virtual void DetectKeyPoints()    = 0;
	virtual void Localization() = 0;
public:
	IMatcher(Mat imagePattern_, Mat imageScene_) :
		m_imagePattern(imagePattern_),
		m_imageScene(imageScene_) {};
	virtual ~IMatcher(){};

	virtual std::vector<ROI> Match() = 0;
};

#endif
