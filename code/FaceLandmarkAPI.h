#ifndef __FACELANDMARKAPI_H__
#define __FACELANDMARKAPI_H__

#include <opencv2/opencv.hpp>

#define FLAPI __declspec(dllexport)

class FaceLandmarkBase;

class FLAPI FaceLandmarkAPI
{
public:
	FaceLandmarkAPI();
	~FaceLandmarkAPI();

	void loadModel(std::string &faceLandmarksPath);

	/**
	* @brief landmarkFaces                 人脸标定重载模块
	* @param faceImage                     人脸图片
	* @param faceLandmarks                 人脸特征点
	* @return null
	*
	* @note
	*/
	void landmarkFaces(const cv::Mat &faceImage, std::vector<cv::Point> &faceLandmarks);

private:
	FaceLandmarkBase *mFaceLandmarkBase;

};


#endif // __FACELANDMARKAPI_H__