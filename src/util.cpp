#include "util.h"
#include <sys/stat.h>
#include <iostream>
#include <valarray>

namespace water_level
{

thread_local std::chrono::high_resolution_clock::time_point Util::mTic;

bool Util::checkDirExist(const std::string &dir)
{
	///note this feature require c++ 17 and above.
	return std::filesystem::exists(dir);
}

bool Util::checkFileExist(const std::string &file)
{
	///note this feature require c++ 17 and above.
	return std::filesystem::exists(file);
}

void Util::tic()
{
	mTic = std::chrono::high_resolution_clock::now();
}

long Util::toc()
{
	auto dur = std::chrono::high_resolution_clock::now() - mTic;
	mTic = std::chrono::high_resolution_clock::now();
	long ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	return ms;
}

long Util::getFileSize(const std::string &file)
{
	if (!checkFileExist(file)) {
		std::cerr << "File: " << file << " not exists..." << std::endl;
		return -1;
	}
	struct stat statBuf;
	stat(file.c_str(), &statBuf);
	return statBuf.st_size;
}

int Util::cvtStr2Int(const char *arr)
{
	return std::atoi(arr);
}
void Util::softmax(std::vector<float> &in)
{
	float total = 0.0f;
	for (auto &e : in) {
		auto curr = std::exp(e);
		e = curr;
		total += curr;
	}
	for (auto &e : in) {
		e /= total;
	}
}
float Util::getAngle(int x0,int y0,int x1,int y1,
					 int m0,int n0,int m1,int n1)
{
	float m = m1-m0;
	float n = n1-n0;
	float x = x1-x0;
	float y = y1-y0;
	float dot = m*x+y*n;
	float xx = std::sqrt(x*x+y*y);
	float mm = std::sqrt(m*m+n*n);
	float cos_theta = dot/(xx*mm);
	return std::acos(cos_theta);
}
void Util::linearRegression(std::vector<cv::Vec4i> &points)
{
	int minx = 0;
	int maxx = 0;
	for (int i = 0; i < points.size(); ++i) {
		minx = std::min(minx,points[i][0]);
		maxx = std::max(maxx,points[i][2]);
	}

}
void Util::fuseLineWithPluecker(cv::Vec4i &l1_ret, cv::Vec4i &l2)
{

}
}
