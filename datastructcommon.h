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