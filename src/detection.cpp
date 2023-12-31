#include <driver_types.h>
#include "detection.h"
#include "config.h"

namespace water_level
{

inline cv::Vec4i getLen(const std::vector<cv::Vec4i>& dets){
	std::vector<float> lens(dets.size(),0.0f);
	for (int i = 0; i < dets.size(); ++i) {
		lens[i]=std::pow(dets[i][0]-dets[i][2],2)+std::pow(dets[i][1]-dets[i][3],2);
	}
	float max_value = 0.0f;
	int idx=0;
	for (int i = 0; i < dets.size(); ++i) {
		if(lens[i]>max_value){
			max_value = lens[i];
			idx = i;
		}
	}
	return dets[idx];
}

void WaterLevelDetection::detect(cv::Mat &curr_img, int &res)
{
	cv::Mat d_img;
	res = 0;
	if (m_config->USE_GPU) {
		std::vector<cv::Vec4i> res_lines;
		if (m_mask.cols < 1) {
			m_mask = cv::Mat::zeros(curr_img.rows / 2, curr_img.cols / 2, curr_img.type());
			int yl = m_line->coord[1] / 2 - 100 / 2;
			if (yl <= 100)yl = 100 / 2;
			cv::rectangle(m_mask, cv::Rect(m_line->coord[0] / 2 , 200,
										   m_line->coord[2] / 2 - m_line->coord[0] / 2,
										   550 / 2),
						  cv::Scalar(255, 255, 255), -1);
		}
		cv::resize(curr_img, curr_img, cv::Size(), 0.5, 0.5);
		curr_img.copyTo(d_img, m_mask);
		res_lines.reserve(10);
		cv::cuda::GpuMat curr;
		curr.upload(d_img,m_stream);
		detectWithGPU(curr, res_lines);
		removeUnrelatedLines(res_lines);
		cv::resize(curr_img, curr_img, cv::Size(), 2.0, 2.0);

		if (res_lines.empty()) {
			return;
		}
		else {
			m_single_cnt++;
			m_prev_detects.push_back(res_lines[0]);
		}
		if (m_single_cnt > m_config->MOVING_LEN) {
			if (!isLower(m_prev_detects, m_line->coord)) {
				m_res++;
			}//over the threshold line.
			if (m_prev_detects.size() > 2 * m_config->MOVING_LEN) {
				auto it = m_prev_detects.begin();
				for (int i = 0; i < m_config->MOVING_LEN/2; ++i) {
					m_prev_detects.erase(it);
					it++;
				}
			}
			m_single_cnt = 0;
			if (m_res > 5) {
				m_res_cnt = 10;
				m_res = 0;
			}
		}
		else res = 0;
	}
	else {
		std::vector<cv::Vec4i> res_lines;
		if (m_mask.cols < 1) {
			m_mask = cv::Mat::zeros(curr_img.rows / 2, curr_img.cols / 2, curr_img.type());
			int yl = m_line->coord[1] / 2 - 100 / 2;
			if (yl <= 100)yl = 100 / 2;
			cv::rectangle(m_mask, cv::Rect(m_line->coord[0] / 2 - 100 / 2, 200,
										   m_line->coord[2] / 2 - m_line->coord[0] / 2 + 200 / 2,
										   550 / 2),
						  cv::Scalar(255, 255, 255), -1);
		}
		cv::resize(curr_img, curr_img, cv::Size(), 0.5, 0.5);
		curr_img.copyTo(d_img, m_mask);
		detectWithCPU(d_img, res_lines);
		removeUnrelatedLines(res_lines);
		cv::resize(curr_img, curr_img, cv::Size(), 2.0, 2.0);
		if (res_lines.empty()) {
			return;
		}
		else {
			m_single_cnt++;
			m_prev_detects.push_back(res_lines[0]);
		}
		if (m_single_cnt > m_config->MOVING_LEN) {
			if (!isLower(m_prev_detects, m_line->coord)) {
				m_res++;
			}//over the threshold line.
			if (m_prev_detects.size() > 2 * m_config->MOVING_LEN) {
				auto it = m_prev_detects.begin();
				for (int i = 0; i < m_config->MOVING_LEN/2; ++i) {
					m_prev_detects.erase(it);
					it++;
				}
			}
			m_single_cnt = 0;
			if (m_res > 5) {
				m_res_cnt = 10;
				m_res = 0;
			}
		}
		else res = 0;
	}
	if (m_res_cnt)res = 1;
}
void WaterLevelDetection::detectWithGPU(cv::cuda::GpuMat &curr_img,
										std::vector<cv::Vec4i> &lines)
{
	//preprocess the color image.
	cv::cuda::GpuMat hough_lines;
	cv::cuda::GpuMat edges;
	cv::cuda::GpuMat img;
	cv::Mat final_line;
	cv::cuda::cvtColor(curr_img, img, cv::COLOR_BGR2GRAY,0,m_stream);
	cv::cuda::equalizeHist(img, img,m_stream);
	if(!clahe){
		clahe = cv::cuda::createCLAHE();
		clahe->setClipLimit(5);
		clahe->setTilesGridSize(cv::Size(5, 5));
	}
	clahe->apply(img, img,m_stream);
	//detect edges.
	m_canny->detect(img, edges,m_stream);
	//detect lines.
	m_gpu_detector->detect(edges, hough_lines,m_stream);
	//get result.
//	lines.resize(1);
	if (!hough_lines.empty()) {
		lines.resize(hough_lines.cols);
//		cv::Mat::setDefaultAllocator(cv::cuda::HostMem::getAllocator (cv::cuda::HostMem::AllocType::PAGE_LOCKED));
		cv::Mat h_lines(1, hough_lines.cols, CV_32SC4, &lines[0]);
//		cv::cuda::registerPageLocked(h_lines);
		hough_lines.download(h_lines,m_stream);
		m_stream.waitForCompletion();
	}
}
void WaterLevelDetection::detectWithCPU(cv::Mat &curr_img,
										std::vector<cv::Vec4i> &lines)
{
	cv::HoughLinesP(curr_img, lines, 1, CV_PI / 180, 50, 60, 5);
}
void WaterLevelDetection::draw(cv::Mat &img)
{
	if (!m_prev_detects.empty()) {
		auto l = m_prev_detects[m_prev_detects.size() - 1];
		cv::line(img, cv::Point(2 * l[0], 2 * l[1]), cv::Point(2 * l[2], 2 * l[3]),
				 cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
		if (m_res_cnt) {
			m_res_cnt--;
		}

	}
	auto t = m_line->coord;
	cv::line(img, cv::Point(t[0], t[1]), cv::Point(t[2], t[3]),
			 cv::Scalar(m_config->DRAW_LINE_COLOR_THRES[2],
						m_config->DRAW_LINE_COLOR_THRES[1],
						m_config->DRAW_LINE_COLOR_THRES[0]),
			 m_config->DRAW_LINE_WIDTH);
}
WaterLevelDetection::WaterLevelDetection(SharedRef<Config> &config, int device)
{
	m_config = config;
	if (m_config->USE_GPU) {
		cv::cuda::setDevice(device);
		m_gpu_detector = cv::cuda::createHoughSegmentDetector(m_config->HOUGH_RHO,
															  m_config->HOUGH_THETA,
															  m_config->HOUGH_MIN_LEN,
															  m_config->HOUGH_MAX_LINE_GAP,
															  m_config->HOUGH_MAX_LINES);
		m_canny = cv::cuda::createCannyEdgeDetector(m_config->CANNY_LOW_THRES,
													m_config->CANNY_HIGH_THRES,
													m_config->CANNY_APPERTURE,
													m_config->CANNY_L2);
	}
	m_line = new WaterLevelLine();
	m_line->coord.resize(4);
	m_line->coord = m_config->LEVEL_LINE;
	m_stream = cv::cuda::Stream(cudaStreamNonBlocking);

}
void WaterLevelDetection::removeUnrelatedLines(std::vector<cv::Vec4i> &detected_lines)
{
	auto comp = m_line->coord;
	std::vector<cv::Vec4i> ret;
	float min_t = 180;
	for (auto &i : detected_lines) {
		if (2 * i[0] > comp[2] && 2 * i[2] > comp[2])continue;
		if (2 * i[2] < comp[0] && 2 * i[0] < comp[0])continue;
		float temp = Util::getAngle(2 * i[0], 2 * i[1], 2 * i[2], 2 * i[3],
									comp[0], comp[1], comp[2], comp[3]);
		if (min_t > temp && temp < 0.1) {
			min_t = temp;
			ret.push_back(i);
		}
	}
	if (ret.size()) {
		detected_lines.clear();
		if (m_prev_detects.size() > m_config->MOVING_LEN) {
			auto curr_line = getLen(ret);
			bool flag = true;
			float mean = 0.0f;
			compareWithPrevDetects(curr_line,flag,mean);
//			if(ret.size()==1){
				if (flag) {
					detected_lines.push_back(curr_line);
				}
//			}
//			else if(ret.size()==2){

//			}

		}else detected_lines.push_back(ret[ret.size() - 1]);
	}
	else detected_lines.clear();
}

bool WaterLevelDetection::isLower(std::vector<cv::Vec4i> &lines,
								  std::vector<unsigned int> &thres)
{
	int cnt = 0;
	for (int i = lines.size() - 1; i > lines.size() - 1 - m_config->MOVING_LEN && i >= 0; i--) {
		if (lines[i][0] > lines[i][2]) {
			std::swap(lines[i][1], lines[i][3]);
			std::swap(lines[i][0], lines[i][2]);
		}
		auto l = lines[i];
		int x = l[0] + l[2];
		int y = l[1] + l[3];
		double tmpy = (thres[3] - thres[1]) / (thres[2] - thres[0]) * (x - thres[0]) + thres[1];
		if (tmpy < y) {
			cnt++;
		}
	}
	if (cnt > m_config->MOVING_LEN * 0.6f) {
		return true;
	}
	return false;
}

WaterLevelDetection::~WaterLevelDetection()
{
	if (m_line) {
		delete m_line;
		m_line = nullptr;
	}
}
void WaterLevelDetection::updateLine(int x0, int y0, int x1, int y1)
{
	if (m_line) {
		delete m_line;
		m_line = nullptr;
	}
	m_line = new WaterLevelLine();
	m_line->coord.resize(4);
	m_line->coord[0] = x0;
	m_line->coord[1] = y0;
	m_line->coord[2] = x1;
	m_line->coord[3] = y1;
}
void WaterLevelDetection::compareWithPrevDetects(const cv::Vec4i &c,
												 bool &res,float& sum)
{
	if(m_prev_detects.empty())return;
	res = true;
	sum = 0.0f;
	int cnt = 0;
	for (int i = m_prev_detects.size() - 1; i >= 0 && i > m_prev_detects.size() - 3; i--) {
		auto delta_x = m_prev_detects[i][0] - m_prev_detects[i][2];
		if (std::abs(delta_x) > 0.001f) {
			float k = m_prev_detects[i][1] - m_prev_detects[i][3];
			k /= delta_x;
			auto b = m_prev_detects[i][1] - k * m_prev_detects[i][0];
			auto sk = std::sqrt(1 + k * k);
			auto d1 = std::abs(b - c[1] + k * c[0]) / sk;
			auto d2 = std::abs(b - c[3] + k * c[2]) / sk;
			if (d1 + d2 > 40)res = false;//
			cnt++;
			sum = sum+d1+d2;
		}
		else continue;
	}
	sum/=cnt;

}
}
