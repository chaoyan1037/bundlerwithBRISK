#ifndef PARSEBUNDLE_H
#define PARSEBUNDLE_H

#include <fstream>
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include "globalSetting.h"

struct Camera
{
	float				f;
	float				k1;
	float				k2;
	cv::Matx33f			R;
	float				t[3];
};

class Bundle{
public:
	bool SparseBundleOut(const std::string bundlePath = gl_BundleOutFilePath);
	bool GetCaremaPara(const std::ifstream& bundleOutFile, Camera& oneCamera);

private:
	std::vector<Camera> CameraListVector;
};

#endif