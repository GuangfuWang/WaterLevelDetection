#pragma once
#include "util.h"
#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>

namespace water_level
{
class WaterLevelDetection final
{
public:
	WaterLevelDetection();
	~WaterLevelDetection();

	void detect(cv::Mat &curr_img, int &res);
	void draw(cv::Mat &img);
private:
	void detectWithGPU(cv::cuda::GpuMat &curr_img, std::vector<cv::Vec4i> &lines);
	void detectWithCPU(cv::Mat &curr_img, std::vector<cv::Vec4i> &lines);
	static void removeUnrelatedLines(std::vector<cv::Vec4i>& detected_lines);
	static bool isLower(std::vector<cv::Vec4i>& lines,std::vector<unsigned int>& thres);

private:
	std::vector<cv::Vec4i> m_prev_detects;
	std::vector<float> m_prev_res;
	cv::Ptr<cv::cuda::HoughSegmentDetector> m_gpu_detector;
	cv::Ptr<cv::cuda::CannyEdgeDetector> m_canny;
	cv::Mat m_mask;
	int m_single_cnt = 0;
	int m_multi_cnt = 0;
	int m_res = 0;
	int m_res_cnt = 0;
};
}