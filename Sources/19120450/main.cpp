#include "EdgeDetector.h"
#include "Blur.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	EdgeDetector eDect;
	Blur blur;
	vector<string> s;
	Mat src, des;
	int kx, ky, mode;
	string outputName = "";
	 ///Kiểm tra số lượng tham số truyền vào 
	if (argc < 3) {
		cerr << "Not enough argument passed!" << endl;
		return 0;
	}
	
	for (int i = 0; i < argc; i++) {
		s.push_back(argv[i]);
	}
	// Mở file và kiểm tra
	
	if (s.size() == 4) {
		src = imread(s[2], 1);
		outputName = s[3];
		if (s[1] == "-sobel") {
			mode = 1;
		}
		else if (s[1] == "-prew") {
			mode = 1;
		}
		else if (s[1] == "-prew"){
			mode = 2;
		}
	}
	else {
		src = imread(s[4], 1);
		outputName = s[5];
		kx = atoi(s[2].c_str());
		ky = atoi(s[3].c_str());
	}
	if (src.empty()) {
		cout << "\nkhong the mo file!";
		return 0;
	}

	if (s[1] == "-avg") {
		mode = 0;
	}
	else if (s[1] == "-sobel" || s[1] == "-med") {
		mode = 1;
	}
	else if (s[1] == "-prew" || s[1] == "-gau") {
		mode = 2;
	}
	else if (s[1] == "-lap") {
		mode = 3;
	}

	// Chuyển đổi về ảnh xám
	cvtColor(src, src, COLOR_BGR2GRAY);
	
	if (s.size() == 4) {
		eDect.DetectEdge(src, des, mode);
	}
	else {		
		blur.BlurImage(src, des, kx, ky, mode);
	}

	// Hiển thị ảnh kết quả
	namedWindow("source grayscale");
	imshow("source grayscale", src);

	namedWindow("Output image");
	imshow("Output image", des);
	
	//save the img
	imwrite(outputName, des);
	waitKey(0);
	return 0;
}
