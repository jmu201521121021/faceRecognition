#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "faceDetector.h"
#include "faceRecognition.h"
#include "faceAlignment.h"

#include "sqlSplite.h"

class RegisterWindow : public QWidget
{
	Q_OBJECT

public:
	//QWidget *parent, 
	RegisterWindow(FaceDetector *faceDetector, FaceAlignment *faceAlignment, FaceRecognition *faceRecognition);
	~RegisterWindow();

	//初始化界面
	void initView();

	//释放界面
	void cleanView();
	//
	void paintEvent(QPaintEvent *e);

	//初始化摄像头
	void initCapture();


private slots:
	void shotButtonSlot();
	void confirmButtonSlot();
	void quitButtonSlot();
	void quitSlot();
private:
	/*   检测器 & 摄像头 & 识别器  都是从MainWindow中传递过来*/
	/*  检测器   */
	FaceDetector * faceDetector;
	/* 对齐*/
	FaceAlignment  *faceAlignment;
	/*  识别器   */
	FaceRecognition *faceRecognition;
	/*  摄像头   */
	cv::VideoCapture capture;

	bool hasOpen = false;  //判断摄像头是否是打开的状态
						   /*  数据结构   */
	struct FaceStructParam rFaceStructParam;  //注册界面的核心数据结构
	cv::Mat img;

	struct InformationParam infParam;

	SqlSplite *sqlSplite = NULL;


	QLabel *mainShow;
	QLabel *subShow;

	QLabel *nameLabel;
	QLineEdit *nameEdit;
	QLabel *noLabel;
	QLineEdit *noEdit;
	QLabel *collegeLabel;
	QLineEdit *collegeEdit;
	QLabel *classLabel;
	QLineEdit *classEdit;

	QPushButton *shotButton;//拍摄按钮
	QPushButton *confirmButton;//确认按钮
	QPushButton *quitButton;//退出按钮

	bool checkInformation();
};


#endif // REGISTERWINDOW_H
