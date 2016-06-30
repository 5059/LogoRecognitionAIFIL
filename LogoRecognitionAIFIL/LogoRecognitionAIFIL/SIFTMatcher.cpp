#include "SIFTMatcher.h"

SIFTMatcher::SIFTMatcher(Mat imagePattern_, Mat imageScene_) :
	IMatcher(imagePattern_, imageScene_)

{
	m_matchingThresholdValue = 800;
	m_keypointsImagePattern = new std::vector<KeyPoint>();
	m_keypointsImageScene = new std::vector<KeyPoint>();
	m_correctMatches = new std::vector<DMatch>();
	m_detector               = new SiftFeatureDetector(m_matchingThresholdValue);
	m_extractor              = new SiftDescriptorExtractor();
	m_matcher                = new FlannBasedMatcher();
	m_resultROI = new std::vector<ROI>(0);
}


SIFTMatcher::~SIFTMatcher()
{
	delete m_detector;
	delete m_extractor;
	delete m_matcher;
	delete m_resultROI;
	delete m_keypointsImagePattern;
	delete m_keypointsImageScene;
	delete m_correctMatches;
}