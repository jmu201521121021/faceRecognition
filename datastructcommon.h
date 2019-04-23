/*  存放数据结构  */

#ifndef __DATASTRUCTCOMMON_H__
#define __DATASTRUCTCOMMON_H__

#include <opencv2/opencv.hpp>
#include "yolo.h"
#include "mxnet_inference.h"
#include "common.h"

/*!< 数据标注结构体 - 人脸 */
//FaceStructParm   管理一张图片
//FaceStructParm   管理一张图片的每一张人脸

struct FaceStructParam {

	cv::Mat image;											/*!< 原始图片 */
	cv::Mat detImage;                                       /*!< 检测之后的图片（画框） */
	// std::vector<bbox_t> vbox;                               /*!< 人脸位置              */
	// cv::Mat tailorFace;                                     //存放裁剪后的人脸Mat
	// cv::Mat alignFace;                                      //存放对齐的人脸图片
	std::vector<cv::Point2f> landmarkPoints;                  //存放标定点

	struct FaceParam {										/*!< 人脸参数 */
		bbox_t detectBox;                                   /*!< 人脸参数 - 人脸检测框 */
		std::vector<cv::Point2f> landmarks;					/*!< 人脸参数 - 人脸关键点 */
		int id = -1;										/*!< 人脸参数 - 人脸ID编号 */
		NDArray featureMap;                                 /*!< 人脸参数 - 人脸特征值 */
		float score;                                        /*!< 人脸参数 - 人脸相似度 */
		cv::Mat alignFace;                                   /*!< 人脸参数 - 对齐后人脸 */
	};
	std::vector<FaceParam> faceParams;						/*!< 所有人脸信息 */
	//NDArray featureMap;                                     //存放alignface的featureMap
	//float score;                                            //人脸和数据库中人脸的比较得分
};
//public:
//	void setImage(cv::Mat i);
//	void setDetImage(cv::Mat det);
//	void setVBox(std::vector<bbox_t> vb);
//	void setTailorFace(cv::Mat tailor);
//	void setAlignFace(cv::Mat align,int i);
//	void setLandmarkPoints(std::vector<cv::Point2f> landmark);
//	void setFeatureMap(NDArray tmp);
//	void setScore(float s);
//
//	cv::Mat getImage();
//	cv::Mat getDetImage();
//	std::vector<bbox_t> getVBox();
//	cv::Mat getTailorFace();
//	cv::Mat getAlignFace();
//	std::vector<cv::Point2f> getLandmarkPoints();
//	NDArray getFeatureMap();
//	float getScore();
//
//	void clearVBox();
//
//	//std::vector<cv::Mat> recFace;//存放裁剪后的人脸Mat
//	//std::vector<cv::Mat> alignmentFace;//存放对齐后的人脸Mat
//	//std::vector<std::vector<cv::Point> > landmarkPoints;  //存放人脸的5个标定点
//	// TODO 可根据实际情况增加
//};
//
//inline void FaceStructParam::setImage(cv::Mat i) {
//	i.copyTo(image);
//}
//inline void FaceStructParam::setDetImage(cv::Mat det) {
//	detImage = det.clone();
//}
//inline void FaceStructParam::setVBox(std::vector<bbox_t> vb) {
//	std::vector<bbox_t> tmp;
//	this->vbox = tmp;
//	if (vb.size() == 0) {
//		return;
//	}
//	else {
//		for (auto rect : vb) {
//			this->vbox.push_back(rect);
//		}
//	}
//}
//inline void FaceStructParam::setTailorFace(cv::Mat tailor) {
//	tailor.copyTo(tailorFace);
//}
//inline void FaceStructParam::setAlignFace(cv::Mat align) {
//	alignFace = align.clone();
//}
//inline void FaceStructParam::setLandmarkPoints(std::vector<cv::Point2f> landmark) {
//	std::vector<cv::Point2f> landmark5;
//	landmarkPoints = landmark5;
//	for (auto point : landmark) {
//		landmarkPoints.push_back(point);
//	}
//}
//
//inline cv::Mat FaceStructParam::getImage() {
//	return image;
//}
//inline cv::Mat FaceStructParam::getDetImage() {
//	return detImage;
//}
//inline std::vector<bbox_t> FaceStructParam::getVBox() {
//	return vbox;
//}
//inline cv::Mat FaceStructParam::getTailorFace() {
//	return tailorFace;
//}
//inline cv::Mat FaceStructParam::getAlignFace() {
//	return alignFace;
//}
//inline std::vector<cv::Point2f> FaceStructParam::getLandmarkPoints() {
//	return landmarkPoints;
//}
//
//inline void FaceStructParam::clearVBox() {
//	vbox.clear();
//}
//inline NDArray FaceStructParam::getFeatureMap() { return featureMap; }
//inline void FaceStructParam::setFeatureMap(NDArray tmp) { featureMap = tmp; }
//inline void FaceStructParam::setScore(float s) { score = s; }
//inline float FaceStructParam::getScore() { return score; }



/*   存放Edit中的信息，方便得发送到数据库中   */
struct InformationParam {
private:
	std::string nameInf;  //姓名
	std::string noInf;  //学号
	std::string collegeInf;  //学院
	std::string classInf;  //班级
	std::string stringFeature;  //变化成string之后的feature
	std::string pictureRoute; //图片路径

public:
	void setName(std::string name) { nameInf = name; };
	void setNo(std::string no) { noInf = no; };
	void setCollege(std::string college) { collegeInf = college; };
	void setClass(std::string cl) { classInf = cl; };
	void setPictureRoute(std::string route) { pictureRoute = route; };
	void setStringFeature(std::string sf) { stringFeature = sf; };

	std::string getName() { return nameInf; };
	std::string getNo() { return noInf; };
	std::string getCollege() { return collegeInf; };
	std::string getClass() { return classInf; };
	std::string getPictureRoute() { return pictureRoute; };
	std::string getStringFeature() { return stringFeature; };
};


inline std::string featureMapToString(NDArray featureMap)
{
	std::string stringFeature;
	float value;
	for (int i = 0; i < featureMap.Size(); i++)
	{
		value = featureMap.At(0, i);
		stringFeature = stringFeature + QS(QString::number(value, 'f', 4)) + ",";
	}
	return stringFeature;
}

#endif //__DATASTRUCTCOMMON_H__