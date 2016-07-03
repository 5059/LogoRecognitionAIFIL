#ifndef I_MATCHER
#define I_MATCHER

#include "stdafx.h"


//! \brief Описывает линию, которая будет ограничивать область наденного фрагмента.
struct Line {
	Point2f                            x;
	Point2f                            y;
	Scalar                             color;
	int                                lineType;
};


//! \brief Описывает четырёхугольник, который будет ограничивать область 
//!        найденного фрагмента.
struct ROI {
	Line                               lines[4];
};


// Погрешность распознавания - распознаный объект 
// не может быть меньше 5х5 пикселей
const int EPSILON = 5;


//! \brief Занимается поиском одного изображения на другом. 
class IMatcher {
public:
	//! \brief                         Конструктор.
	//! \params [in] imagePattern_     Изображение-образец, т. е. то, которое
	//!                                будем искать.
	//! \params [in] imageScene_       Изображение-сцена, на котором будем искать
    //!                                изображение-образец.
	IMatcher(Mat imagePattern_, Mat imageScene_);

	virtual ~IMatcher();

    //! \brief                         Собственно, сама функция сравнения.
	//! \return                        Область, на которой находится изображение-фрагмент.
	virtual std::vector<ROI>*          Match();

protected:
	int                                m_matchingThresholdValue;     //!< Пороговое значение (например, минимальное значение гессиана).
	Mat                                m_imagePattern;               //!< Изображение-образец.
	Mat                                m_imageScene;                 //!< Изображение-сцена.
	Mat                                m_descriptorsImagePattern;    //!< Дескрипторы изображения-образца.
	Mat                                m_descriptorsImageScene;      //!< Дескрипторы изображения-сцены.
	Feature2D                         *m_detector;                   //!< Находит ключевые точки.
	DescriptorExtractor               *m_extractor;                  //!< Вычисляет дескрипторы.
	DescriptorMatcher                 *m_matcher;                    //!< Выполняет сравнение, т. е. находит хорошие образцы (матчи).
	std::vector<DMatch>               *m_correctMatches;             //!< Хранит корректные образцы (матчи).
	std::vector<ROI>                  *m_resultROI;                  //!< Найденая область с изображением-образцом
	std::vector<KeyPoint>             *m_keypointsImagePattern;      //!< Ключевые точки изображени-образца.
	std::vector<KeyPoint>             *m_keypointsImageScene;        //!< Ключевые точки изображения-сцены.

	//! \brief                         Вычисляет дескрипторы.
	//! \note                          Используется только внутри функции Match().
	virtual void                       ComputeDescriptors();

	//! \brief                         Находит ключевые точки.
	//! \note                          Используется только внутри функции Match().
	virtual void                       DetectKeyPoints();

	//! \brief                         Работает с локализованными образцами.
	//! \note                          Используется только внутри функции Match().
	virtual void                       Localization();
};

#endif
