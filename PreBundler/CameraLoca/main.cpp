

#include <iostream> 
#include <fstream>
#include <opencv2/opencv.hpp>

#include "globalSetting.h"
#include "pattern.h"
#include "imageprepare.h"
#include "parsebundle.h"

using namespace std;
using namespace cv;

//void pngImageProcess();
//void pngImgToJpg();
std::string gl_BundleOutFilePath = "../../database/bundle/bundle.rd.out";
std::string gl_ImgPathQuery = "../../database/images/query/";
std::string gl_ImgPathDB = "../../database/images/db/";
std::string gl_ImgListPathQuery = "../../database/list/list_query.txt";
std::string gl_ImgListDB = "../../database/list/list_db.txt";

int main(int argc, char **argv)
{
	cout << argv[0] << endl;
	if (argc == 2)//run with cygwin
	{
		gl_ImgPathDB = "./";
		gl_ImgListDB = "list.txt";
	}
	//Bundle bundleOut;
	//bundleOut.SparseBundleOut();

	
	//该函数在更查询图片后只用执行一次，参数：imgPath, imgListPath
	//Image imageQuery(gl_ImgPathQuery, gl_ImgListPathQuery);
	//imageQuery.MakeImgListTxt();
	////生成图片名列表向量
	//imageQuery.MakeImgListVector();
	//imageQuery.ShowImage();
	//std::vector<std::string> imgListVectorQuery;
	//imgListVectorQuery = imageQuery.ReturnImgListVector();
#if 1
	//该函数在更换数据库图片后只用执行一次，参数：imgPath, imgListPath
	Image imageDB(gl_ImgPathDB, gl_ImgListDB);
	//imageDB.MakeImgListTxt();
	//生成图片名列表向量
	imageDB.MakeImgListVector();  
	//imageDB.ShowImage();
	std::vector<std::string> imgListVectorDB;
	imgListVectorDB = imageDB.ReturnImgListVector();
#endif

#if 0
	vector<string> imgListVectorDB;
	imgListVectorDB.push_back("database/images/db/kermit000.jpg");
	imgListVectorDB.push_back("database/images/db/kermit001.jpg");
	imgListVectorDB.push_back("database/images/db/kermit002.jpg");
	imgListVectorDB.push_back("database/images/db/kermit003.jpg");
	imgListVectorDB.push_back("database/images/db/kermit004.jpg");
	imgListVectorDB.push_back("database/images/db/kermit005.jpg");
	imgListVectorDB.push_back("database/images/db/kermit006.jpg");
	imgListVectorDB.push_back("database/images/db/kermit007.jpg");
	imgListVectorDB.push_back("database/images/db/kermit008.jpg");
	imgListVectorDB.push_back("database/images/db/kermit009.jpg");
	imgListVectorDB.push_back("database/images/db/kermit010.jpg");
#endif

	PatternMatch imgPatMatch;
	imgPatMatch.BuildPatternFromImage(imgListVectorDB);
	
	uint imgListSize = (uint)imgListVectorDB.size();
	std::vector<uint> dstNumberVector(imgListSize);
	for (uint i = 0; i < imgListSize; i++){
		dstNumberVector[i]= i ;
	}
	uint srcNumber = 0;
	//imgPatMatch.MatchPatternOneToAll(srcNumber, dstNumberVector);
	imgPatMatch.MatchPatternTwoInAll(dstNumberVector);

	//pngImageProcess();
	//pngImgToJpg();
	return 0;
}


#if 0
void pngImageProcess(){
	const string pngImagePath = "E:\\Ucas\\lab\\Project\\seq-01\\*color.png";
	const string imgListPath = "E:\\Ucas\\lab\\Project\\imgList.txt";

	ofstream fileList;
	fileList.open(imgListPath, ios::trunc | ios::out);

	_finddata_t file;
	intptr_t lf;

	//输入文件夹路径  
	if ((lf = _findfirst(pngImagePath.c_str(), &file)) == -1){
		fileList << file.name << endl;
		cout << "No Images Found!" << endl;
	}
	else{
		
		//cout << "file name list:" << endl;
		while (_findnext(lf, &file) == 0){
			cout << file.name << endl;
			fileList << file.name << endl;
			//fileList <<"E:\\Ucas\\lab\\Project\\seq-01\\"<< file.name << endl;
		}
		fileList.close();
	}
	_findclose(lf);

}

void pngImgToJpg(){
	const string imgListPath = "E:\\Ucas\\lab\\Project\\imgList.txt";
	ifstream fileList;
	fileList.open(imgListPath, ios::in);

	std::vector<string> imgListVector;

	string path = "E:\\Ucas\\lab\\Project\\seq-01\\";
	char nameRead[256];
	while (fileList.getline(nameRead, 256)){
		string name = path + nameRead;
		cout << name << endl;
		imgListVector.push_back(name);
	}

	cv::Mat img;
	for (uint i = 0; i < imgListVector.size();){
		img = imread(imgListVector[i]);
	}

	fileList.close();

}
#endif