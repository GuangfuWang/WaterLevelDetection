
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
	bool init  = false;
	cvModel *model = nullptr;
	while (cap.isOpened()) {
		cap.read(img);
		if(img.rows==0||img.cols==0)break;
		if(!init){
			model = Allocate_Algorithm(img,0,0);
			SetPara_Algorithm(model,0);
			UpdateParams_Algorithm(model);
			init = true;
		}
		Process_Algorithm(model,img);
		vw.write(img.clone());
	}
	cap.release();
	vw.release();
	Destroy_Algorithm(model);

	return 0;
}