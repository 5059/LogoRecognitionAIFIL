#include "IMatcher.h"

IMatcher::IMatcher(Mat imagePattern_, Mat imageScene_) :
	m_imagePattern(imagePattern_),
	m_imageScene(imageScene_)
{
	// Защита от слишком большого изображения (а то памяти не хватит)
	if (imagePattern_.cols > 1200 || imagePattern_.rows > 1200)
	{
		Size newSize(1000, 1200);
		resize(imagePattern_, imagePattern_, newSize);
	}

	if (m_imageScene.cols > 1200 || m_imageScene.rows > 1200)
	{
		Size newSize(1000, 1200);
		resize(m_imageScene, m_imageScene, newSize);
	}

};


IMatcher::~IMatcher()
{
	delete m_detector;
	delete m_extractor;
	delete m_matcher;
	delete m_keypointsImagePattern;
	delete m_keypointsImageScene;
	delete m_correctMatches;
	delete m_resultROI;
}

void IMatcher::DetectKeyPoints()
{
	m_detector->detect(m_imagePattern, *m_keypointsImagePattern);
	m_detector->detect(m_imageScene,    *m_keypointsImageScene);
}


void IMatcher::ComputeDescriptors()
{
	m_extractor->compute(m_imagePattern, *m_keypointsImagePattern, m_descriptorsImagePattern);
	m_extractor->compute(m_imageScene, *m_keypointsImageScene, m_descriptorsImageScene);
}


void IMatcher::Localization()
{
	// Локализация объектов
	vector<Point2f> imagePattern;
	vector<Point2f> imageScene;

	for (int i = 0; i < m_correctMatches->size(); i++)
	{
		imagePattern.push_back(m_keypointsImagePattern->at(m_correctMatches->at(i).queryIdx).pt);
		imageScene.push_back(m_keypointsImageScene->at(m_correctMatches->at(i).trainIdx).pt);
	}

	Mat H = findHomography(imagePattern, imageScene, CV_RANSAC);

	// Получить "углы" изображения с целевым объектом
	std::vector<Point2f> imagePatternCorners(4);
	imagePatternCorners[0] = cvPoint(0, 0);
	imagePatternCorners[1] = cvPoint(m_imagePattern.cols, 0);
	imagePatternCorners[2] = cvPoint(m_imagePattern.cols, m_imagePattern.rows);
	imagePatternCorners[3] = cvPoint(0, m_imagePattern.rows);

	std::vector<Point2f> imageCorners(4);
	// Отобразить углы целевого объекта, используя найденное преобразование, на сцену
	perspectiveTransform(imagePatternCorners, imageCorners, H);

	// Устраняем погрешность.
	// Вычислим длины линий, которые обводят найденый шаблон
	float leftLine   = sqrt( pow((imageCorners[0].x - imageCorners[1].x), 2) +
		                     pow((imageCorners[0].y - imageCorners[1].y), 2) ) ;

	float topLine    = sqrt( pow((imageCorners[1].x - imageCorners[2].x), 2) +
		                     pow((imageCorners[1].y - imageCorners[2].y), 2));

	float rightLine  = sqrt( pow((imageCorners[2].x - imageCorners[3].x), 2) +
		                     pow((imageCorners[2].y - imageCorners[3].y), 2));

	float bottomLine = sqrt( pow((imageCorners[3].x - imageCorners[0].x), 2) +
		                     pow((imageCorners[3].y - imageCorners[0].y), 2));
	// Примерная площадь фигуры должна быть больше 2% от площади всего изображения 
	if (leftLine * topLine < (m_imageScene.cols * m_imageScene.rows) / 50 )//|| 
		//leftLine < EPSILON || rightLine < EPSILON || topLine < EPSILON || bottomLine < EPSILON )
		//abs( (leftLine + rightLine) - (topLine + bottomLine) ) > (1.5 * max(topLine, rightLine) ))
	{
		return;
	}

	ROI resultROI;
	resultROI.lines[0] = { imageCorners[0], imageCorners[1], Scalar(0, 255, 0), 4 };
	resultROI.lines[1] = { imageCorners[1], imageCorners[2], Scalar(0, 255, 0), 4 };
	resultROI.lines[2] = { imageCorners[2], imageCorners[3], Scalar(0, 255, 0), 4 };
	resultROI.lines[3] = { imageCorners[3], imageCorners[0], Scalar(0, 255, 0), 4 };

	m_resultROI->push_back(resultROI);
}

std::vector<ROI>* IMatcher::Match()
{
	DetectKeyPoints();
	ComputeDescriptors();

	// HACK: Не удаляю вектор - утекает память!!!
	std::vector<DMatch> *matches = new std::vector<DMatch>();
	m_matcher->match(m_descriptorsImagePattern, m_descriptorsImageScene, *matches);

	double maxDistanse = 0; double minDistanse = 150;

	// Вычисление максимального и минимального расстояния среди всех дескрипторов
	// в пространстве признаков
	for (int i = 0; i < m_descriptorsImagePattern.rows; i++)
	{
		double dist = matches->at(i).distance;
		if (dist < minDistanse)
			minDistanse = dist;
		if (dist > maxDistanse)
			maxDistanse = dist;
	}

	// Отобрать только хорошие матчи, расстояние меньше чем coefficient * min_dist
	// TODO: Поиграться со значением coefficient
	double coefficient = 6;
	for (int i = 0; i < m_descriptorsImagePattern.rows; i++)
	{
		if (matches->at(i).distance < coefficient * minDistanse)
		{
			m_correctMatches->push_back(matches->at(i));
		}
	}
	if (m_correctMatches->size() == 0)
	{
		return nullptr;
	}

	Localization();

	return m_resultROI;
}
