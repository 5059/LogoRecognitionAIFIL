#ifndef SURF_MATCHER
#define SURF_MATCHER

#include "IMatcher.h"


//! \brief ��������� ������� SURF.
class SURFMatcher :	public IMatcher {	
public:
	//! \brief                         �����������.
	//! \params [in] imagePattern_     �����������-�������, �. �. ��, �������
	//!                                ����� ������.
	//! \params [in] imageScene_       �����������-�����, �� ������� ����� ������
	//!                                �����������-�������.
	SURFMatcher(Mat imagePattern_, Mat imageScene_);
	~SURFMatcher();
};

#endif