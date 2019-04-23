#include "faceRecognition.h"
#include <qDebug.h>


//FaceRecognition::FaceRecognition()
//{
//}

FaceRecognition::FaceRecognition(bool gpuType)
{
	//加载  预测模型
	this->gpu_context_type = gpuType;
	std::vector<index_t> input_dimensions = getShapeDimensions(input_shape);
	input_dimensions.insert(input_dimensions.begin(), 1);
	Shape input_data_shape(input_dimensions);
	//predict = new Predictor(model_file_json, model_file_params, input_data_shape, gpu_context_type);
	predict = new Predictor(model_file_json, model_file_params, input_data_shape, gpu_context_type);
}
FaceRecognition::~FaceRecognition()
{
}
float FaceRecognition::caculateSim(std::string feature_src, std::string feature_det)
{
	float sim = 0;
	float L1 = 0;
	float L2 = 0;
	//存放到vector中
	std::vector<float> src, det;
	int i = 0;
	std::string s1, s2;
	while (i < feature_src.size())
	{
		if (feature_src[i] != ',')
			s1.push_back(feature_src[i]);
		else
		{
			src.push_back(atof(s1.c_str()));
			s1 = "";
		}
		i++;
	}
	i = 0;
	while (i < feature_det.size())
	{
		if (feature_det[i] != ',')
			s2.push_back(feature_det[i]);
		else
		{
			det.push_back(atof(s2.c_str()));
			s2 = "";
		}
		i++;
	}

	//DEBUG
#ifdef DEBUG_PR

	for (int i = 0; i < 5; i++)
	{
		qDebug() << "src:" << i << "  " << src[i];
	}
	for (int i = 0; i < 5; i++)
	{
		qDebug() << "det:" << i << "  " << det[i];
	}
#endif // DEBUG
	//calc
	for (int i = 0; i < src.size(); i++)
	{
		L1 += (src[i] * src[i]);
		L2 += (det[i] * det[i]);
		sim += (src[i] * det[i]);
	}
	L1 = sqrt(L1);
	L2 = sqrt(L2);
	sim /= (L1 * L2);

	return sim;
}

NDArray FaceRecognition::makeFeature(cv::Mat mat) {
	// Run the forward pass to predict the image.
	NDArray feature;
	try {
		feature = predict->PredictImage(mat);
	}
	catch (std::runtime_error &error) {
		LG << "Execution failed with ERROR: " << error.what();
	}
	catch (...) {
		/*
		* If underlying MXNet code has thrown an exception the error message is
		* accessible through MXGetLastError() function.
		*/
		LG << "Execution failed with following MXNet error";
		LG << MXGetLastError();
	}
	// GPU模式
	if (this->gpu_context_type == true) {
		NDArray cpu_array(feature.GetShape(), Context::cpu());
		feature.WaitToRead();
		feature.CopyTo(&cpu_array);
		return cpu_array;
	}
	// CPU模式
	return feature;
}
