#include "faceAlignment.h"
#include "qdebug.h"
FaceAlignment::FaceAlignment(int alignType) {
	// seetaface2 
	if (alignType == 0) {
		seetaLandmark = new seeta::PointDetector2(seetaModel);
	}// dlib
	else if(alignType == 1){
		mFaceLandmarkAPI = new FaceLandmarkAPI();
		mFaceLandmarkAPI->loadModel(landmarkModelFile);
	}
	else {
		// TODO
	}
}
FaceAlignment::FaceAlignment()
{
}


FaceAlignment::~FaceAlignment()
{
	if (mFaceLandmarkAPI)
	{
		delete mFaceLandmarkAPI;
		mFaceLandmarkAPI = NULL;
	}
	if (seetaLandmark) {
		delete seetaLandmark;
		seetaLandmark = NULL;

	}
}
/**
返回相对坐标，相对于人脸左上角
*/
void FaceAlignment::landmarkFace(cv::Mat faceImage, bbox_t bbox, std::vector<cv::Point2f> &landmark) {
	if (this->alignType == 0) {
		SeetaRect rect;
		rect.x = bbox.x;
		rect.y = bbox.y;
		rect.height = bbox.h;
		rect.width = bbox.w;
		seeta::cv::ImageData image = faceImage;
		SeetaPointF points[5];
		auto start = std::chrono::steady_clock::now();//记录检测开始时间
		bool flag = this->seetaLandmark->DetectEx(image, rect, points);
		//= PD.Detect(image, *face);
		auto end = std::chrono::steady_clock::now();//记录检测结束时间
		std::chrono::duration<double> spent = end - start;//检测时长
		std::cout << " landmark seeta Time: " << spent.count() << " sec \n";
		if (flag) {
			for (int i = 0; i < this->seetaLandmark->LandmarkNum(); i++) {
				landmark.push_back(cv::Point2f((points[i].x - rect.x), (points[i].y - rect.y)));
			}
		}
	}
	else if(this->alignType== 1){
		std::vector<cv::Point> point_5;
		cv::Mat rectImage;
		cv::Rect rect(bbox.x, bbox.y, bbox.w, bbox.h);
		// 裁剪
		faceImage(rect).copyTo(rectImage);
		// 检测五个点
		mFaceLandmarkAPI->landmarkFaces(rectImage, point_5);
		for (auto data : point_5) {
			landmark.push_back(cv::Point2f(data.x, data.y));
		}
	}
	else{
		// TODO
	}
}
void  FaceAlignment::alignFace(cv::Mat landmarkFace, std::vector<cv::Point2f>landmarkPoints,cv::Mat &alignFaceImg) {
	if (landmarkFace.empty()) {
		return;
	}
	double transD[5][2] = {
		{ 30.2946, 51.6963 },
		{ 65.5318, 51.5014 },
		{ 48.0252, 71.7366 },
		{ 33.5493, 92.3655 },
		{ 62.7299, 92.2041 }
	};
	for (int i = 0; i < 5; i++) {
		transD[i][0] += 8;
	}
	cv::Mat src(5, 2, CV_64F, transD);
	double point5[5][2];
	int i = 0;
	for (auto point : landmarkPoints) {
		point5[i][0] = point.x;
		point5[i][1] = point.y;
		i += 1;
	}
	cv::Mat dst(5, 2, CV_64F, point5);
	//for (int i = 0; i < 5; i++) {
	//	qDebug() << dst.at<double>(i, 0); // = this->imageAndParam.getLandmarkPoints()[i].x;
	//	qDebug() << dst.at<double>(i, 1); //  = this->imageAndParam.getLandmarkPoints()[i].y;
	//}
	cv::Mat M;
	cv::estimateRigidTransform(dst, src, true).copyTo(M);
	cv::Mat warped;
	// M为空直接返回裁剪完图片
	if (M.empty()) {
		qDebug() << "M is empty!!";
		landmarkFace.copyTo(alignFaceImg);
		return;
	}
	cv::warpAffine(landmarkFace, warped, M, { 112,112 });
	warped.copyTo(alignFaceImg);
}
void  FaceAlignment::transformImgAndPoint(cv::Mat srcImg, bbox_t bbox,int imageSize, cv::Mat &detectImg, std::vector<cv::Point2f> &landkmarkPoint) {
	cv::Rect rect(bbox.x, bbox.y, bbox.w, bbox.h);
	// 裁剪
	srcImg(rect).copyTo(detectImg);
	cv::resize(detectImg, detectImg, cv::Size(imageSize,imageSize), (0, 0), (0, 0), cv::INTER_LINEAR);
	float scale_x = imageSize / (bbox.w + 1e-7);
	float scale_y = imageSize / (bbox.h + 1e-7);
	for (int i = 0; i < landkmarkPoint.size(); i++) {
		landkmarkPoint[i].x *= scale_x;
		landkmarkPoint[i].y *= scale_y;
	}
}
cv::Mat FaceAlignment::rectImage(cv::Mat img, bbox_t bbox) {
	int minL = MIN(bbox.w, bbox.h);
	if (minL < 112) {

	}
	cv::Mat recImg;
	return recImg;
}