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
	/**
	 * @brief loading the config yaml file, default folder is ./config/infer_cfg.yaml
	 * @param argc terminal arg number.
	 * @param argv terminal arg values.
	 * @param file config file full path.
	 * @note priority: 1 Terminal; 2 Config file; 3 Compilation settings
	 */
	static void LoadConfigFile(int argc, char **argv, const std::string &file);
public:
	static std::string VIDEO_FILE;///< video file path.
	static std::vector<unsigned int> LEVEL_LINE;
	static float LEVEL_ANGLE_THRESHOLD;
	static unsigned int LEVEL_COORD_Y;
	static unsigned int LEVEL_LINE_DIFF;
	static bool ENABLE_MOVING_AVERAGE;
	static unsigned int MOVING_LEN;
	static bool USE_GPU;
	static float HOUGH_RHO;
	static float HOUGH_THETA;
	static int HOUGH_MIN_LEN;
	static int HOUGH_MAX_LINE_GAP;
	static int HOUGH_MAX_LINES;
	static double CANNY_LOW_THRES;
	static double CANNY_HIGH_THRES;
	static int CANNY_APPERTURE;
	static bool CANNY_L2;
	static std::string POST_TEXT;
	static int DRAW_LINE_WIDTH;
	static std::vector<unsigned char> DRAW_LINE_COLOR;
	static std::vector<unsigned char> DRAW_LINE_COLOR_THRES;
	static float TEXT_LINE_WIDTH;
	static std::vector<unsigned char> TEXT_LINE_COLOR;
	static double TEXT_FONT_SIZE;
	static unsigned int TEXT_OFFSET_X;
	static unsigned int TEXT_OFFSET_Y;
};
}