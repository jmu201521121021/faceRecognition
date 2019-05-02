/*进一步封装 yolo  检测器*/
/*   原始图片   ->    人脸框bbox_t    */

#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include "yolo.h"
#include "alignment\FaceDetector2.h"
#include "dbg.h"
#include <vector>
#include <string>


class FaceDetector {
public:
	FaceDetector(std::string names_file, std::string cfg_file, std::string weights_file);
	~FaceDetector();

	/*  检测         */
	cv::Mat drawFaceBoxes(cv::Mat);//返回画好框的图片
	std::vector<bbox_t> faceDetectBoxes(cv::Mat); //返回bbox_t
	
private:
	/*  检测器   */
	Detector *detector;
	seeta::FaceDetector2 *FD;
	/*  初始化检测器  */
	void initDetector();

	/*  和检测器相关的函数    */
	std::vector<std::string> objects_names_from_file(std::string);

	/*   参数   */
	std::vector<std::string> obj_names;
	std::string names_file = "../data/face.names";
	std::string cfg_file = "../cfg/yolov3-spp-focal_loss-test.cfg";
	std::string weights_file = "../model/yolov3-spp-focal_loss_12000.weights";
	float theta = 0.7;
	int detectType = 0; // 0 mean yolo,1 mean seeta
};
#endif

