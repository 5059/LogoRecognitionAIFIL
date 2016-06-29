#ifndef SURF_MATCHER
#define SURF_MATCHER

#include "stdafx.h"
#include "IMatcher.h"

class SURFMatcher :	public IMatcher {
protected:
	void ComputeDescriptors();
	void DetectKeyPoints();
	void Localization();
public:
	SURFMatcher(Mat imagePattern_, Mat imageScene_);
	~SURFMatcher();

	void Match();
};

#endif