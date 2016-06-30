#include "SURFMatcher.h"

SURFMatcher::SURFMatcher(Mat imagePattern_, Mat imageScene_) :
	IMatcher(imagePattern_, imageScene_)
	
{
	m_matchingThresholdValue = 2000;
	m_detector               = new SurfFeatureDetector(m_matchingThresholdValue);
	m_extractor              = new SurfDescriptorExtractor();
	m_matcher                = new FlannBasedMatcher();
}


SURFMatcher::~SURFMatcher()
{		
	delete m_detector;
	delete m_extractor;
	delete m_matcher;
}

