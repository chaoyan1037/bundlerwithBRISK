#include <iostream> 
#include <fstream>
#include <string>

#include "convertDesc.h"
#include "parsejson.h"
#include "convertBundle.h"

using namespace std;


std::string gl_ImgPathDB = "D:/openMVG/d1/";

std::string gl_sfm_data_json = "D:/openMVG/d1/g_reconstruction/sfm_data.json";
std::string gl_ply_file = "D:/openMVG/d1/g_reconstruction/colorlized.ply";
std::string gl_desc_feat = "D:/openMVG/d1/s_matches/";

int main(int argc, char **argv)
{
#if 1
	ParseJson sfm_data_json(gl_sfm_data_json);
	sfm_data_json.ReadImageFileName();
	sfm_data_json.WriteImageList();

	DescFeatConvert desc_feat_converter(gl_desc_feat, gl_ImgPathDB);
	//desc_feat_converter.ConvertAll(sfm_data_json.image_filename);

	BundleConvert  bundle_converter(gl_sfm_data_json, gl_ply_file, sfm_data_json.ImageNumber());

	//bundle_converter.ReadIntricExtric();
	//bundle_converter.ReadPly();
	//bundle_converter.Convert2Bundle(gl_ImgPathDB+"bundler.rd.out", sfm_data_json);

#else

	istringstream stream;
	string word;
	stream.str("wo shi");
	stream >> word;
	cout << word << endl; 

	stream.str("ni yanchao");
	stream >> word;
	cout << word << endl;
#endif
	cout << "main end" << endl;
	return 0;
}
