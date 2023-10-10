#include "model.h"
#include "detection.h"
#include "config.h"
#include <string>
#include <iostream>

namespace water_level{

class InferModel {
public:
	InferModel() {
		mDeploy = createSharedRef<WaterLevelDetection>();
	}
public:
	SharedRef<WaterLevelDetection> mDeploy;
//	std::vector<int> res;
//	std::vector<cv::Mat> mSampled;
};

static void *GenModel() {
	InferModel *model = new InferModel();
	return reinterpret_cast<void *>(model);
}

cvModel* Allocate_Algorithm(cv::Mat &input_frame, int algID, int gpuID){
	std::string file;
	if(Util::checkFileExist("./config.yaml"))
		file = "./config.yaml";
	else if(Util::checkFileExist("../config/config.yaml")){
		file = "../config/config.yaml";
	}else{
		std::cerr<<"Cannot find YAML file!"<<std::endl;
	}
	Config::LoadConfigFile(1, nullptr, file);
	auto *ptr = new cvModel();
	ptr->FrameNum = 0;
	ptr->Frameinterval = 0;
	ptr->countNum = 0;
	ptr->width = input_frame.cols;
	ptr->height = input_frame.rows;
	ptr->iModel = GenModel();
	return ptr;
}
void SetPara_Algorithm(cvModel *pModel,int algID){

}
void UpdateParams_Algorithm(cvModel *pModel){

}
void Process_Algorithm(cvModel *pModel, cv::Mat &input_frame){
	int curr_res = 0;
	auto model = reinterpret_cast<InferModel *>(pModel->iModel);
	model->mDeploy->detect(input_frame, curr_res);
	model->mDeploy->draw(input_frame);
}
void Destroy_Algorithm(cvModel *pModel){
	if (pModel->iModel){
		auto model = reinterpret_cast<InferModel *>(pModel->iModel);
		delete model;
		model = nullptr;
	}
	if (pModel) {
		delete pModel;
		pModel = nullptr;
	}
	std::cout<<"Deconstruct the model done..."<<std::endl;
}
}
