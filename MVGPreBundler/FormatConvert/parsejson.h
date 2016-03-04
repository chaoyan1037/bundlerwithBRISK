#ifndef PARSEJSON_H
#define PARSEJSON_H

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "convertbundle.h"

class ParseJson
{
public:
	

	ParseJson(const std::string &filename) :json_filename(filename),image_filepath(""){};
	
	bool ReadImageFileName();
	bool WriteImageList();

	std::size_t ImageNumber(){ return image_filename.size();}

	friend bool Convert2Bundle(const ParseJson &sfm_data_json);

	std::vector<std::string> image_filename;

//private:
	
	std::size_t img_number;
	std::string json_filename;
	std::string image_filepath;
	std::vector<std::size_t> id_intrinsic;
	//std::vector<std::size_t> id_pose;
	std::vector<std::double_t> focal_length;
};

#endif