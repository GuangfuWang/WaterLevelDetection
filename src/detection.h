#pragma once
#include "util.h"
#include "config.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>

namespace water_level
{

typedef struct{
	std::vector<unsigned int> coord;
} WaterLevelLine;

class WaterLevelDetection final
{
public:
	explicit WaterLevelDetection(SharedRef<Config>& config, int device = 0);
	~WaterLevelDetection();

	void detect(cv::Mat &curr_img, int &res);
	void draw(cv::Mat &img);
	void updateLine(int x0,int y0,int x1,int y1);
	void reset(){
		m_prev_detects.clear();
		m_prev_res.clear();
		m_mask = cv::Mat();
		m_res = 0;
		m_res_cnt = 0;
	}
private:
	void detectWithGPU(cv::cuda::GpuMat &curr_img, std::vector<cv::Vec4i> &lines);
	void detectWithCPU(cv::Mat &curr_img, std::vector<cv::Vec4i> &lines);
	void removeUnrelatedLines(std::vector<cv::Vec4i>& detected_lines);
	bool isLower(std::vector<cv::Vec4i>& lines,std::vector<unsigned int>& thres);
	void compareWithPrevDetects(const cv::Vec4i& c,bool & res,float& sum);

private:
	std::vector<cv::Vec4i> m_prev_detects;
	std::vector<float> m_prev_res;
	cv::Ptr<cv::cuda::HoughSegmentDetector> m_gpu_detector;
	cv::Ptr<cv::cuda::CannyEdgeDetector> m_canny;
	cv::Ptr<cv::cuda::CLAHE> clahe = nullptr;
	WaterLevelLine *m_line = nullptr;
	SharedRef<Config> m_config = nullptr;
	cv::cuda::Stream m_stream;
	cv::Mat m_mask;
	int m_single_cnt = 0;
	int m_multi_cnt = 0;
	int m_res = 0;
	int m_res_cnt = 0;
};
}