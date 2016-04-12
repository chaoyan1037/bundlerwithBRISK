#include "parsejson.h"
#include <omp.h>


using namespace std;

//read the image file name in the json file
bool ParseJson::ReadImageFileName(){
	std::ifstream sfm_data_json;
	sfm_data_json.open(json_filename);
	if (!sfm_data_json.is_open())
	{
		cout << "Could not open the json file\n";
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

			//std::cout << word << " ";
			image_filename.push_back(word);
		}
		else if (word == "\"id_intrinsic\":")
		{
			string id;
			size_t id_number(0);
			//size_t idx = UINT_MAX;
			words_a_line >> id;
			id.erase(id.end() - 1);
			//cout << SIZE_MAX << endl;
			id_number = stoul(id, 0, 10)>USHRT_MAX ? 0 : stoul(id, 0, 10);
			id_intrinsic.push_back(id_number);
			//std::cout << id_number << std::endl;
		}
		else if (word == "\"focal_length\":")
		{
			string focal;
			double dfocal_length;
			words_a_line >> focal;
			focal.erase(focal.end()-1);
			dfocal_length = stod(focal, 0);
			focal_length.push_back(dfocal_length);
			//std::cout << dfocal_length << std::endl;
		}
		else if (word == "\"root_path\":")
		{
			string path;
			words_a_line >> path;
			path.erase(path.begin());
			path.erase(path.end()-2, path.end());
			image_filepath = path;
		}
	}
	//cout << image_filename.size() << endl;
	img_number = image_filename.size();
	sfm_data_json.close();
	return 1;
}

bool ParseJson::WriteImageList()
{
	//image_filepath = "D:/Localization/Montrel/images/db";
	string s1(image_filepath + "/list.txt");
	string s2(image_filepath + "/list_db.txt");

	std::ofstream list_txt, list_db_txt;
	list_txt.open(image_filepath + "/list.txt", ios::trunc | ios::out);
	list_db_txt.open(image_filepath + "/list_db.txt", ios::trunc | ios::out);

	if (!list_txt.is_open() || !list_db_txt.is_open())
	{
		cout << "Could not open the list file\n";
		return 0;
	}

	
	for (int i = 0; i < image_filename.size(); ++i)
	{
		//list_db_txt << image_filepath + "/" + image_filename[i] << endl;

		//list_txt << image_filepath + "/" + image_filename[i] << " 0 ";
		//list_txt << focal_length[id_intrinsic[i]] << endl;
		
		list_db_txt <<  "/images/db/"+image_filename[i] << endl;
		//list_txt << "/images/query/" + image_filename[i] << endl;
	}

	list_txt.close();
	list_db_txt.close();
	return 1;
}

