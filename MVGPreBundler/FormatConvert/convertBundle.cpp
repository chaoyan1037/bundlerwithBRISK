#include "convertBundle.h"

using namespace std;

bool BundleConvert::ReadIntrinsics(std::ifstream &infile, std::string &word)
{
	if (!infile.is_open())
	{
		cout << "ReadExtrinsics failed: " << endl;
		return 0;
	}
	vector<double> double_vector;
	string line;
	while (getline(infile, line))
	{
		istringstream words_a_line(line);
		
		words_a_line >> word;
		if (word == "\"focal_length\":")
		{
			words_a_line >> word;
			word.erase(word.end() - 1);

			double focal = stod(word);
			double_vector.clear();
			double_vector.push_back(focal);
		}
		else if (word == "\"disto_k3\":")
		{
			getline(infile, line);
			line.erase(line.end() - 1);
			double_vector.push_back(stod(line));

			getline(infile, line);
			line.erase(line.end() - 1);
			double_vector.push_back(stod(line));

			camera_f_k1_k2.push_back(double_vector);
		}
		else if (word == "\"extrinsics\":")
		{
			return 1;
		}
	}

	return 1;
}

bool BundleConvert::ReadExtrinsics(std::ifstream &infile, std::string &word)
{
	if (!infile.is_open())
	{
		cout << "ReadExtrinsics failed: "  << endl;
		return 0;
	}

	//r.resize(image_number);
	//c.resize(image_number);

	vector<double> double_vector;
	string line;
	int i = 0;
	while (getline(infile, line))
	{
		istringstream words_a_line(line);
		words_a_line >> word;
		if (word == "\"key\":")
		{
			words_a_line >> word;
			word.erase(word.end());
			i = stoi(word);
			image_mask[i] = 1;
		}
		else if (word == "\"rotation\":")
		{
#if 1
			double_vector.clear();
			getline(infile, line);//[
			getline(infile, line);
			line.erase(line.end() - 1);
			double_vector.push_back(stod(line));

			getline(infile, line);
			line.erase(line.end() - 1);
			double_vector.push_back(stod(line));

			getline(infile, line);
			double_vector.push_back(stod(line));

			getline(infile, line);//],
			getline(infile, line);//[
			getline(infile, line);
			line.erase(line.end() - 1);
			double_vector.push_back(stod(line));

			getline(infile, line);
			line.erase(line.end() - 1);
			double_vector.push_back(stod(line));

			getline(infile, line);
			double_vector.push_back(stod(line));

			getline(infile, line);//],
			getline(infile, line);//[
			getline(infile, line);
			line.erase(line.end() - 1);
			double_vector.push_back(stod(line));

			getline(infile, line);
			line.erase(line.end() - 1);
			double_vector.push_back(stod(line));

			getline(infile, line);
			double_vector.push_back(stod(line));

			r.push_back(double_vector);
			
			//r[i].assign(double_vector.cbegin(), double_vector.cend());
#endif
		}
		else if (word == "\"center\":")
		{
			double_vector.clear();
			getline(infile, line);
			line.erase(line.end() - 1);
			double_vector.push_back(stod(line));

			getline(infile, line);
			line.erase(line.end() - 1);
			double_vector.push_back(stod(line));

			getline(infile, line);
			double_vector.push_back(stod(line));

			c.push_back(double_vector);
			//c[i].assign(double_vector.cbegin(), double_vector.cend());
		}
		else if (word == "\"structure\":")
		{
			return 1;
		}
	}

	return 1;
}

bool BundleConvert::ReadStructure(std::ifstream & infile, std::string &word)
{
	if (!infile.is_open())
	{
		cout << "ReadExtrinsics failed: " << endl;
		return 0;
	}
	vector<double> double_vector;
	string line;

	view_list.clear();
	vector<vector<double>> view_list_each;
	while (getline(infile, line))
	{
		istringstream words_a_line(line);
		words_a_line.clear();
		words_a_line.str(line);
		words_a_line >> word;
		//cout << word << endl;
		if (word == "\"key\":")
		{
			view_list_each.clear();
			while (getline(infile, line))
			{
				words_a_line.clear();
				words_a_line.str(line);
				words_a_line >> word;
				if (word == "\"key\":")
				{
#if 1
					words_a_line >> word;//key
					word.erase(word.end() - 1);
					double_vector.clear();
					double_vector.push_back(stod(word));
					getline(infile, line);//value

					getline(infile, line);//id_feat
					words_a_line.clear();
					words_a_line.str(line);
					words_a_line >> word;
					words_a_line >> word;
					word.erase(word.end() - 1);
					double_vector.push_back(stod(word));

					getline(infile, line);//x
					getline(infile, line);//x_value
					words_a_line.clear();
					words_a_line.str(line);
					words_a_line >> word;
					word.erase(word.end() - 1);
					double_vector.push_back(stod(word));

					getline(infile, line);//y_value
					words_a_line.clear();
					words_a_line.str(line);
					words_a_line >> word;
					double_vector.push_back(stod(word));
#endif
					view_list_each.push_back(double_vector);

					getline(infile, line);//]
					getline(infile, line);//}
					getline(infile, line);//}, or }:end
					words_a_line.clear();
					words_a_line.str(line);
					words_a_line >> word;
					if (word == "},")
					{
						continue;
					}
					else if (word == "}")
					{
						view_list.push_back(view_list_each);
						break;
					}
					else
					{
						cout << "read view list error." << endl;
						return 0;
					}

				}

			}
		}
	}
	return 1;
}

bool BundleConvert::ReadIntricExtric()
{
	ifstream infile(sfm_data_json);
	if (!infile.is_open())
	{
		cout << "Open sfm_data.json failed: " << sfm_data_json << endl;
		return 0;
	}

	camera_f_k1_k2.clear();
	r.clear();
	t.clear();
	string line;
	vector<double> double_vector;
	
	while (getline(infile, line))
	{
		istringstream words_a_line(line);
		string word;
		words_a_line >> word;
		if (word == "\"intrinsics\":")
		{
			ReadIntrinsics(infile, word);
		}
		if (word == "\"extrinsics\":")
		{
			ReadExtrinsics(infile, word);
		}
		if (word == "\"structure\":")
		{
			ReadStructure(infile, word);
		}
	}
	GetTranslation();
	return 1;
}

bool BundleConvert::ReadPly()
{
	ifstream infile(ply_file);
	if (!infile.is_open())
	{
		cout << "Open ply failed: " << sfm_data_json << endl;
		return 0;
	}

	string line;
	vector<double> double_vector;
	vector<int> int_vector;
	while (getline(infile, line))
	{
		istringstream words_a_line(line);
		string word;
		words_a_line >> word;
		if (word == "end_header"){
			while (getline(infile, line))
			{
				words_a_line.clear();
				words_a_line.str(line);
				double d_temp;
				int i_temp;
				words_a_line >> d_temp;
				double_vector.clear();
				double_vector.push_back(d_temp);
				words_a_line >> d_temp;
				double_vector.push_back(d_temp);
				words_a_line >> d_temp;
				double_vector.push_back(d_temp);
				point_position.push_back(double_vector);

				words_a_line >> i_temp;
				int_vector.clear();
				int_vector.push_back(i_temp);
				words_a_line >> i_temp;
				int_vector.push_back(i_temp);
				words_a_line >> i_temp;
				int_vector.push_back(i_temp);
				point_rgb.push_back(int_vector);
			}
		}
	}
	//tt.resize(c.size());
	//tt.assign(point_position.end() - c.size(), point_position.end());
	point_position.resize(point_position.size() - c.size());
	point_rgb.resize(point_rgb.size() - c.size());
	//cout << c.size() << endl;
	return 1;
}

bool BundleConvert::GetTranslation()
{
	vector<double> camera_t;
	t.clear();
	//t.resize(image_number);
	for (size_t i = 0; i < r.size(); ++i)
	{
		if (r[i].size() == 0)
		{
			continue;
		}	
		camera_t.clear();
		camera_t.push_back(-(r[i][0] * c[i][0] + r[i][1] * c[i][1] + r[i][2] * c[i][2]));
		camera_t.push_back(-(r[i][3] * c[i][0] + r[i][4] * c[i][1] + r[i][5] * c[i][2]));
		camera_t.push_back(-(r[i][6] * c[i][0] + r[i][7] * c[i][1] + r[i][8] * c[i][2]));
		//t[i].assign(camera_t.cbegin(), camera_t.cend());
		t.push_back(camera_t);
	}

	return 1;
}

bool BundleConvert::Convert2Bundle(string bundler_out,const ParseJson &sfm_data_json)
{
	ofstream bundle(bundler_out, ios::trunc | ios::out);
	if (!bundle.is_open())
	{
		cout << "Open bundle.out failed." << endl;
		return 0;
	}

	bundle << "# Bundle file v0.3" << endl;
	bundle << image_number << " " << point_position.size() << endl;
	//write camera parameters
	for (size_t i = 0, j = 0; i < image_number; ++i)
	{
		if (image_mask[i])//success
		{
			auto &id_intrin = sfm_data_json.id_intrinsic[i];
			bundle << camera_f_k1_k2[id_intrin][0] << " " << camera_f_k1_k2[id_intrin][1] << " "
				<< camera_f_k1_k2[id_intrin][2] << endl;

			bundle << r[j][0] << " " << r[j][1] << " " << r[j][2] << endl;
			bundle << r[j][0] << " " << r[j][1] << " " << r[j][2] << endl;
			bundle << r[j][0] << " " << r[j][1] << " " << r[j][2] << endl;

			bundle << t[j][0] << " " << 0 << " " << 0 << endl;
			++j;
		}
		else{
			bundle << 0 << " " << 0 << " " << 0 << endl;
			bundle << 0 << " " << 0 << " " << 0 << endl;
			bundle << 0 << " " << 0 << " " << 0 << endl;
			bundle << 0 << " " << 0 << " " << 0 << endl;
			bundle << 0 << " " << 0 << " " << 0 << endl;
		}
	}

	//write points parameters
	for (size_t i = 0; i < point_position.size(); ++i)
	{
		bundle << point_position[i][0] << " "
			<< point_position[i][0] << " "
			<< point_position[i][0] << " " << endl;

		bundle << point_rgb[i][0] << " "
			<< point_rgb[i][1] << " "
			<< point_rgb[i][2] << " " << endl;

		bundle << view_list[i].size() << " ";
		for (size_t j = 0; j < view_list[i].size(); ++j)
		{
			bundle << (int)view_list[i][j][0] << " "
				<< (int)view_list[i][j][1] << " "
				<< view_list[i][j][2] << " "
				<< view_list[i][j][3] << " ";
		}
		bundle << endl;
	}
	bundle.close();
	return 1;
}