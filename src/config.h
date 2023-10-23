#pragma once
#include <string>
#include <vector>
namespace water_level
{
/**
 * @brief config class for deployment. should not be derived.
 */
class Config final
{
public:
	Config(int argc, char** argv, const std::string& file){
		LoadConfigFile(argc,argv,file);
	};
	/**
	 * @brief loading the config yaml file, default folder is ./config/infer_cfg.yaml
	 * @param argc terminal arg number.
	 * @param argv terminal arg values.
	 * @param file config file full path.
	 * @note priority: 1 Terminal; 2 Config file; 3 Compilation settings
	 */
	void LoadConfigFile(int argc, char **argv, const std::string &file);
public:
	std::string VIDEO_FILE = "";
	std::vector<unsigned int> LEVEL_LINE = {500,480,800,575};
	unsigned int LEVEL_COORD_Y = 300;
	float LEVEL_ANGLE_THRESHOLD = 15.0f;
	unsigned int LEVEL_LINE_DIFF = 5;
	bool ENABLE_MOVING_AVERAGE = true;
	unsigned int MOVING_LEN = 10;
	bool USE_GPU = true;
	float HOUGH_RHO = 0.5f;
	float HOUGH_THETA = 1.0f;
	int HOUGH_MIN_LEN = 50;
	int HOUGH_MAX_LINE_GAP = 5;
	int HOUGH_MAX_LINES = 50;
	double CANNY_LOW_THRES = 2.0;
	double CANNY_HIGH_THRES = 100.0;
	int CANNY_APPERTURE = 3;
	bool CANNY_L2 = false;
	std::string POST_TEXT = "Water Level Warning";
	int DRAW_LINE_WIDTH = 1.5f;
	std::vector<unsigned char> DRAW_LINE_COLOR = {255,0,0};
	std::vector<unsigned char> DRAW_LINE_COLOR_THRES = {0,0,255};
	float TEXT_LINE_WIDTH = 1.2f;
	std::vector<unsigned char> TEXT_LINE_COLOR = {255,0,0};
	double TEXT_FONT_SIZE = 2.0;
	unsigned int TEXT_OFFSET_X = 400;
	unsigned int TEXT_OFFSET_Y = 50;
};
}
