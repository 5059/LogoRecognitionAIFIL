#ifndef SIFT_MATCHER
#define SIFT_MATCHER

#include "stdafx.h"
#include "IMatcher.h"

class SIFTMatcher :	public IMatcher {
protected:
	void ComputeDescriptors();
	void DetectKeyPoints();
	void Localization();
public:
	SIFTMatcher(Mat imagePattern_, Mat imageScene_);
	~SIFTMatcher();

	void Match();
};

#endif