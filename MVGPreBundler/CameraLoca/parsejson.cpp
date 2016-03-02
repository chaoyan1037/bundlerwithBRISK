#include "parsejson.h"

using namespace std;

//read the image file name in the json file
bool ParseJson::ReadImageFileName(){
	std::ifstream sfm_data_json;
	sfm_data_json.open(json_filename);
	if (!sfm_data_json.is_open())
	{
		cout << "Could not open the bundleout file\n";
		return 0;
	}

	string line;
	image_filename.clear();
	id_intrinsic.clear();
	focal_length.clear();

	while (getline(sfm_data_json, line))
	{
		istringstream words_a_line(line);
		string word;
		words_a_line >> word;
		if (word == "\"filename\":")
		{
			words_a_line >> word;
			word.erase(word.begin());
			word.erase(word.end()-1);
			word.erase(word.end()-1);

			std::cout << word << " ";
			image_filename.push_back(word);
		}
		if (word == "\"id_intrinsic\":")
		{
			string id;
			size_t id_number;
			words_a_line >> id;
			id.erase(id.end() - 1);
			id_number = stoi(id, 0, 10);
			id_intrinsic.push_back(id_number);
			std::cout << id_number << std::endl;
		}
		if (word == "\"focal_length\":")
		{
			string focal;
			double dfocal_length;
			words_a_line >> focal;
			focal.erase(focal.end()-1);
			dfocal_length = stod(focal, 0);
			focal_length.push_back(dfocal_length);
			//std::cout << dfocal_length << std::endl;
		}
		
	}
	cout << image_filename.size() << endl;

	sfm_data_json.close();
	return 1;
}

bool ParseJson:: WriteImageList(const std::string &s)
{
	std::ofstream list_txt;
	list_txt.open(s, ios::trunc | ios::out);
	if (!list_txt.is_open())
	{
		cout << "Could not open the list file\n";
		return 0;
	}

	for (size_t i = 0; i<image_filename.size(); ++i)
	{
		list_txt << "./" << image_filename[i] << " 0 ";
		list_txt << focal_length[id_intrinsic[i]] << endl;
	}

	list_txt.close();
	return 1;
}