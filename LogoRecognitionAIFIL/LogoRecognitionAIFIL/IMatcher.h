#ifndef I_MATCHER
#define I_MATCHER

#include "stdafx.h"
#include "Image.h"

class IMatcher {
protected:
	int                  m_matchingThresholdValue;
	Feature2D           *m_detector;
	DescriptorExtractor *m_extractor;
	DescriptorMatcher   *m_matcher;
	virtual void ComputeDescriptors() = 0;
	virtual void DetectKeyPoints()    = 0;
public:
	IMatcher(){};
	virtual ~IMatcher(){};

	virtual void Match(Image *pattern_image_, Image *images_) = 0;	
};

#endif
