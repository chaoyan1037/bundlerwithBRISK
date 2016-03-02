#include "convertDesc.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;


bool DescFeatConvert::ReadDesc(std::string &s)
{
	s.erase(s.end() - 3, s.end());
	s += "desc";
	ifstream infile(s.c_str());
	if (!infile.is_open())
	{
		cout << "Open desc file failed: " << s << endl;
		return 0;
	}

	desc.clear();
	string line;
	while (getline(infile,line))
	{
		istringstream desc_line(line);
		vector<unsigned char> desc_vector;
		int byte;
		while (desc_line >> byte)
		{
			desc_vector.push_back(byte);
		}
		desc.push_back(desc_vector);
	}
	return 1;
}
bool DescFeatConvert::ReadFeat(std::string &s)
{
	s.erase(s.end() - 3, s.end());
	s += "feat";
	ifstream infile(s.c_str());
	if (!infile.is_open())
	{
		cout << "Open feat file failed: " << s << endl;
		return 0;
	}

	string line;
	feat.clear();
	while (getline(infile, line))
	{
		istringstream feat_line(line);
		double  double_value;
		vector<double> feat_vector;
		while (feat_line >> double_value)
		{
			feat_vector.push_back(double_value);
		}
		feat.push_back(feat_vector);
	}
	return 1;
}
bool DescFeatConvert::WriteConvertedKey(std::string &s)
{
	s.erase(s.end() - 3, s.end());
	s += "key";
	ofstream keyfile(s);
	if (!keyfile.is_open())
	{
		cout << "Open key file failed: " << s << endl;
		return 0;
	}

	keyfile << feat.size() << " " << desc[0].size() << endl;
	for (size_t i = 0; i < feat.size(); ++i)
	{
		keyfile << feat[i][0] << " "
			<< feat[i][1] << " "
			<< feat[i][2] << " "
			<< feat[i][3] << endl;
		int byte;
		for (size_t j = 0; j < desc[0].size(); ++j)
		{
			byte = desc[i][j];
			keyfile << (int)desc[i][j] << " ";
			if (j % 20 == 19) keyfile << endl;
		}
		keyfile << endl;
	}
	keyfile.close();
	return 1;
}

bool DescFeatConvert::ConvertAll(std::vector<std::string> &vs)
{
	for (size_t i = 0; i < vs.size(); ++i)
	{
		ReadDesc(desc_feat_filepath + vs[i]);
		ReadFeat(desc_feat_filepath + vs[i]);
		WriteConvertedKey(key_filepath + vs[i]);
	}
	return 1;
}