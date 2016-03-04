#ifndef CONVERTDESC_H
#define CONVERTDESC_H
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class DescFeatConvert 
{
public:

	DescFeatConvert(const std::string &s1, const std::string &s2) 
		:desc_feat_filepath(s1), key_filepath(s2)
	{
		std::cout << "DescFeatConvert Constructed!" << std::endl;
	};
	~DescFeatConvert(){ std::cout << "DescFeatConvert Destructed!" << std::endl; };

	bool ReadDesc( std::string &s);
	bool ReadFeat( std::string &s);
	bool WriteConvertedKey( std::string &s);
	bool ConvertAll(std::vector<std::string> &vs);

//private:

	std::string desc_feat_filepath;
	std::string key_filepath;
	std::vector<std::vector<unsigned char>> desc;
	std::vector<std::vector<double>> feat; //x, y, size, angle
};



#endif