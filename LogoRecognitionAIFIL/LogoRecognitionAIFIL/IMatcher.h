#ifndef I_MATCHER
#define I_MATCHER

#include "stdafx.h"
#include "Image.h"

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

	virtual void ComputeDescriptors() = 0;
	virtual void DetectKeyPoints()    = 0;
public:
	IMatcher(Mat imagePattern_, Mat imageScene_) :
		m_imagePattern(imagePattern_),
		m_imageScene(imageScene_) {};
	virtual ~IMatcher(){};

	virtual void Match() = 0;	
};

#endif
