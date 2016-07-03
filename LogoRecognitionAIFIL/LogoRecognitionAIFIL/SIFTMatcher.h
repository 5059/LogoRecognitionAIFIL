#ifndef SIFT_MATCHER
#define SIFT_MATCHER

#include "IMatcher.h"


//! \brief ��������� ������� SIFT.
class SIFTMatcher :	public IMatcher {	
public:
	//! \brief                         �����������.
	//! \params [in] imagePattern_     �����������-�������, �. �. ��, �������
	//!                                ����� ������.
	//! \params [in] imageScene_       �����������-�����, �� ������� ����� ������
	//!                                �����������-�������.
	SIFTMatcher(Mat imagePattern_, Mat imageScene_);
	~SIFTMatcher();	
};

#endif