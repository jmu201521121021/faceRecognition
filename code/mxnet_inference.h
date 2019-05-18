
#ifndef  MXNET_INFERENCE_H
#define  MXNET_INFERENCE_H
#ifndef LIB_API 
#define LIB_API __declspec(dllexport)
#endif

#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "mxnet-cpp/MxNetCpp.h"
#include <opencv2/opencv.hpp>

using namespace mxnet::cpp;

extern "C"   LIB_API LIB_API std::vector<index_t> getShapeDimensions(const std::string& hidden_units_string);
class Predictor {
public:
	LIB_API Predictor() {}
	LIB_API Predictor(const std::string& model_json_file,
		const std::string& model_params_file,
		const Shape& input_shape,
		bool gpu_context_type = false,
		const  std::string& synset_file = "",
		const  std::string& mean_image_file = "");
	LIB_API NDArray PredictImage(cv::Mat mat);
	LIB_API ~Predictor();

private:
	void LIB_API LoadModel(const std::string& model_json_file);
	void LIB_API LoadParameters(const std::string& model_parameters_file);

	// void LoadSynset(const std::string& synset_file);
	LIB_API NDArray LoadInputImage(cv::Mat mat);
	void LIB_API LoadMeanImageData();
	// void LoadDefaultMeanImageData();
	// void NormalizeInput(const std::string& mean_image_file);
	inline  bool LIB_API FileExists(const std::string& name) {
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}
	// NDArray mean_img;
	std::map<std::string, NDArray> args_map;
	std::map<std::string, NDArray> aux_map;
	// std::vector<std::string> output_labels;
	Symbol net;
	Executor *executor;
	Shape input_shape;
	NDArray mean_image_data;
	Context global_ctx = Context::cpu();
	std::string mean_image_file;
};

#endif // !MXNET_INFERENCE_H

