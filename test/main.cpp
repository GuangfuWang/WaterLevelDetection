
#include <thread>
#include "../src/config.h"
#include "../src/detection.h"
#include "../src/model.h"
using namespace water_level;

int NUM_THREADS = 1;

void process_void(const int& thread_id, const std::string& file){
	//prepare the input data.

	if(!Util::checkFileExist(file)){
		std::cerr<<"The file is not exist"<<std::endl;
		return;
	}
	auto in_path = std::filesystem::path(file);
	cv::VideoCapture cap(file);
	cv::VideoWriter vw;
	std::filesystem::path output_path = in_path.parent_path() / (in_path.stem().string() +
		std::to_string(thread_id)+ ".result.mp4");
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
	std::chrono::high_resolution_clock::time_point curr_time;
	while (cap.isOpened()) {
		cap.read(img);
		if(img.rows==0||img.cols==0)break;
		if(!init){
			model = Allocate_Algorithm(img,0,0);
			SetPara_Algorithm(model,0);
			UpdateParams_Algorithm(model);
			init = true;
		}

		curr_time =
			std::chrono::high_resolution_clock::now();
		Process_Algorithm(model,img);
		auto dur = std::chrono::high_resolution_clock::now() - curr_time;
		curr_time = std::chrono::high_resolution_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		std::cout<<"Thread: "<<std::this_thread::get_id()<<" Cpu: "<<sched_getcpu()<<" taken: "<<ms<<"ms"<<std::endl;

		vw.write(img.clone());
	}
	cap.release();
	vw.release();
	Destroy_Algorithm(model);
}

int main(int argc, char** argv){
	if(argc>1){
		NUM_THREADS = std::atoi(argv[1]);
	}
	std::vector<std::thread> threads(NUM_THREADS);
	std::vector<std::string> files(NUM_THREADS);
	for (int i = 0; i < NUM_THREADS; ++i) {
		files[i] = "/home/wgf/Downloads/datasets/water-level/waterlevel-new.mp4";
		threads[i] = std::thread(process_void,i,files[i]);
	}
	for (int i = 0; i < NUM_THREADS; ++i) {
		threads[i].join();
	}

	return 0;
}