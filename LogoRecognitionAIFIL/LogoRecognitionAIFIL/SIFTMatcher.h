#ifndef SIFT_MATCHER
#define SIFT_MATCHER

#include "IMatcher.h"


//! \brief Сравнение методом SIFT.
class SIFTMatcher :	public IMatcher {	
public:
	//! \brief                         Конструктор.
	//! \params [in] imagePattern_     Изображение-образец, т. е. то, которое
	//!                                будем искать.
	//! \params [in] imageScene_       Изображение-сцена, на котором будем искать
	//!                                изображение-образец.
	SIFTMatcher(Mat imagePattern_, Mat imageScene_);
	~SIFTMatcher();	
};

#endif