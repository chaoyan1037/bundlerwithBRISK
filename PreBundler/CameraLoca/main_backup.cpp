

#include "imagepre.h"
#include <iostream> 
#include <fstream>
#include <opencv2/features2d.hpp> 
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

bool refineMatchesWithHomography(
	const std::vector<cv::KeyPoint>& queryKeypoints,
	const std::vector<cv::KeyPoint>& trainKeypoints,
	float reprojectionThreshold,
	std::vector<cv::DMatch>& matches,
	cv::Mat& homography
	);

int main(int argc, char** argv)
{
	
	//char* imageDBPath = "database\\images\\db\\";//图片的存储路径
	//char* imageQueryPath = "database\\images\\query\\";
	//char* imageListDBPath = "database\\list\\list_db.txt";//图片list的存储路径
	//char* imageListQueryPath = "database\\list\\list_query.txt";
	//char* imageKeysDBPath = "database\\keys\\db\\";//特征点和描述子的存储路径
	//char* imageKeysQueryPath = "database\\keys\\query\\";
	//char* imageListDB[MAX_IMAGE_NUMBER_DB];
	//char* imageListQuery[MAX_IMAGE_NUMBER_DB];
	//int imageNumberDB = 0;
	//int imageNumberQuery = 0;
	////给每个图片的路径分配内存
	//for (int i = 0; i < MAX_IMAGE_NUMBER_DB; ++i){
	//	imageListDB[i] = new char[256];//记得要释放！！
	//	imageListQuery[i] = new char[256];
	//}

	////以下函数在更换数据库图片后只用执行一次
	//MakeImageListTxt(imageDBPath, imageListDBPath);
	//imageNumberDB = GetImageList(imageListDBPath, imageListDB);
	//ShowImageTest(imageListDB, imageNumberDB);//显示图片,测试用
	//ExtraFeature(imageKeysDBPath, imageListDB, imageNumberDB);

	////以下函数在更换查询图片后只用执行一次
	//MakeImageListTxt(imageQueryPath, imageListQueryPath);
	//imageNumberQuery = GetImageList(imageListQueryPath, imageListQuery);
	//ShowImageTest(imageListQuery, imageNumberQuery);//显示图片,测试用
	//ExtraFeature(imageKeysQueryPath, imageListQuery, imageNumberQuery);
	

	cv::Mat img_1, img_2;
	img_1 = imread("database/images/db/00000000.jpg", IMREAD_GRAYSCALE);
	img_2 = imread("database/images/db/00000001.jpg", IMREAD_GRAYSCALE);
	if (img_1.empty() | img_2.empty()){
		cout << "no images\n" << endl;
		exit(0);
	}

	//set brisk parameters
	int Threshl = 30;
	int Octaves = 3; //(pyramid layer) from which the keypoint has been extracted
	float PatternScales = 1.2f;
	
	cv::Ptr<cv::Feature2D> ptrBrisk = cv::BRISK::create(Threshl, Octaves, PatternScales);
	
	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
	ptrBrisk->detect(img_1, keypoints_1);//将img_1图像中检测到的特征点存储起来放在keypoints_1中
	ptrBrisk->detect(img_2, keypoints_2);//同理

	//-- Draw keypoints 
	//cv::Mat img_keypoints_1, img_keypoints_2;
	//drawKeypoints(img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	//(img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	//-- Show detected (drawn) keypoints 
	//imshow("Keypoints1", img_keypoints_1);
	//imshow("Keypoints2", img_keypoints_2);
	
	// Compute feature vector
	cv::Mat descriptors_1, descriptors_2; //define feature vectors matrix
	ptrBrisk->compute(img_1, keypoints_1, descriptors_1);
	ptrBrisk->compute(img_2, keypoints_2, descriptors_2);

	std::vector< DMatch > matches;
	//BFMatcher::BFMatcher(int normType = NORM_L2, bool crossCheck = false)
	cv::BFMatcher matcher(NORM_HAMMING, false);//使用knn时为false
	//matcher.match(descriptors_1, descriptors_2, matches);
	
	std::vector< std::vector< DMatch > > knn_matches;
	matcher.knnMatch(descriptors_1, descriptors_2, knn_matches, 2);
	for (size_t i = 0; i < knn_matches.size(); ++i)
	{
		const cv::DMatch& bestMatch = knn_matches[i][0];
		const cv::DMatch& betterMatch = knn_matches[i][1];

		float distanceRatio = bestMatch.distance / betterMatch.distance;

		// Pass only matches where distance ratio between 
		// nearest matches is greater than 1.5 (distinct criteria)
		if (distanceRatio < (1.f/1.5f))
		{
			matches.push_back(bestMatch);
		}
	}

	

	//cv::Mat img_matches;
	//drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_matches);
	//imshow("_Matches", img_matches);
	//waitKey(0);

	//std::vector<KeyPoint> alignedKps1, alignedKps2;
	//for (size_t i = 0; i < matches.size(); i++) {
	//	alignedKps1.push_back(keypoints_1[matches[i].queryIdx]);
	//	alignedKps2.push_back(keypoints_2[matches[i].trainIdx]);
	//}

	////Keypoints to points
	//std::vector<Point2f> ps1, ps2;
	//for (unsigned i = 0; i < alignedKps1.size(); i++)
	//	ps1.push_back(alignedKps1[i].pt);

	//for (unsigned i = 0; i < alignedKps2.size(); i++)
	//	ps2.push_back(alignedKps2[i].pt);

	cv::Mat homography;
	float reprojectionThreshold = 3.0;
	//std::vector<unsigned char> inliersMask(ps1.size());
	////CV_EXPORTS_W Mat findHomography( InputArray srcPoints, InputArray dstPoints,
	////int method = 0, double ransacReprojThreshold = 3,
	//	//OutputArray mask = noArray(), const int maxIters = 2000,
	//	//const double confidence = 0.995);

	//homography = cv::findHomography(ps1, ps2, CV_FM_RANSAC, reprojectionThreshold, inliersMask);

	//vector<DMatch> inliers;
	//vector<KeyPoint> inlierKeyPoints_1, inlierKeyPoints_2;
	//for (size_t i = 0; i < inliersMask.size(); i++){
	//	if (inliersMask[i]){
	//		inliers.push_back(matches[i]);
	//	}
	//}
	
	if ( refineMatchesWithHomography(keypoints_1, keypoints_2, reprojectionThreshold, matches, homography) ){
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				cout << homography.at<double>(i, j) << endl;

		cv::Mat img_matches_refined;
		drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_matches_refined);
		cout << "matches after refined:"<<matches.size() << endl;
		imshow("Matches_Homography_Refined", img_matches_refined);
		waitKey(0);

		imshow("im1", img_1);
		imshow("im2", img_2);
		cv::Mat img_warped;
		warpPerspective(img_1, img_warped, homography, img_2.size(), cv::INTER_LINEAR);
		imshow("Warped image.", img_warped);
		waitKey(0);

		std::vector<cv::KeyPoint> keypoints_warped;
		cv::Mat descriptors_warped;
		cv::Mat img_matches_warped;
		std::vector< DMatch > matches_warped;
		ptrBrisk->detect(img_warped, keypoints_warped);
		ptrBrisk->compute(img_warped, keypoints_warped, descriptors_warped);
		matcher.match(descriptors_warped, descriptors_2, matches_warped);

		cv::Mat homography_refined;
		refineMatchesWithHomography(keypoints_warped, keypoints_2, reprojectionThreshold, matches_warped, homography_refined);

		drawMatches(img_warped, keypoints_warped, img_2, keypoints_2, matches_warped, img_matches_warped);

		imshow("Warped image matches.", img_matches_warped);
		cout <<"matches after warped:"<< matches_warped.size() << endl;
		waitKey(0);
	}


	

	

	//释放内存每个图片的路径内存
	//for (int i = 0; i < MAX_IMAGE_NUMBER_DB; ++i){
	//	delete imageListDB[i];
	//	imageListDB[i] = nullptr;
	//	delete imageListQuery[i];
	//	imageListQuery[i] = nullptr;
	//}
}

bool refineMatchesWithHomography(
	const std::vector<cv::KeyPoint>& srcKeypoints,
	const std::vector<cv::KeyPoint>& desKeypoints,
	float reprojectionThreshold,
	std::vector<cv::DMatch>& matches,
	cv::Mat& homography
	)
{
	const int minNumberMatchesAllowed = 8;

	if (matches.size() < minNumberMatchesAllowed)
		return false;

	// Prepare data for cv::findHomography
	std::vector<cv::Point2f> srcPoints(matches.size());
	std::vector<cv::Point2f> dstPoints(matches.size());

	for (size_t i = 0; i < matches.size(); i++)
	{
		srcPoints[i] = srcKeypoints[matches[i].queryIdx].pt;
		dstPoints[i] = desKeypoints[matches[i].trainIdx].pt;
	}

	// Find homography matrix and get inliers mask
	std::vector<unsigned char> inliersMask(srcPoints.size());
	homography = cv::findHomography(srcPoints,
		dstPoints,
		CV_FM_RANSAC,
		reprojectionThreshold,
		inliersMask);

	std::vector<cv::DMatch> inliers;
	for (size_t i = 0; i<inliersMask.size(); i++)
	{
		if (inliersMask[i])
			inliers.push_back(matches[i]);
	}

	matches.swap(inliers);
	return matches.size() > minNumberMatchesAllowed;
}