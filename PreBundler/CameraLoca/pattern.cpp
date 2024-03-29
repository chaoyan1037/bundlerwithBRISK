#include "pattern.h"

using namespace std;
using namespace cv;
using namespace xfeatures2d;


PatternMatch::PatternMatch():homographyReproThres(8.0f)
{
	//cv::xfeatures2d::LATCH::create(int bytes = 32,bool rotationInvariance = true,int half_ssd_size = 3)
	//mPtrXfeature = cv::xfeatures2d::LATCH::create(64, true, 3);
	//Threshl = 30; Octaves = 3; PatternScales = 1.2f;
	mPtrFeature = cv::BRISK::create(30,3,1.2f);//for the opencv3
	mPtrMatcher = new cv::BFMatcher(NORM_HAMMING, false);
	//create (int nfeatures=500, float scaleFactor=1.2f, int nlevels=8, 
	//int edgeThreshold=31, int firstLevel=0, int WTA_K=2, 
	//int scoreType=ORB::HARRIS_SCORE, int patchSize=31, int fastThreshold=20)
	//mPtrFeature = cv::ORB::create();


	//mPtrFeature = xfeatures2d::SIFT::create();
	//mPtrMatcher = new cv::BFMatcher(NORM_L2, true);

	//mPtrFeature = xfeatures2d::SURF::create();
	//mPtrMatcher = new cv::BFMatcher(NORM_L2, true);
	
	//mPtrFeature = cv::AKAZE::create();

	std::cout << "PatternMatch Construct\n" << std::endl;
}

PatternMatch::~PatternMatch(){
	//delete mPtrMatcher;
	cout <<"~PatternMatch\n" << endl;
}

void PatternMatch::BuildPatternFromImage(const std::vector<std::string>& imgList)
{
	for (uint k = 0; k < imgList.size(); ++k){
		cv::Mat img;
		img = imread(imgList[k], cv::IMREAD_GRAYSCALE);
		if (img.empty()){
			cout << "Open Img fail:" << imgList[k] << endl;
		}

		Pattern tempPattern;
		mPtrFeature->detect(img, tempPattern.keypoints);
		cout << "Imag " << k <<" find keypoints: " << tempPattern.keypoints.size() << endl;
		mPtrFeature->compute(img, tempPattern.keypoints, tempPattern.descriptors);
		//mPtrXfeature->compute(img, tempPattern.keypoints, tempPattern.descriptors);
		cout << "Descriptors Length: " << tempPattern.descriptors.cols << endl;
	
		//store the key and descriptors for each image
		WriteKeyPointsWithDescriptors(imgList[k],
			tempPattern.keypoints,
			tempPattern.descriptors);
		//float max = 0;
		//KeyPoint maxPoint;
		//for (uint i = 0; i < tempPattern.keypoints.size(); ++i){
		//	if (max < tempPattern.keypoints[i].pt.x + tempPattern.keypoints[i].pt.y){
		//		max = tempPattern.keypoints[i].pt.x + tempPattern.keypoints[i].pt.y;
		//		maxPoint = tempPattern.keypoints[i];
		//	}
		//}
		//
		//for (uint i = maxPoint.pt.x; i < maxPoint.pt.x+10; ++i)
		//for (uint j = maxPoint.pt.y; j < maxPoint.pt.y+10; ++j)
		//		img.at<uchar>(j, i) = 255;
		//imshow("test", img);
		//waitKey(0);

		//显示带特征点的图片，测试用
		//cv::Mat imgKeypoints;
		//drawKeypoints(img, tempPattern.keypoints, imgKeypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		//imshow("Keypoints", imgKeypoints);
		//waitKey(0);
		//保存图片，调试阶段用
 		tempPattern.img = img;

		size_t size = tempPattern.keypoints.size();
		for (uint i = 0; i < size; ++i){
			tempPattern.points2d.push_back(tempPattern.keypoints[i].pt);
		}
		mPattern.push_back(tempPattern);
	}

}

//把指定的图片集中所有图片两两匹配
void PatternMatch::MatchPatternTwoInAll(std::vector<uint> dstNumberVector){
	std::vector<uint> tsrcNumberVector;
	tsrcNumberVector.clear();
	for (uint dstNumber : dstNumberVector){
		tsrcNumberVector.push_back(dstNumber);
		MatchPatternOneToAll(tsrcNumberVector, dstNumber);
	}
}


//把一个图片和其他所有图片匹配,得到匹配和匹配列表
//匹配列表中保存了匹配成功的图片的序号
void PatternMatch::MatchPatternOneToAll(const std::vector<uint>& srcNumberVector,
	const uint& dstNumber)
{
	//std::vector<uint>::iterator it;
	static int match_count = 0;
	MatchesImage			matchedImg;
	MatchesInvertedIndex	matedInvertedIndex;
	for (uint srcNumber : srcNumberVector){
		if (srcNumber == dstNumber)
			continue;
		matchedImg.matches.clear();
		matchedImg.matchesNumber = 0;
		if (MatchPatternOneToOne(mPattern[srcNumber], mPattern[dstNumber], matchedImg.matches)){
			//std::vector<cv::DMatch> matches;

			char number[8];
			sprintf_s(number, "%d", matchedImg.matches.size());
			string windowName = "  matches: ";
			windowName += number;
			cout << ++match_count << " : " << srcNumber << " - " << dstNumber << windowName << endl;
		#if 0
			cv::Mat img_matches;
			drawMatches(mPattern[srcNumber].img,
				mPattern[srcNumber].keypoints,
				mPattern[dstNumber].img,
				mPattern[dstNumber].keypoints,
				matchedImg.matches,
				img_matches,
				Scalar::all(-1),
				Scalar::all(-1),
				std::vector<char>(),
				DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS
				);
			imshow(windowName, img_matches);
			waitKey(0);
		#endif			
			matchedImg.matchesNumber = dstNumber;
			mPattern[srcNumber].matchedImg.push_back(matchedImg);
			
			matedInvertedIndex.matchedImgNumber = srcNumber;
			matedInvertedIndex.matchedImgMatchesNumber = mPattern[srcNumber].matchedImg.size() - 1;
			mPattern[dstNumber].matchedImgInvertedIndex.push_back(matedInvertedIndex);

			//wirte the matches.init.txt
			WriteMatches(srcNumber, dstNumber, matchedImg.matches);
		}
	}
}


void DrawEpiLines(const Mat& img_1, const Mat& img_2, vector<Point2f>points1, vector<Point2f>points2);
//匹配两个模式（图像）,利用基础矩阵优化匹配，匹配成功返回1，否则返回0
bool PatternMatch::MatchPatternOneToOne(const Pattern& src, 
	const Pattern& dst, 
	std::vector<DMatch>& matches)
{
	matches.clear();
	//mPtrMatcher->match(src.descriptors, dst.descriptors, matches);

	float nn_match_ratio = 0.80f;
	std::vector<std::vector<cv::DMatch>> nn_matches;
	mPtrMatcher->knnMatch(src.descriptors, dst.descriptors, nn_matches, 2);
	for (size_t i = 0; i < nn_matches.size(); i++) {
		float dist1 = nn_matches[i][0].distance;
		float dist2 = nn_matches[i][1].distance;

		if (dist1 < nn_match_ratio * dist2) {
			matches.push_back(nn_matches[i][0]);
		}
	}

	cv::Mat fundamental;
	cv::Mat homography;
#if 1
	if (RefineMatchesWithFundamental(src.keypoints,
			dst.keypoints, 
			homographyReproThres, 
			matches, 
			fundamental)
		) 
#else 
	if(	RefineMatchesWithHomography(src.keypoints, 
			dst.keypoints,
			homographyReproThres,
			matches,
			homography)
		)
#endif
	{
	#if 0
		// 画出外极线
		// Prepare data for cv::findFundamentalMat
		std::vector<cv::Point2f> points1(matches.size());
		std::vector<cv::Point2f> points2(matches.size());
		for (size_t i = 0; i < matches.size(); i++){
			points1[i] = src.keypoints[matches[i].queryIdx].pt;
			points2[i] = dst.keypoints[matches[i].trainIdx].pt;
		}
		DrawEpiLines(src.img, dst.img, points1, points2);
	#endif
		return true;
	}
	return false;
}


bool PatternMatch::RefineMatchesWithHomography(
	const std::vector<cv::KeyPoint>& queryKeypoints,
	const std::vector<cv::KeyPoint>& trainKeypoints,
	const float reprojectionThreshold,
	std::vector<cv::DMatch>& matches,
	cv::Mat& homography)
{
	const int minNumberMatchesAllowed = 14;

	if (matches.size() < minNumberMatchesAllowed)
		return false;

	// Prepare data for cv::findHomography
	std::vector<cv::Point2f> srcPoints(matches.size());
	std::vector<cv::Point2f> dstPoints(matches.size());

	for (size_t i = 0; i < matches.size(); i++)
	{
		srcPoints[i] = queryKeypoints[matches[i].queryIdx].pt;
		dstPoints[i] = trainKeypoints[matches[i].trainIdx].pt;
	}

	// Find homography matrix and get inliers mask
	std::vector<unsigned char> inliersMask(srcPoints.size());
	homography = cv::findHomography(srcPoints,
		dstPoints,
		cv::FM_RANSAC,
		reprojectionThreshold,
		inliersMask);

	std::vector<cv::DMatch> inliers;
	for (size_t i = 0; i<inliersMask.size(); i++)
	{
		if (inliersMask[i])
			inliers.push_back(matches[i]);
	}

	matches.swap(inliers);
	return (matches.size() > minNumberMatchesAllowed);
}

bool PatternMatch::RefineMatchesWithFundamental(
	const std::vector<cv::KeyPoint>& queryKeypoints,
	const std::vector<cv::KeyPoint>& trainKeypoints,
	const float reprojectionThreshold,
	std::vector<cv::DMatch>& matches,
	cv::Mat& fundamental)
{
	const int minNumberMatchesAllowed = 14;

	if (matches.size() < minNumberMatchesAllowed)
		return false;

	// Prepare data for cv::findHomography
	std::vector<cv::Point2f> srcPoints(matches.size());
	std::vector<cv::Point2f> dstPoints(matches.size());

	for (size_t i = 0; i < matches.size(); i++)
	{
		srcPoints[i] = queryKeypoints[matches[i].queryIdx].pt;
		dstPoints[i] = trainKeypoints[matches[i].trainIdx].pt;
	}

	// Find homography matrix and get inliers mask
	std::vector<unsigned char> inliersMask(srcPoints.size());
	fundamental = cv::findFundamentalMat(srcPoints,
		dstPoints,
		cv::FM_RANSAC,
		reprojectionThreshold,
		0.99f,
		inliersMask);

	std::vector<cv::DMatch> inliers;
	for (size_t i = 0; i<inliersMask.size(); i++)
	{
		if (inliersMask[i])
			inliers.push_back(matches[i]);
	}

	matches.swap(inliers);
	return (matches.size() > minNumberMatchesAllowed);
}

//判断一个数srcNumber是否在目标向量中
//在返回1， 否则返回0
bool PatternMatch::findElement(uint& srcNumber, 
	std::vector<uint>& dstNumberVector)
{
	for (uint dstNumber : dstNumberVector){
		if (dstNumber == srcNumber){
			return 1;
		}
	}
	return 0;
}

//画出外极线,对应点已经经过了RANSAC优化
void DrawEpiLines(const Mat& img_1, 
	const Mat& img_2, 
	vector<Point2f>points1, 
	vector<Point2f>points2)
{

	cv::Mat F = cv::findFundamentalMat(points1, points2, CV_FM_8POINT);
	std::vector<cv::Vec<float, 3>> epilines1, epilines2;
	cv::computeCorrespondEpilines(points1, 1, F, epilines1);
	cv::computeCorrespondEpilines(points2, 2, F, epilines2);
	//draw epilines
	cv::Mat img1, img2;
	if (img_1.type() == CV_8UC3)
	{
		img_1.copyTo(img1);
		img_2.copyTo(img2);
	}
	else if (img_1.type() == CV_8UC1)
	{
		cvtColor(img_1, img1, COLOR_GRAY2BGR);
		cvtColor(img_2, img2, COLOR_GRAY2BGR);
	}
	else
	{
		cout << "unknow img type\n" << endl;
		exit(0);
	}

	cv::RNG& rng = theRNG();
	for (uint i = 0; i < points2.size(); i++)
	{
		Scalar color = Scalar(rng(256), rng(256), rng(256));

		circle(img2, points2[i], 5, color);
		line(img2, Point(0,
			-epilines1[i][2] / epilines1[i][1]), 
			Point(img2.cols, -(epilines1[i][2] + epilines1[i][0] * img2.cols) / epilines1[i][1]), 
			color);

		circle(img1, points1[i], 4, color);
		line(img1, 
			cv::Point(0, -epilines2[i][2] / epilines2[i][1]), 
			cv::Point(img1.cols, -(epilines2[i][2] + epilines2[i][0] * img1.cols) / epilines2[i][1]), 
			color);
		
	}
	cv::imshow("img2 epiline1", img2);
	cv::imshow("img1 epiline2", img1);

	waitKey(0);
}

//把特征点和描述子写到.key文件中
/* #keypoints #dimension
 keypoint postion scale orientation
 descriptor
 ....
*/
bool WriteKeyPointsWithDescriptors(string imgName, 
	const std::vector<cv::KeyPoint>&keypoints, 
	const cv::Mat&descriptor)
{
	size_t length = imgName.length();
	imgName[length - 3] = 'k';
	imgName[length - 2] = 'e';
	imgName[length - 1] = 'y';

	size_t keyNumbers = keypoints.size();
	size_t lengthDescriptor = descriptor.cols;//the cols of decs Mat is the Dimension of Descriptor

	std::ofstream outfile;
	outfile.open(imgName, ios::out | ios::trunc);
	if (!outfile.is_open()){
		cout << "Write file error: " << imgName << endl;
		return 0;
	}

	outfile << keyNumbers << ' ' << lengthDescriptor << endl;
	const uchar* ptrData = nullptr;
	for (int i = 0; i < keyNumbers; ++i)
	{
		outfile << keypoints[i].pt.y 
			<< ' ' << keypoints[i].pt.x
			<< ' ' << keypoints[i].octave 
			<< ' ' << keypoints[i].angle;
		ptrData = descriptor.ptr<uchar>(i);
		for (int j = 0; j < lengthDescriptor; ++j)
		{
			if(j%20 == 0) outfile << endl;
			outfile << ' '<< (uint)ptrData[j];
		}
		outfile << endl;
	}
	outfile.close();
	return 1;
}

/**write the initial matches pair to the matches.init.txt
format imgIndex1 imgIndex2
 #matches
 matches.queryIdx matches.trainIdx
 ...
*/
bool WriteMatches(const int imgIndex1, 
	const int imgIndex2, 
	const std::vector<cv::DMatch>& matches)
{
	static bool firstTime = 1;
	string filename = "matches.init.txt";
	filename = gl_ImgPathDB + filename;
	std::ofstream outfile;
	//rewrite the file when open it for the first time
	//then append the file
	if (firstTime) {
		firstTime = 0;
		outfile.open(filename, ios::out | ios::trunc);
	}
	else outfile.open(filename, ios::out | ios::app);

	if (!outfile.is_open()){
		cout << "Write file error: " << filename << endl;
		return 0;
	}
	size_t size = matches.size();
	outfile << imgIndex1 << ' ' << imgIndex2 << endl;
	outfile << size << endl;
	for (int i = 0; i < size; ++i){
		outfile << matches[i].queryIdx << " "
			<< matches[i].trainIdx << endl;
	}
	outfile.close();
	return 1;
}