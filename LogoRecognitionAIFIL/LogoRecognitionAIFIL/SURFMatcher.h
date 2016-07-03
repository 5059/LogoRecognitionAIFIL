#ifndef SURF_MATCHER
#define SURF_MATCHER

#include "IMatcher.h"


//! \brief Сравнение методом SURF.
class SURFMatcher :	public IMatcher {	
public:
	//! \brief                         Конструктор.
	//! \params [in] imagePattern_     Изображение-образец, т. е. то, которое
	//!                                будем искать.
	//! \params [in] imageScene_       Изображение-сцена, на котором будем искать
	//!                                изображение-образец.
	SURFMatcher(Mat imagePattern_, Mat imageScene_);
	~SURFMatcher();
};

#endif