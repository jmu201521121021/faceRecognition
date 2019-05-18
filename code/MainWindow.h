#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
//#include "yolo.h"
#include "faceDetector.h"    // 人脸检测器
#include "faceAlignment.h"   // 人脸对齐 
#include "faceRecognition.h"  // 人脸识别器
#include "datastructcommon.h"  //数据结构
#include "sqlSplite.h"

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QGridLayout>
#include <QFrame>
#include <vector>
#include <string>
#include <qDebug.h>
#include <QCombobox>
#include <QAxObject>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	/*   改变模式   */
	void changeMode(int);

	/*  初始化界面函数   */
	void initView();
	void initMenu();
	void cleanMenu();
	void initLayout();
	void cleanLayout();

	struct InformationParam predictFace(cv::Mat img, bool isRegister, struct FaceStructParam::FaceParam &faceParam);
	/**
	* @brief MainWindow::transformImgAndPoint
	* 获得裁剪后的人脸（resize）和由于图片resize关键点变换
	* 
	*/
/* old	/*   yolo    
	std::vector<std::string> objects_names_from_file(std::string);
	void initDetector();
*/
	inline void setDataSetInfomations() {
		this->dataSetInfomations = sqlSplite->query();
	}
	inline std::vector<InformationParam> getDataSetInfomations() {
		return this->dataSetInfomations;
	}

	void cleanModel();

	void checkFunction(); //检查标志位func，做出相应的动作
	inline void setSimTheta(float simTheta) {
		this->simTheta = simTheta;
	}
	inline float getSimTheta() {
		return this->simTheta;
	}

	inline void setVideoStreamRtsp(std::string videoStreamRtsp) {
		this->videoStreamRtsp = videoStreamRtsp;
	}
	inline std::string getVideoStreamRtsp() {
		return this->videoStreamRtsp;
	}

	inline void setUseUSBCamera(int useUSBCamera) {
		this->useUSBCamera = useUSBCamera;
	}
	inline int getUseUSBCamera() {
		return this->useUSBCamera;
	}

	inline void setUSBPort(int  USBPort) {
		this->USBPort = USBPort;
	}
	inline int getUSBPort() {
		return this->USBPort;
	}

	inline void setUseGpuRecognize(int  useGpuRecognize) {
		this->useGpuRecognize = useGpuRecognize;
	}
	inline int getUseGpuRecognize() {
		return this->useGpuRecognize;
	}
	inline void setDetectTheta(float theta) {
		this->detectTheta = theta;
	}
	inline float getDetectTheta() {
		return this->detectTheta;
	}
	// xml 
	bool readXmlData(cv::String xmlFileName);
	// open摄像头
	inline bool  openVedio()
	{
		// 使用自带摄像头
		int cameraMode = getUseUSBCamera();
		if (cameraMode == 0) {
			if (!capture.isOpened()) {
				capture.open(getUSBPort());
			}
		}
		// 使用USB摄像头
		else if (cameraMode == 1) {
			if (!capture.isOpened()) {
				qDebug() << "open usb camera!";
				capture.open(getUSBPort());
			}
		}
		// 使用网络摄像头
		else if (cameraMode == 2) {
			if (!capture.isOpened()) {
				capture.open(getVideoStreamRtsp());
			}
		}
		else {
			return false;
		}
		if (capture.isOpened()) {
			return true;
		}
		else {
			return false;
		}
	}
	// 关闭摄像头
	inline void releaseVedio()
	{
		capture.release();
	}
	inline std::vector<std::string> getStuNos() {
		return this->stuNos;
	}
	inline int findNoIndex(std::string number) {
		int index = -1;
		for (int i = 0; i < this->stuNos.size(); i++) {
			if (stuNos[i] == "0") {
				stuNos[i] = number;
				index = i;
				break;
			}
			if (this->stuNos[i] == number) {
				index = i;
				break;
			}
		}
		return index;
	}
	
	inline void setActivityId(int activityId) {
		this->activityId = activityId;
	}
	inline int getActivityId() {
		return this->activityId;
	}
	inline void setRecordId(int recordId) {
		this->recordId = recordId;
	}
	inline int getRecordId() {
		return this->recordId;
	}
	inline void setPushRecordId(int pushRecordId) {
		this->pushRecordId = pushRecordId;
	}
	inline int getPushRecordId() {
		return this->pushRecordId;
	}
	/*写入excel操作*/
	void writeExcel();
private slots:

	/*   槽 - 模式选择   */
	void openPictureSlot();   //图片按钮的响应槽
	void openVideoSlot();     //视屏按钮的响应槽
	void openCameraSlot();    //摄像头按钮的响应槽
	void openRegisterSlot();  //注册按钮的响应槽
	void quitSlot();

	/*   槽 - 功能选择   */
	void detFunctionSlot();    //检测功能
	void detAliRecFunctionSlot();  //检测 + 对齐 + 识别功能

	void paintEvent(QPaintEvent *e);
	void addActSlot();      // 添加活动事件
	void startRecordSlot(); 
	void selectBoxIndex(int index);
	void selectRecordIndex(int index);
	void pushRecordSlot();
private:
	

	QWidget *centralWidget;
	QWidget *subShowWidget;
	QWidget *rightDownWidget;

	/*  菜单                    */
	QMenuBar *bar;
	/*  菜单控件 - 模式选择mode  */
	QMenu *openFileMenu;
	QAction *openPictureAction;
	QAction *openVideoAction;
	QAction *openCameraAction;
	QAction *registerAction;
	QAction *quitAction;

	/*  菜单控件 - 功能选择func  */
	QMenu *functionMenu;
	QAction *detAction;
	QAction *detAliRecAction;

	/*   边界线      */

	//QFrame * line = new QFrame();
	//line->setStyleSheet("border:10px groove #242424");
	//line->setFrameShape(QFrame::HLine);
	//line->setFrameShadow(QFrame::Sunken);
	QLabel *leftSplitLine;
	//QFrame *leftLine;
	//QFrame *rightLine;
	QLabel *rightSplitLine;
	QFrame *centralLine;

	/*   布局            */
	QGridLayout *mainGridLayout;
	QGridLayout *rightTopLayout;
	QGridLayout *rightDownLayout;

	/*   布局中的控件    */
	QLabel *mainShow;

	QLabel *upLable;
	QLabel *subShow11;
	QLabel *subShow12;
	QLabel *simiInfo1;

	QLabel *subShow21;
	QLabel *subShow22;
	QLabel *simiInfo2;

	QLabel *subShow31;
	QLabel *subShow32;
	QLabel *simiInfo3;

	QLabel *subShow41;
	QLabel *subShow42;
	QLabel *simiInfo4;

	QLabel *informationShow;
	// 添加活动
	QPushButton *addActButton;      // 添加活动名称
	QPushButton *startRecordButton; // 开始考勤按钮
	QComboBox *selectBox;           // 活动选择
	QLabel *selectLabel;            //
	QLabel *selectRecordLabel;
	QComboBox *selectRecord;
	QPushButton *pushButton;
	/*   标志位    */
	//mode = 1   ->   picture;
	//mode = 2   ->   video;
	//mode = 3   ->   camera;
	int mode = 0;

	/*  人脸检测器   */
	 FaceDetector *faceDetector = NULL;
	 /* 人脸对齐*/
	 FaceAlignment *faceAlignment = NULL;
	/*  人脸识别器   */
	 FaceRecognition *faceRecognition = NULL;

	//cv::Mat picture;  //原始图片
	struct FaceStructParam faceStructParams;//核心数据结构
	struct InformationParam reInfParam; // 个人信息

    // 人脸检测参数文件地址
	std::string names_file = "../data/face.names";
	std::string cfg_file = "../cfg/yolov3-spp-focal_loss-test.cfg";
	std::string weights_file = "../model/yolov3-spp-focal_loss_12000.weights";
	

	/* 摄像头   */
	cv::VideoCapture capture;
	/*    功能 - 标志位    */
	//func = 1   ->   检测
	//func = 2   ->   检测 + 对齐 + 识别
	//默认模式 - 仅检测
	int func = 1;

	int faceSumNumber = 0; // 记录人脸数目，大于1000 变为0
	
	/*xml 文件参数*/
	// 摄像头
	float simTheta = 0.5;        // 相似阈值
	std::string videoStreamRtsp; // 网络摄像头
	int useUSBCamera = 1;        // 0表示不使用摄像头，1表示使用USB摄像头，2表示使用网络摄像头
	int USBPort = 0;             // 摄像头接口 0，1，2，3
	int useGpuRecognize = 1;     // 识别是否使用GPU
	float detectTheta = 0.7;     // 人脸检测分数 
	std::vector<std::string>stuNos = {"0","0","0","0"};
	int currentShowFace = -1;
	int imageSize = 112;
	int alignType = 0; // 0代表seeta2,1代表dlib
					   //数据库管理器
	 SqlSplite *sqlSplite = NULL;

	// 数据库所有人信息
	std::vector<InformationParam> dataSetInfomations;
	/*<考勤变量>*/
	// 选择活动名称
	int activityId;
	//是否点击考勤
	bool isStartRecord = false;
	// 已记录的数据
	std::vector<QString> recordNames;
	// 考勤ID
	int recordId = -1;
	// 导出ID
	int pushRecordId = -1;
};

#endif // MAINWINDOW_H
