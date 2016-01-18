#include "parsebundle.h"

using namespace std;

//解析bundle.out文件，提取出相机的姿态（R，t）,存到Camera结构中
/*Each camera entry contains the estimated camera intrinsics and extrinsics, and has the form:

<f> <k1> <k2>   [the focal length, followed by two radial distortion coeffs]
<R>             [a 3x3 matrix representing the camera rotation]
<t>             [a 3-vector describing the camera translation]
*/
bool Bundle::SparseBundleOut(const std::string bundlePath){
	std::ifstream bundleOutFile;
	bundleOutFile.open(gl_BundleOutFilePath);
	if (!bundleOutFile.is_open())
	{
		cout << "Could not open the bundleout file\n";
		return 0;
	}

	
	char fileLine[256];
	uint caremaNumber[2];

	bundleOutFile.getline(fileLine, 256);//# Bundle file v0.3
	std::string version(fileLine);
	std::string truth("# Bundle file v0.3");
	if (version != truth){
		cout << "Open wrong budle.out file.\n" << endl;
	}

	bundleOutFile.getline(fileLine, 256);//#camera #points
	cout << fileLine << endl;
	sscanf_s(fileLine, "%d %d", &(caremaNumber[0]), &(caremaNumber[1]));

	Camera tCamera;
	uint i = caremaNumber[0];
	while (i){
		bundleOutFile.getline(fileLine, 256); cout << fileLine << endl;
		sscanf_s(fileLine, "%f %f %f", &(tCamera.f), &(tCamera.k1), &(tCamera.k2));

		float rotation[9];
		bundleOutFile.getline(fileLine, 256); cout << fileLine << endl;
		sscanf_s(fileLine, "%f %f %f", &(rotation[0]), &(rotation[1]), &(rotation[2]));
		bundleOutFile.getline(fileLine, 256); cout << fileLine << endl;
		sscanf_s(fileLine, "%f %f %f", &(rotation[3]), &(rotation[4]), &(rotation[5]));
		bundleOutFile.getline(fileLine, 256); cout << fileLine << endl;
		sscanf_s(fileLine, "%f %f %f", &(rotation[6]), &(rotation[7]), &(rotation[8]));
		cv::Matx33f tRotation(rotation);
		tCamera.R = tRotation;
		
		bundleOutFile.getline(fileLine, 256); cout << fileLine << endl;
		sscanf_s(fileLine, "%f %f %f", &(tCamera.t[0]), &(tCamera.t[1]), &(tCamera.t[2]));
		
		CameraListVector.push_back(tCamera);
		--i;
	}
	bundleOutFile.close();
	if( 0 == i) return 1;
	else return 0;
}
