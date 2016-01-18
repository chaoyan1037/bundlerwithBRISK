#ifndef PATTERN_H
#define PATTERN_H

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2\opencv.hpp>
//#include "opencv2/nonfree/nonfree.hpp"  //opencv2 sift
#include <opencv2\xfeatures2d.hpp> //opencv301

#include "globalSetting.h"

//为当前的Pattern存储
struct MatchesInvertedIndex{
	uint matchedImgNumber;
	uint matchedImgMatchesNumber;
};

//为当前的Pattern存储匹配成功的图像序号和matches
struct MatchesImage{
	//匹配成功的图片的序号
	uint									matchesNumber;
	std::vector<cv::DMatch>					matches;
};

//存储每个的图像数据
struct Pattern {
	cv::Mat									img;
	
	std::vector<MatchesImage>				matchedImg;
	std::vector<MatchesInvertedIndex>		matchedImgInvertedIndex;

	std::vector<cv::KeyPoint>				keypoints;
	cv::Mat									descriptors;
	
	std::vector<cv::Point2f>				points2d;
	std::vector<cv::Point3f>				points3d;
	
	//保存当前图片的姿态，从bundle.out文件中读取姿态
	//cv::Matx34f			  cameraPose;
};

class PatternMatch
{
public:
	//构造函数
	PatternMatch();
	
	//析构函数
	~PatternMatch();

	//给所有图片构造pattern
	void BuildPatternFromImage(const std::vector<std::string>& imgList);

	//把指定的图片集中所有图片两两匹配
	void MatchPatternTwoInAll(std::vector<uint> dstNumberVector);

	//把一个图片和其他所有图片匹配,得到匹配和匹配列表
	//匹配列表中保存了匹配成功的图片的序号
	void MatchPatternOneToAll(const std::vector<uint>& srcNumberVector, const uint& dstNumber);

protected:

	//匹配两个模式（图像）,基础矩阵优化匹配，匹配成功返回1，否则返回0
	bool MatchPatternOneToOne(const Pattern& src, const Pattern& dst, std::vector<cv::DMatch>& matches);

	//通过单应优化匹配，剔除外点
	static bool RefineMatchesWithHomography(
		const std::vector<cv::KeyPoint>& queryKeypoints,
		const std::vector<cv::KeyPoint>& trainKeypoints,
		const float homographyReproThres,
		std::vector<cv::DMatch>& matches,
		cv::Mat& homography);

	//通过基础矩阵约束优化匹配，剔除外点
	static bool RefineMatchesWithFundamental(
		const std::vector<cv::KeyPoint>& queryKeypoints,
		const std::vector<cv::KeyPoint>& trainKeypoints,
		const float reprojectionThreshold,
		std::vector<cv::DMatch>& matches,
		cv::Mat& fundamental);
	//判断一个数srcNumber是否在目标向量中,在返回1， 否则返回0
	static bool findElement(uint& srcNumber, std::vector<uint>& dstNumberVector);

private:
	const float						homographyReproThres;
	std::vector<Pattern>			mPattern;
	cv::Ptr<cv::Feature2D>			mPtrFeature;
	cv::Ptr<cv::DescriptorMatcher>	mPtrMatcher;
	cv::Ptr<cv::Feature2D>			mPtrXfeature;

};


bool WriteKeyPointsWithDescriptors(std::string imgName,
	const std::vector<cv::KeyPoint>& keypoints,
	const cv::Mat& descriptor);

bool WriteMatches(const int imgIndex1,
	const int imgIndex2,
	const std::vector<cv::DMatch>& matches);

#endif