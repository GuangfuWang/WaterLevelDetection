#include "model.h"
#include "detection.h"
#include "config.h"
#include <string>
#include <iostream>
#include <cuda_runtime_api.h>

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
	if(Util::checkFileExist("./water_level.yaml"))
		file = "./water_level.yaml";
	else if(Util::checkFileExist("../config/water_level.yaml")){
		file = "../config/water_level.yaml";
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
	cudaSetDevice(gpuID);
	return ptr;
}
void SetPara_Algorithm(cvModel *pModel,int algID){

}
void UpdateParams_Algorithm(cvModel *pModel){
	if(pModel->p[1].x<=0||pModel->p[1].y<=0)return;
	auto model = reinterpret_cast<InferModel *>(pModel->iModel);
	model->mDeploy->updateLine(pModel->p[0].x,
							   pModel->p[0].y,
							   pModel->p[1].x,
							   pModel->p[1].y);
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
