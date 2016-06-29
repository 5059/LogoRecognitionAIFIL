#include "stdafx.h"
#include "Image.h"
#include "IMatcher.h"
#include "SURFMatcher.h"

// ����������� � �������� � ��������.
void SURFDetector(Image *imageTemplate_, Image *image_)
{
	Mat imageTemplate = *(imageTemplate_->GetMat());

	Mat image = *(image_->GetMat());

	// TODO: ���������� �� ���������
	int hessianThreshold = 1500;

	SurfFeatureDetector detector(hessianThreshold);

	std::vector<KeyPoint> keypointsForTemplateImage, keypointsForImage;

	// ����� �������� �����
	detector.detect(imageTemplate, keypointsForTemplateImage);
	detector.detect(image, keypointsForImage);

	SurfDescriptorExtractor extractor;

	Mat descriptorsImageTemplate, descriptorsImage;

	// ��������� �����������. 
	extractor.compute(imageTemplate, keypointsForTemplateImage, descriptorsImageTemplate);
	extractor.compute(image, keypointsForImage, descriptorsImage);

	// ������ ������ ������ ��������� �������
	FlannBasedMatcher matcher;
	vector< DMatch > matches;
	matcher.match(descriptorsImageTemplate, descriptorsImage, matches);

	double maxDistanse = 0; double minDistanse = 150;

	// ���������� ������������� � ������������ ���������� ����� ���� ������������
	// � ������������ ���������
	for (int i = 0; i < descriptorsImageTemplate.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < minDistanse) minDistanse = dist;
		if (dist > maxDistanse) maxDistanse = dist;
	}

	printf("-- Max dist : %f \n", maxDistanse);
	printf("-- Min dist : %f \n", minDistanse);

	
	vector< DMatch > goodMatches;
	// �������� ������ ������� �����, ���������� ������ ��� coefficient * min_dist
	// TODO: ���������� �� ��������� coefficient
	double coefficient = 3.5;
	for (int i = 0; i < descriptorsImageTemplate.rows; i++)
	{
		if (matches[i].distance < coefficient * minDistanse)
		{
			goodMatches.push_back(matches[i]);
		}
	}
	
	Mat imgageMatches;
	
	// ���������� ������� �����
	drawMatches(imageTemplate, keypointsForTemplateImage, image, keypointsForImage,
		goodMatches, imgageMatches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	
	// ����������� ��������
	vector<Point2f> obj;
	vector<Point2f> scene;

	for (int i = 0; i < goodMatches.size(); i++)
	{
		obj.push_back(keypointsForTemplateImage[goodMatches[i].queryIdx].pt);
		scene.push_back(keypointsForImage[goodMatches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, scene, CV_RANSAC);
	
	//-- �������� "����" ����������� � ������� ��������
	std::vector<Point2f> templateImageCorners(4);
	templateImageCorners[0] = cvPoint(0, 0); templateImageCorners[1] = cvPoint(imageTemplate.cols, 0);
	templateImageCorners[2] = cvPoint(imageTemplate.cols, imageTemplate.rows); templateImageCorners[3] = cvPoint(0, imageTemplate.rows);
	std::vector<Point2f> imageCorners(4);

	//-- ���������� ���� �������� �������, ��������� ��������� ��������������, �� �����
	perspectiveTransform(templateImageCorners, imageCorners, H);
	
	//-- ��������� ������������ ����
	line(imgageMatches, imageCorners[0] + Point2f(imageTemplate.cols, 0), imageCorners[1] + Point2f(imageTemplate.cols, 0), Scalar(0, 255, 0), 4);
	line(imgageMatches, imageCorners[1] + Point2f(imageTemplate.cols, 0), imageCorners[2] + Point2f(imageTemplate.cols, 0), Scalar(0, 255, 0), 4);
	line(imgageMatches, imageCorners[2] + Point2f(imageTemplate.cols, 0), imageCorners[3] + Point2f(imageTemplate.cols, 0), Scalar(0, 255, 0), 4);
	line(imgageMatches, imageCorners[3] + Point2f(imageTemplate.cols, 0), imageCorners[0] + Point2f(imageTemplate.cols, 0), Scalar(0, 255, 0), 4);

	//-- Show detected matches
	imshow("Good Matches & Object detection", imgageMatches);

}

int main()
{
	// TODO: ������ ����.
	Image *template_image = new Image("..\\..\\..\\DataSet_pic\\reference\\chupa.jpg", "testing");
	template_image->ShowImage();

	Image *image = new Image("..\\..\\..\\DataSet_pic\\1305023417875.jpg", "testing2");
	//Image *image = new Image("E:\\JOB\\DataSet_pic\\MiniChupaChups-1-Sindy.jpg", "testing2");
	image->ShowImage();

	//SURFDetector(template_image, image);	

	IMatcher *matcher = new SURFMatcher(*template_image->GetMat(), *image->GetMat());
	matcher->Match();
	// ��� ������� �������d
	cvWaitKey(0);

	cvDestroyAllWindows();
	return 0;
}