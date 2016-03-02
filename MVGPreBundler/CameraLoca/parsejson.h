#ifndef PARSEJSON_H
#define PARSEJSON_H

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <iostream>

class ParseJson
{
public:
	ParseJson(const std::string &filename) :json_filename(filename){};
	bool ReadImageFileName();
	bool WriteImageList(const std::string &s);
	
	std::vector<std::string> image_filename;

private:
	std::string json_filename;
	std::vector<std::size_t> id_intrinsic;
	std::vector<std::double_t> focal_length;
};

#endif