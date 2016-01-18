
#include "imageprepare.h"

using namespace cv;
using namespace std;


Image::Image(std::string imgPath, std::string imgListPath)
	:imgPath(imgPath),imgListPath(imgListPath)
{
	cout << "Image Construct.\n" << endl;
	cout << "imgPath:" << imgPath << endl;
	cout << "imgListPath:" << imgListPath << endl;
};

Image::~Image(){
	cout << "Image deconstruct.\n" << endl;
}

//读取image路径下的图片名称，并存储在list_**.txt中
//每一行是一个图片的路径
void Image::MakeImgListTxt()const{
	ofstream fileList;
	fileList.open(imgListPath, ios::trunc | ios::out);
	if (!fileList.is_open()){
		cout << "open image list txt failed." << endl;
	}

	string path = imgPath + "*.jpg";
	_finddata_t file;
	intptr_t lf;

	//输入文件夹路径  
	if ((lf = _findfirst(path.c_str(), &file)) == -1)
		cout << "No Images Found!" << endl;
	else{
		fileList << imgPath << file.name << endl;
		//cout << "file name list:" << endl;
		while (_findnext(lf, &file) == 0){
			//cout << file.name << endl;
			fileList << imgPath << file.name << endl;
		}
	}
	_findclose(lf);
}

//读取图像文件名列表（包括路径）,保存在imageList中
bool Image::MakeImgListVector(){
	ifstream imageListFile;
	//读取图像文件列表
	imageListFile.open(imgListPath);
	if (!imageListFile.is_open())
	{
		cout << "could not open the img list file: " << imgListPath << endl;
		return 0;
	}

	char nameRead[256];
	char *strToken = 0;
	char *nextToken = 0;
	const char *d = " \0";
	while (imageListFile.getline(nameRead, 256)){
		//cout << "strtok1:"<<nameRead << endl;
		strToken = strtok_s(nameRead, d, &nextToken);
		//cout << "strtok2:" << strToken << endl;
		imgListVector.push_back(nameRead);
	}
	return 1;
}


//显示图片列表中的图片
void Image::ShowImage(uint numberMax)const{
	Mat image;

	for (uint i = 0; i < numberMax && i < imgListVector.size(); ++i){
		image = imread(imgListVector[i]);
		if (image.empty()){
			cout << "ShowImage - Fail to Open Img:" << imgListVector[i] << endl;
			continue;
		}
		else {
			imshow(imgListVector[i], image);
			
		}
	}
	waitKey(0);
}

std::vector<std::string> Image::ReturnImgListVector()const{
	std::vector<std::string> tImgListVector(imgListVector);
	return tImgListVector;
}

/*
//给图片提取特征点和描述子，并分别存储在p***.xml,d***.xml文件中
void ExtraFeature(char* keysListPath, char* imageList[], int imageNumber){
	cout << "Go into Function:ExtraFeature" << endl;
	Mat img, outputImg;
	Mat descriptors;
	vector<KeyPoint> keypoints;

	//set brisk parameters
	int Threshl = 40;
	int Octaves = 4; //(pyramid layer) from which the keypoint has been extracted
	float PatternScales = 1.0f;

	Ptr<Feature2D> BRISKD = BRISK::create(Threshl, Octaves, PatternScales);

	for (int i = 0; i < imageNumber; ++i){
		img = imread(imageList[i], CV_LOAD_IMAGE_GRAYSCALE);
		if (img.empty()){
			cout << "Load image error:" << imageList[i] << endl;
		}
		else{
			BRISKD->detect(img, keypoints);
			BRISKD->compute(img, keypoints, descriptors);
			WriteKeyPointsIntoFile(keysListPath, i, keypoints);
			WriteDescriptorsIntoFile(keysListPath, i, descriptors);
			
			drawKeypoints(img, keypoints, outputImg, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
			imshow("Keypoints", outputImg);
			waitKey(0);
		}
	}
	cout << "Go out Function:ExtraFeature" << endl;
}
*/

/*
//把每张图片的特征点写入到.yml中
//格式：
//特征点数量
//特征点序号（0开始），坐标（x,y）
void WriteKeyPointsIntoFile(char* path, int fileNumber,vector<KeyPoint>& keypoints){

	//ofstream file;
	//int num = keypoints.size();
	char filePath[256];
	char fileIndex[8];
	char *fileType = ".yml";

	strcpy_s(filePath, 256, path);
	sprintf_s(fileIndex, "p%d", fileNumber); //int to string
	strcat_s(filePath, 256, fileIndex);
	strcat_s(filePath, 256, fileType);

	FileStorage fs(filePath, FileStorage::WRITE );
	fs << "size" << (int)keypoints.size();
	fs << "capacity" << (int)keypoints.capacity();
	fs << "keypoints" << "[";
	for (unsigned int i = 0; i < keypoints.size(); ++i){
		fs << "{:" << "pt" << "{:" << "x" << keypoints[i].pt.x << "y" << keypoints[i].pt.y << "}";
		fs << "size" << keypoints[i].size << "angle" << keypoints[i].angle << "response" << keypoints[i].response;
		fs << "octave" << keypoints[i].octave << "class_id" << keypoints[i].class_id << "}";
	}
	fs << "]";
	fs.release();

	//file.open(filePath, ios::trunc | ios::out);//输出，如文件存在则删除该文件
	//file << num << endl;
	//for (int i = 0; i < num; ++i){
	//	file << i << '\t' <<keypoints[i].pt.x << '\t' << keypoints[i].pt.y << endl;
	//}
	//file.close();
	cout << "Go out Function:WriteKeyPointsIntoFile" << endl;
}
*/

/*
//把每张图片的描述子写入到.xml文件中
//格式：
//描述子数量
//序号（0开始），描述子
void WriteDescriptorsIntoFile(char* path, int fileNumber, cv::Mat &des){

	char filePath[256];
	char fileIndex[8];
	char *fileType = ".yml";

	strcpy_s(filePath, 256, path);
	sprintf_s(fileIndex, "d%d", fileNumber);//int to string
	strcat_s(filePath, 256, fileIndex);
	strcat_s(filePath, 256, fileType);

	FileStorage fs(filePath, FileStorage::WRITE);
	fs << "rows" << (int)des.rows;
	fs << "cols" << (int)des.cols;
	fs << "data" << "[";
	for (int i = 0; i < des.rows; ++i){
		fs << "[:";
		uchar* data = des.ptr<uchar>(i);
		for (int j = 0; j < des.cols; ++j){
			fs << data[j];
		}
		fs << "]";
	}
	fs << "]";
	fs.release();
	
	cout << "Go out Function:WriteDescriptorsIntoFile" << endl;
}*/

