#ifndef CONVERTBUNDLE_H
#define CONVERTBUNDLE_H


#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "parsejson.h"
class ParseJson;

class BundleConvert
{
public:
	BundleConvert(const std::string &s1, const std::string &s2, const size_t t) 
		:sfm_data_json(s1),ply_file(s2), image_number(t) 
	{
		image_mask.resize(t,0);
	};

	~BundleConvert(){ std::cout << "Ply file convert finished!" << std::endl; };

	bool ReadIntricExtric();
	bool ReadPly();
	bool ReadStructure();
	bool Convert2Bundle(std::string bundler_out, const ParseJson &sfm_data_json);

private:
	bool GetTranslation();
	bool ReadExtrinsics(std::ifstream & infile, std::string &word);
	bool ReadIntrinsics(std::ifstream & infile, std::string &word);
	bool ReadStructure(std::ifstream & infile, std::string &word);

private:

	std::string sfm_data_json;
	std::size_t image_number;
	std::vector<std::vector<double>> camera_f_k1_k2;
	std::vector<bool> image_mask;
	std::vector<std::vector<double>> r;
	std::vector<std::vector<double>> t;
	std::vector<std::vector<double>> c;

	std::string ply_file;
	std::vector<std::vector<double>> point_position;
	std::vector<std::vector<int>> point_rgb;
	std::vector<std::vector<std::vector<double>>> view_list;

};

#endif