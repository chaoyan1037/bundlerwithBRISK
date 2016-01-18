#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
	cv::Mat img_1 = imread("database\\images\\db\\00000000.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat img_2 = imread("database\\images\\db\\00000001.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (img_1.empty() || img_2.empty())
	{
		cout << "Load image error!\n" << endl;
		return -1;
	}

	cv::Mat srcImage = img_1;
	cv::Mat outImg;
	outImg = Mat::zeros(srcImage.rows, srcImage.cols, srcImage.type());
	cv::Matx23d warpMat;

	//定义两组点，代表两个三角形  
	Point2f srcTriangle[3];
	Point2f dstTriangle[3];
	srcTriangle[0] = Point2f(0, 0);
	srcTriangle[1] = Point2f(static_cast<float>(srcImage.cols - 1), 0);
	srcTriangle[2] = Point2f(0, static_cast<float>(srcImage.rows - 1));

	dstTriangle[0] = Point2f(static_cast<float>(srcImage.cols*0.0), static_cast<float>(srcImage.rows*0.13));
	dstTriangle[1] = Point2f(static_cast<float>(srcImage.cols*0.85), static_cast<float>(srcImage.rows*0.1));
	dstTriangle[2] = Point2f(static_cast<float>(srcImage.cols*0.15), static_cast<float>(srcImage.rows*0.8));

	warpMat = getAffineTransform(srcTriangle, dstTriangle);
	warpAffine(srcImage, outImg, warpMat, outImg.size());

	imshow("Raw image", srcImage);
	imshow("Warped image", outImg);

	cv::Mat dstImage_warp(outImg.size(), outImg.type());
	Point center = Point(dstImage_warp.cols / 2, dstImage_warp.rows / 2);
	double angle = 20.0;
	double scale = 1;

	cv::Matx23d rotMat = getRotationMatrix2D(center, angle, scale);
	warpAffine(outImg, dstImage_warp, rotMat, dstImage_warp.size());
	imshow("rot image", dstImage_warp);
	waitKey(0);
	return 0;
}