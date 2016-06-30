#ifndef SURF_MATCHER
#define SURF_MATCHER

#include "stdafx.h"
#include "IMatcher.h"

class SURFMatcher :	public IMatcher {	
public:
	SURFMatcher(Mat imagePattern_, Mat imageScene_);
	~SURFMatcher();
};

#endif