#include "Blur.h"

int Blur::BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method)
{
	Convolution cCon;
	vector<float> temp;
	Mat tempImg;

	if (method == 0) {
		// Find kernel
		for (int i = 0; i < kWidth * kHeight; i++) {
			temp.push_back(float(1) / float(kWidth * kHeight));
		}

		destinationImage = sourceImage.clone();

		cCon.SetKernel(temp, kWidth, kHeight);
		cCon.DoConvolution(sourceImage, tempImg);
		for (int i = 0; i < sourceImage.rows; i++)
			for (int j = 0; j < sourceImage.cols; j++)
				destinationImage.at<uchar>(i, j) = (uchar)tempImg.at<float>(i, j);
	}
	else if (method == 1) {
		//xử lý lọc trung vị
		destinationImage.create(sourceImage.rows, sourceImage.cols, sourceImage.type());
		//width là chiều rộng ảnh, height là chiều cao ảnh
		int width = sourceImage.cols, height = sourceImage.rows;
		//nChannels là số kênh màu
		int nChannels = sourceImage.channels();
		//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
		int widthStep = sourceImage.step[0];

		//Tạo bảng offsets
		int kHalfWidth = kWidth >> 1;
		int kHalfHeight = kHeight >> 1;

		vector<int> offsets;

		for (int y = -kHalfHeight; y <= kHalfHeight; y++)
			for (int x = -kHalfWidth; x <= kHalfWidth; x++)
				offsets.push_back(y * widthStep + x);
		int n = offsets.size();

		uchar* I = new uchar[n];
		uchar* pData = (uchar*)destinationImage.data;
		uchar* psData = (uchar*)sourceImage.data;

		for (int i = 0; i < height; i++, psData += widthStep, pData += destinationImage.step[0])
		{
			uchar* pRow = pData;
			uchar* psRow = psData;
			for (int j = 0; j < width; j++, psRow += nChannels, pRow += 1)
			{
				//Tìm trung vị của các điểm ảnh theo kSize
				for (int k = 0; k < n; k++)
				{
					I[k] = psRow[offsets[k]];
				}
				//sắp xếp
				for (int i = 0; i < n / 2 + 1; i++)
					for (int j = i + 1; j < n; j++)
						if (I[i] > I[j])
						{
							I[i] ^= I[j];
							I[j] ^= I[i];
							I[i] ^= I[j];
						}
				// Gán giá trị điểm ảnh là giá trị trung vị tìm được
				pRow[0] = I[n / 2];
			}
		}
		if (I != NULL) {
			delete[] I;
		}
	}
	else if (method == 2) {
		float PI = 3.141592;
		float sigma = 1.0;
		float value;

		for (int cnt = 0; cnt < kWidth * kHeight; cnt++) {
			int i = cnt % kWidth;
			int j = cnt / kHeight;

			value = expf(-(powf((i - kWidth / 2), 2) + powf((j - kHeight / 2), 2)) / (2 * sigma * sigma)) / (2 * PI * sigma * sigma);
			temp.push_back(value);
		}

		destinationImage = sourceImage.clone();

		cCon.SetKernel(temp, kWidth, kHeight);
		cCon.DoConvolution(sourceImage, tempImg);
		for (int i = 0; i < sourceImage.rows; i++)
			for (int j = 0; j < sourceImage.cols; j++)
				destinationImage.at<uchar>(i, j) = (uchar)tempImg.at<float>(i, j);
	}
	else
		return 1;

	return 0;
}

Blur::Blur()
{
}

Blur::~Blur()
{
}
