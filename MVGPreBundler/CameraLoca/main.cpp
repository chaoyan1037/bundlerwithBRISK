

#include <iostream> 
#include <fstream>
#include <opencv2/opencv.hpp>

#include "pattern.h"
#include "convertDesc.h"
#include "parsejson.h"

using namespace std;
using namespace cv;


std::string gl_ImgPathDB = "D:/openMVG/d1/";
std::string gl_ImgListDB = "list.txt";

std::string gl_sfm_data_json = "D:/openMVG/d1/s_matches/sfm_data.json";
std::string gl_desc_feat = "D:/openMVG/d1/s_matches/";

int main(int argc, char **argv)
{

	ParseJson sfm_data_sjon(gl_sfm_data_json);
	sfm_data_sjon.ReadImageFileName();
	sfm_data_sjon.WriteImageList(gl_ImgPathDB + gl_ImgListDB);

	DescFeatConvert converter(gl_desc_feat, gl_ImgPathDB);
	converter.ConvertAll(sfm_data_sjon.image_filename);

	return 0;
}
