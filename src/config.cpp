//
// Created by wgf on 23-10-8.
//

#include <iostream>
#include "config.h"
#include "util.h"
#include <yaml-cpp/yaml.h>

namespace water_level{

std::string Config::VIDEO_FILE = "";
std::vector<unsigned int> Config::LEVEL_LINE = {500,480,800,575};
unsigned int Config::LEVEL_COORD_Y = 300;
float Config::LEVEL_ANGLE_THRESHOLD = 15.0f;
unsigned int Config::LEVEL_LINE_DIFF = 5;
bool Config::ENABLE_MOVING_AVERAGE = true;
unsigned int Config::MOVING_LEN = 10;
bool Config::USE_GPU = true;
float Config::HOUGH_RHO = 0.5f;
float Config::HOUGH_THETA = 1.0f;
int Config::HOUGH_MIN_LEN = 50;
int Config::HOUGH_MAX_LINE_GAP = 5;
int Config::HOUGH_MAX_LINES = 50;
double Config::CANNY_LOW_THRES = 2.0;
double Config::CANNY_HIGH_THRES = 100.0;
int Config::CANNY_APPERTURE = 3;
bool Config::CANNY_L2 = false;
std::string Config::POST_TEXT = "Water Level Warning";
int Config::DRAW_LINE_WIDTH = 1.5f;
std::vector<unsigned char> Config::DRAW_LINE_COLOR = {255,0,0};
std::vector<unsigned char> Config::DRAW_LINE_COLOR_THRES = {0,0,255};
float Config::TEXT_LINE_WIDTH = 1.2f;
std::vector<unsigned char> Config::TEXT_LINE_COLOR = {255,0,0};
double Config::TEXT_FONT_SIZE = 2.0;
unsigned int Config::TEXT_OFFSET_X = 400;
unsigned int Config::TEXT_OFFSET_Y = 50;

void Config::LoadConfigFile(int argc, char **argv, const std::string &file) {

	if (!Util::checkFileExist(file)) {
		std::cerr << "Config file for water level non exists! Aborting..." << std::endl;
	}

	YAML::Node config;
	config = YAML::LoadFile(file);

	if (config["DATA"].IsDefined()) {
		auto model_node = config["DATA"];
		if (model_node["VIDEO_FILE"].IsDefined()) {
			Config::VIDEO_FILE = model_node["VIDEO_FILE"].as<std::string>();
		}
	} else {
		std::cerr << "Please set VIDEO FILE, " << std::endl;
	}

	if (config["METHOD"].IsDefined()) {
		auto model_node = config["METHOD"];
		if (model_node["LEVEL_LINE"].IsDefined()) {
			Config::LEVEL_LINE = model_node["LEVEL_LINE"].as<std::vector<unsigned int>>();
		}
		if (model_node["LEVEL_COORD_Y"].IsDefined()) {
			Config::LEVEL_COORD_Y = model_node["LEVEL_COORD_Y"].as<unsigned int>();
		}
		if (model_node["LEVEL_LINE_DIFF"].IsDefined()) {
			Config::LEVEL_LINE_DIFF = model_node["LEVEL_LINE_DIFF"].as<unsigned int>();
		}
		if (model_node["ENABLE_MOVING_AVERAGE"].IsDefined()) {
			Config::ENABLE_MOVING_AVERAGE = model_node["ENABLE_MOVING_AVERAGE"].as<bool>();
		}
		if (model_node["MOVING_LEN"].IsDefined()) {
			Config::MOVING_LEN = model_node["MOVING_LEN"].as<unsigned int>();
		}
		if (model_node["USE_GPU"].IsDefined()) {
			Config::USE_GPU = model_node["USE_GPU"].as<bool>();
		}
		if (model_node["HOUGH_RHO"].IsDefined()) {
			Config::HOUGH_RHO = model_node["HOUGH_RHO"].as<float>();
		}
		if (model_node["HOUGH_THETA"].IsDefined()) {
			Config::HOUGH_THETA = model_node["HOUGH_THETA"].as<float>();
		}
		if (model_node["HOUGH_MIN_LEN"].IsDefined()) {
			Config::HOUGH_MIN_LEN = model_node["HOUGH_MIN_LEN"].as<int>();
		}
		if (model_node["HOUGH_MAX_LINE_GAP"].IsDefined()) {
			Config::HOUGH_MAX_LINE_GAP = model_node["HOUGH_MAX_LINE_GAP"].as<int>();
		}
		if (model_node["HOUGH_MAX_LINES"].IsDefined()) {
			Config::HOUGH_MAX_LINES = model_node["HOUGH_MAX_LINES"].as<int>();
		}
		if (model_node["CANNY_LOW_THRES"].IsDefined()) {
			Config::CANNY_LOW_THRES = model_node["CANNY_LOW_THRES"].as<double>();
		}
		if (model_node["CANNY_HIGH_THRES"].IsDefined()) {
			Config::CANNY_HIGH_THRES = model_node["CANNY_HIGH_THRES"].as<double>();
		}
		if (model_node["CANNY_APPERTURE"].IsDefined()) {
			Config::CANNY_APPERTURE = model_node["CANNY_APPERTURE"].as<int>();
		}
		if (model_node["CANNY_L2"].IsDefined()) {
			Config::CANNY_L2 = model_node["CANNY_L2"].as<bool>();
		}
	} else {
		std::cerr << "Please set Hough Params, " << std::endl;
	}

	if (config["POST_PROCESS"].IsDefined()) {
		auto model_node = config["POST_PROCESS"];
		if (model_node["POST_TEXT"].IsDefined()) {
			Config::POST_TEXT = model_node["POST_TEXT"].as<std::string>();
		}
		if (model_node["DRAW_LINE_WIDTH"].IsDefined()) {
			Config::DRAW_LINE_WIDTH = model_node["DRAW_LINE_WIDTH"].as<int>();
		}
		if (model_node["DRAW_LINE_COLOR"].IsDefined()) {
			Config::DRAW_LINE_COLOR = model_node["DRAW_LINE_COLOR"].as<std::vector<unsigned char>>();
		}
		if (model_node["DRAW_LINE_COLOR_THRES"].IsDefined()) {
			Config::DRAW_LINE_COLOR_THRES = model_node["DRAW_LINE_COLOR_THRES"].as<std::vector<unsigned char>>();
		}
		if (model_node["TEXT_LINE_WIDTH"].IsDefined()) {
			Config::TEXT_LINE_WIDTH = model_node["TEXT_LINE_WIDTH"].as<float>();
		}
		if (model_node["TEXT_FONT_SIZE"].IsDefined()) {
			Config::TEXT_FONT_SIZE = model_node["TEXT_FONT_SIZE"].as<double>();
		}
		if (model_node["TEXT_LINE_COLOR"].IsDefined()) {
			Config::TEXT_LINE_COLOR = model_node["TEXT_LINE_COLOR"].as<std::vector<unsigned char>>();
		}
		if (model_node["TEXT_OFFSET_X"].IsDefined()) {
			Config::TEXT_OFFSET_X = model_node["TEXT_OFFSET_X"].as<unsigned int>();
		}
		if (model_node["TEXT_OFFSET_Y"].IsDefined()) {
			Config::TEXT_OFFSET_Y = model_node["TEXT_OFFSET_Y"].as<unsigned int>();
		}
	} else {
		std::cerr << "Please set POST_PROCESS, " << std::endl;
	}

}
}
