#pragma once
#include <opencv2/opencv.hpp>
#include "datastructcommon.h"
#include "dbg.h"
#include "mxnet_inference.h"


class FaceRecognition
{
public:
	FaceRecognition(bool gpuType);
	/*FaceRecognition();*/
	~FaceRecognition();
	NDArray makeFeature(cv::Mat mat);
	//float caculateSim(NDArray feature_src, NDArray feature_det);
	float caculateSim(std::string src, std::string det);
	inline void setGpuContextType(bool type) {
		this->gpu_context_type = type;
	}
private:
	//Predictor *predict = NULL;
	Predictor *predict;
	//²ÎÊý
	std::string model_file_json = "../model/model-r50-am-lfw/model-symbol.json";
	std::string model_file_params = "../model/model-r50-am-lfw/model-0000.params";
	std::string input_shape = "3 112 112";
	bool gpu_context_type = true;

};

