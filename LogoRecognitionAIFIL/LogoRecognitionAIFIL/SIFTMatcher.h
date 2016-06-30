#ifndef SIFT_MATCHER
#define SIFT_MATCHER

#include "stdafx.h"
#include "IMatcher.h"

class SIFTMatcher :	public IMatcher {	
public:
	SIFTMatcher(Mat imagePattern_, Mat imageScene_);
	~SIFTMatcher();	
};

#endif