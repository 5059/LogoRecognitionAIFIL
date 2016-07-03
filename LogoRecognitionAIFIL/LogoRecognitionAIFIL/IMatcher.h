#ifndef I_MATCHER
#define I_MATCHER

#include "stdafx.h"


//! \brief ��������� �����, ������� ����� ������������ ������� ��������� ���������.
struct Line {
	Point2f                            x;
	Point2f                            y;
	Scalar                             color;
	int                                lineType;
};


//! \brief ��������� ��������������, ������� ����� ������������ ������� 
//!        ���������� ���������.
struct ROI {
	Line                               lines[4];
};


// ����������� ������������� - ����������� ������ 
// �� ����� ���� ������ 5�5 ��������
const int EPSILON = 5;


//! \brief ���������� ������� ������ ����������� �� ������. 
class IMatcher {
public:
	//! \brief                         �����������.
	//! \params [in] imagePattern_     �����������-�������, �. �. ��, �������
	//!                                ����� ������.
	//! \params [in] imageScene_       �����������-�����, �� ������� ����� ������
    //!                                �����������-�������.
	IMatcher(Mat imagePattern_, Mat imageScene_);

	virtual ~IMatcher();

    //! \brief                         ����������, ���� ������� ���������.
	//! \return                        �������, �� ������� ��������� �����������-��������.
	virtual std::vector<ROI>*          Match();

protected:
	int                                m_matchingThresholdValue;     //!< ��������� �������� (��������, ����������� �������� ��������).
	Mat                                m_imagePattern;               //!< �����������-�������.
	Mat                                m_imageScene;                 //!< �����������-�����.
	Mat                                m_descriptorsImagePattern;    //!< ����������� �����������-�������.
	Mat                                m_descriptorsImageScene;      //!< ����������� �����������-�����.
	Feature2D                         *m_detector;                   //!< ������� �������� �����.
	DescriptorExtractor               *m_extractor;                  //!< ��������� �����������.
	DescriptorMatcher                 *m_matcher;                    //!< ��������� ���������, �. �. ������� ������� ������� (�����).
	std::vector<DMatch>               *m_correctMatches;             //!< ������ ���������� ������� (�����).
	std::vector<ROI>                  *m_resultROI;                  //!< �������� ������� � ������������-��������
	std::vector<KeyPoint>             *m_keypointsImagePattern;      //!< �������� ����� ����������-�������.
	std::vector<KeyPoint>             *m_keypointsImageScene;        //!< �������� ����� �����������-�����.

	//! \brief                         ��������� �����������.
	//! \note                          ������������ ������ ������ ������� Match().
	virtual void                       ComputeDescriptors();

	//! \brief                         ������� �������� �����.
	//! \note                          ������������ ������ ������ ������� Match().
	virtual void                       DetectKeyPoints();

	//! \brief                         �������� � ��������������� ���������.
	//! \note                          ������������ ������ ������ ������� Match().
	virtual void                       Localization();
};

#endif
