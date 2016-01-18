#ifndef IMAGEPREPARE_H
#define IMAGEPREPARE_H

#include "iostream"
#include "fstream"
#include "string"
#include "io.h"  
#include <opencv2/opencv.hpp>
#include "globalSetting.h"


class Image{
public:

	//默认是查询图像路径
	Image(
		std::string imgPath = gl_ImgPathQuery,
		std::string imgListPath = gl_ImgListPathQuery
		);

	~Image();

	//读取image路径下的图片名称，并存储在list_**.txt中
	//每一行是一个图片的路径
	void MakeImgListTxt() const;

	//读取图像文件列表,保存在imageList中
	bool MakeImgListVector();

	//显示图片列表中的图片
	void ShowImage(uint numberMax = 5)const;
	
	//返回图像文件名向量
	std::vector<std::string> ReturnImgListVector()const;

private:
	//图片所在路径
	std::string imgPath;
	
	//imgListPath - 图像列表文件list_**.txt所在路径
	std::string imgListPath;
	
	//存储图片的文件名向量（带路径）
	std::vector<std::string> imgListVector;
};


#endif