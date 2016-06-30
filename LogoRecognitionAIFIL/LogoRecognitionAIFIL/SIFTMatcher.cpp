#include "SIFTMatcher.h"

SIFTMatcher::SIFTMatcher(Mat imagePattern_, Mat imageScene_) :
	IMatcher(imagePattern_, imageScene_)

{
	m_matchingThresholdValue = 2000;
	m_detector               = new SiftFeatureDetector(m_matchingThresholdValue);
	m_extractor              = new SiftDescriptorExtractor();
	m_matcher                = new FlannBasedMatcher();
}


SIFTMatcher::~SIFTMatcher()
{
	delete m_detector;
	delete m_extractor;
	delete m_matcher;
}