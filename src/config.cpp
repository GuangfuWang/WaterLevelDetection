//
// Created by wgf on 23-10-8.
//

#include <iostream>
#include "config.h"
#include "util.h"
#include <yaml-cpp/yaml.h>

namespace water_level{



void Config::LoadConfigFile(int argc, char **argv, const std::string &file) {

	if (!Util::checkFileExist(file)) {
		std::cerr << "Config file for water level non exists! Aborting..." << std::endl;
	}

	YAML::Node config;
	config = YAML::LoadFile(file);

	if (config["DATA"].IsDefined()) {
		auto model_node = config["DATA"];
		if (model_node["VIDEO_FILE"].IsDefined()) {
			VIDEO_FILE = model_node["VIDEO_FILE"].as<std::string>();
		}
	} else {
		std::cerr << "Please set VIDEO FILE, " << std::endl;
	}

	if (config["METHOD"].IsDefined()) {
		auto model_node = config["METHOD"];
		if (model_node["LEVEL_LINE"].IsDefined()) {
			LEVEL_LINE = model_node["LEVEL_LINE"].as<std::vector<unsigned int>>();
		}
		if (model_node["LEVEL_COORD_Y"].IsDefined()) {
			LEVEL_COORD_Y = model_node["LEVEL_COORD_Y"].as<unsigned int>();
		}
		if (model_node["LEVEL_LINE_DIFF"].IsDefined()) {
			LEVEL_LINE_DIFF = model_node["LEVEL_LINE_DIFF"].as<unsigned int>();
		}
		if (model_node["ENABLE_MOVING_AVERAGE"].IsDefined()) {
			ENABLE_MOVING_AVERAGE = model_node["ENABLE_MOVING_AVERAGE"].as<bool>();
		}
		if (model_node["MOVING_LEN"].IsDefined()) {
			MOVING_LEN = model_node["MOVING_LEN"].as<unsigned int>();
		}
		if (model_node["USE_GPU"].IsDefined()) {
			USE_GPU = model_node["USE_GPU"].as<bool>();
		}
		if (model_node["HOUGH_RHO"].IsDefined()) {
			HOUGH_RHO = model_node["HOUGH_RHO"].as<float>();
		}
		if (model_node["HOUGH_THETA"].IsDefined()) {
			HOUGH_THETA = model_node["HOUGH_THETA"].as<float>();
		}
		if (model_node["HOUGH_MIN_LEN"].IsDefined()) {
			HOUGH_MIN_LEN = model_node["HOUGH_MIN_LEN"].as<int>();
		}
		if (model_node["HOUGH_MAX_LINE_GAP"].IsDefined()) {
			HOUGH_MAX_LINE_GAP = model_node["HOUGH_MAX_LINE_GAP"].as<int>();
		}
		if (model_node["HOUGH_MAX_LINES"].IsDefined()) {
			HOUGH_MAX_LINES = model_node["HOUGH_MAX_LINES"].as<int>();
		}
		if (model_node["CANNY_LOW_THRES"].IsDefined()) {
			CANNY_LOW_THRES = model_node["CANNY_LOW_THRES"].as<double>();
		}
		if (model_node["CANNY_HIGH_THRES"].IsDefined()) {
			CANNY_HIGH_THRES = model_node["CANNY_HIGH_THRES"].as<double>();
		}
		if (model_node["CANNY_APPERTURE"].IsDefined()) {
			CANNY_APPERTURE = model_node["CANNY_APPERTURE"].as<int>();
		}
		if (model_node["CANNY_L2"].IsDefined()) {
			CANNY_L2 = model_node["CANNY_L2"].as<bool>();
		}
	} else {
		std::cerr << "Please set Hough Params, " << std::endl;
	}

	if (config["POST_PROCESS"].IsDefined()) {
		auto model_node = config["POST_PROCESS"];
		if (model_node["POST_TEXT"].IsDefined()) {
			POST_TEXT = model_node["POST_TEXT"].as<std::string>();
		}
		if (model_node["DRAW_LINE_WIDTH"].IsDefined()) {
			DRAW_LINE_WIDTH = model_node["DRAW_LINE_WIDTH"].as<int>();
		}
		if (model_node["DRAW_LINE_COLOR"].IsDefined()) {
			DRAW_LINE_COLOR = model_node["DRAW_LINE_COLOR"].as<std::vector<unsigned char>>();
		}
		if (model_node["DRAW_LINE_COLOR_THRES"].IsDefined()) {
			DRAW_LINE_COLOR_THRES = model_node["DRAW_LINE_COLOR_THRES"].as<std::vector<unsigned char>>();
		}
		if (model_node["TEXT_LINE_WIDTH"].IsDefined()) {
			TEXT_LINE_WIDTH = model_node["TEXT_LINE_WIDTH"].as<float>();
		}
		if (model_node["TEXT_FONT_SIZE"].IsDefined()) {
			TEXT_FONT_SIZE = model_node["TEXT_FONT_SIZE"].as<double>();
		}
		if (model_node["TEXT_LINE_COLOR"].IsDefined()) {
			TEXT_LINE_COLOR = model_node["TEXT_LINE_COLOR"].as<std::vector<unsigned char>>();
		}
		if (model_node["TEXT_OFFSET_X"].IsDefined()) {
			TEXT_OFFSET_X = model_node["TEXT_OFFSET_X"].as<unsigned int>();
		}
		if (model_node["TEXT_OFFSET_Y"].IsDefined()) {
			TEXT_OFFSET_Y = model_node["TEXT_OFFSET_Y"].as<unsigned int>();
		}
	} else {
		std::cerr << "Please set POST_PROCESS, " << std::endl;
	}

}
}
