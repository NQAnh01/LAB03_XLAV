#include "Convolution.h"

vector<float> Convolution::GetKernel()
{
	return _kernel;
}

void Convolution::SetKernel(vector<float> kernel, int kWidth, int kHeight)
{
	this->_kernelHeight = kHeight;
	this->_kernelWidth = kWidth;

	for (int i = 0; i < kernel.size(); i++)
		this->_kernel.push_back(kernel[i]);
}

int Convolution::DoConvolution(const Mat& sourceImage, Mat& destinationImage)
{
	int nRow = sourceImage.rows,
		nCol = sourceImage.cols;

	// tạo và tính offset
	vector<int> y_offset;
	vector<int> x_offset;

	int halfHeight = floor(this->_kernelHeight / 2);
	for (int i = -halfHeight; i <= halfHeight; i++) {
		for (int cnt = 0; cnt < this->_kernelHeight; cnt++)
			y_offset.push_back(i);
	}
	
	for (int j = 0; j < this->_kernelHeight; j++) {
		for (int i = -halfHeight; i <= halfHeight; i++) {
			x_offset.push_back(i);
		}
	}

	//Khoi tao anh dich
	destinationImage = Mat(sourceImage.size(), CV_32FC1);

	//Do convolution
	for (int row = 0; row < nRow; row++)// y
	{
		for (int col = 0; col < nCol; col++) //x
		{
			float result = 0;

			//Ilterate mask vector
			for (int k = 0; k < this->_kernelHeight * this->_kernelWidth; k++) {
				// Nếu có offset nào mà tại đó giá trị không truy xuất được thì result = 0
				// Nếu không thì kết quả là tích và được cộng dồn
				int x_temp = col + x_offset[k], y_temp = row + y_offset[k];
				if (y_temp < 0 || y_temp >= nRow || x_temp < 0 || x_temp >= nCol)
					continue;
				result += (this->_kernel[k] * sourceImage.at<uchar>(y_temp, x_temp));
			}
			//Assign sum to dest pixel(row,col)
			destinationImage.at<float>(row, col) = result;
		}
	}
	return 0;
}

Convolution::Convolution()
{
	_kernelHeight = 0;
	_kernelWidth = 0;
}

Convolution::~Convolution()
{
}
