
#include "../src/config.h"
#include "../src/detection.h"
#include "../src/model.h"
using namespace water_level;

int main(int argc, char** argv){
	Config::LoadConfigFile(argc, argv, "../config/water_level.yaml");
//	SharedRef<WaterLevelDetection> wd = createSharedRef<WaterLevelDetection>();
	//prepare the input data.
	auto in_path = std::filesystem::path(Config::VIDEO_FILE);
	cv::VideoCapture cap(in_path);
	cv::VideoWriter vw;
	std::filesystem::path output_path = in_path.parent_path() / (in_path.stem().string() + ".result.mp4");
	vw.open(output_path,
			cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
			cap.get(cv::CAP_PROP_FPS),
			cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH),
					 cap.get(cv::CAP_PROP_FRAME_HEIGHT)));
	cv::Mat img;

	std::vector<cv::Mat> whole;
	whole.reserve(cap.get(cv::CAP_PROP_FRAME_COUNT));
	while (cap.read(img)) {
		whole.emplace_back(img.clone());
	}
	int res = 0;
	auto model = Allocate_Algorithm(whole[0],0,0);
	SetPara_Algorithm(model,0);
	UpdateParams_Algorithm(model);
	std::vector<cv::Mat> infer;
	for (auto each: whole) {
		Process_Algorithm(model,each);
		infer.push_back(each.clone());
	}
	Destroy_Algorithm(model);
	for (auto &f: infer) {
		vw.write(f);
	}

	return 0;
}