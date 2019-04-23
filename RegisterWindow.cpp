#include "common.h"
#include "RegisterWindow.h"
#include <QHBoxLayout>
#include <qmessagebox.h>
#include <io.h>
#include <direct.h>

//QWidget *parent, 
RegisterWindow::RegisterWindow(FaceDetector *faceDetector,FaceAlignment *faceAlignment,FaceRecognition *faceRecognition) : QWidget()
{
	//初始化检测器
	this->faceDetector = faceDetector;  //从MainWindw中传递过来的检测器。（可以防止重复初始化检测器，节约时间和内存）
	this->faceAlignment = faceAlignment;
	this->faceRecognition = faceRecognition;
	//初始化摄像头
	initCapture();
	sqlSplite = new SqlSplite();

	//初始化界面
	initView();
}
RegisterWindow::~RegisterWindow()
{
}
/**
初始化界面
*/
void RegisterWindow::initView()
{
	this->setWindowTitle(SQ("注册"));
	this->setFixedSize(1000, 700);

	//左边
	mainShow = new QLabel();
	mainShow->setFixedSize(650, 650);
	mainShow->setStyleSheet("border:1px groove #242424");
	QHBoxLayout *left = new QHBoxLayout();
	left->addWidget(mainShow);
	left->setMargin(10);

	//右边
	//上
	subShow = new QLabel();
	subShow->setFixedSize(300, 300);
	subShow->setStyleSheet("border:1px groove #242424");
	QHBoxLayout *rightTop = new QHBoxLayout();
	rightTop->addWidget(subShow);
	//rightTop->setAlignment(Qt::AlignLeft);
	//中
	nameLabel = new QLabel(SQ("姓名:"));
	noLabel = new QLabel(SQ("学号:"));
	collegeLabel = new QLabel(SQ("学院:"));
	classLabel = new QLabel(SQ("班级:"));
	QVBoxLayout *rightMidL = new QVBoxLayout();
	rightMidL->setAlignment(Qt::AlignLeft);
	rightMidL->addWidget(nameLabel);
	rightMidL->addWidget(noLabel);
	rightMidL->addWidget(collegeLabel);
	rightMidL->addWidget(classLabel);

	nameEdit = new QLineEdit();
	nameEdit->setFixedSize(200, 30);
	noEdit = new QLineEdit();
	noEdit->setFixedSize(200, 30);
	collegeEdit = new QLineEdit();
	collegeEdit->setFixedSize(200, 30);
	classEdit = new QLineEdit();
	classEdit->setFixedSize(200, 30);
	QVBoxLayout *rightMidR = new QVBoxLayout();
	rightMidR->setAlignment(Qt::AlignLeft);
	rightMidR->addWidget(nameEdit);
	rightMidR->addWidget(noEdit);
	rightMidR->addWidget(collegeEdit);
	rightMidR->addWidget(classEdit);

	QHBoxLayout *rightMid = new QHBoxLayout();
	rightMid->addLayout(rightMidL);
	rightMid->addLayout(rightMidR);

	//下
	shotButton = new QPushButton();
	shotButton->setText(SQ("拍摄"));
	shotButton->setFixedSize(70, 30);
	connect(shotButton, SIGNAL(clicked()), this, SLOT(shotButtonSlot()));

	confirmButton = new QPushButton();
	confirmButton->setText(SQ("确认"));
	confirmButton->setFixedSize(70, 30);
	connect(confirmButton, SIGNAL(clicked()), this, SLOT(confirmButtonSlot()));

	quitButton = new QPushButton();
	quitButton->setText(SQ("退出"));
	quitButton->setFixedSize(70, 30);
	connect(quitButton, SIGNAL(clicked()), this, SLOT(quitButtonSlot()));

	QHBoxLayout *rightButtom = new QHBoxLayout();
	rightButtom->addWidget(shotButton);
	rightButtom->addWidget(confirmButton);
	rightButtom->addWidget(quitButton);
	rightButtom->setAlignment(Qt::AlignHCenter);
	rightButtom->setSpacing(10);

	QVBoxLayout *right = new QVBoxLayout();
	right->addLayout(rightTop);
	right->addLayout(rightMid);
	right->addLayout(rightButtom);

	//整合
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->addLayout(left);
	mainLayout->addLayout(right);
}
/**
初始化摄像头
*/
void RegisterWindow::initCapture()
{
	if (!capture.isOpened()) {
		capture.open(0);
	}
	
}

/**
释放界面
*/
void RegisterWindow::cleanView()
{
	if (mainShow)
	{
		delete mainShow;
		mainShow = NULL;
	}
	if (subShow)
	{
		delete subShow;
		subShow = NULL;
	}
	if (nameLabel)
	{
		delete nameLabel;
		nameLabel = NULL;
	}
	if (nameEdit)
	{
		delete nameEdit;
		nameEdit = NULL;
	}
	if (noLabel)
	{
		delete noLabel;
		noLabel = NULL;
	}
	if (noEdit)
	{
		delete noEdit;
		noEdit = NULL;
	}
	if (collegeEdit)
	{
		delete collegeEdit;
		collegeEdit = NULL;
	}
	if (classLabel)
	{
		delete classLabel;
		classLabel = NULL;
	}
	if (classEdit)
	{
		delete classEdit;
		classEdit = NULL;
	}
	if (shotButton)
	{
		delete shotButton;
		shotButton = NULL;
	}
	if (confirmButton)
	{
		delete confirmButton;
		confirmButton = NULL;
	}
	if (quitButton)
	{
		delete quitButton;
		quitButton = NULL;
	}
}

/*

*/
bool RegisterWindow::checkInformation()
{
	if (rFaceStructParam.faceParams.empty())  //没有人脸图片，说明没有检测到人脸
	{
		return false;
	}
	else if (nameEdit->text().isEmpty() || noEdit->text().isEmpty() || collegeEdit->text().isEmpty() || classEdit->text().isEmpty())
	{
		return false;
	}
	return true;
}

/**
拍摄按钮 - 拍摄
把 mainShow 中的人脸图片放到 subShow 中,提取特征
*/
void RegisterWindow::shotButtonSlot()
{
	//识别，主要是为了获取featureMap
	if (capture.isOpened()) {
		capture.release();
	}
	else {
		capture.open(0);
	}
	if (rFaceStructParam.faceParams.size() == 0) {
		QMessageBox message(QMessageBox::NoIcon, SQ("确定"), SQ("<font color = 'red'> 请重新拍照 </font>"));
		message.exec();
		return;
	}
	//this->faceRecognition->recognize(rImageAndParam, 0, true);
	/*struct FaceStructParam faceStructParam;
	rFaceStructParam = faceStructParam;
	struct FaceStructParam::FaceParam faceParam;*/
	std::vector<cv::Point2f> landmarkPoints;
	cv::Mat detectImg;
	// 关键点检测
	faceAlignment->landmarkFace(rFaceStructParam.image,rFaceStructParam.faceParams[0].detectBox, landmarkPoints);
	faceAlignment->transformImgAndPoint(rFaceStructParam.image, rFaceStructParam.faceParams[0].detectBox, 112, detectImg, landmarkPoints);
	
	// 对齐
	cv::Mat alignFace;
	faceAlignment->alignFace(detectImg, landmarkPoints, alignFace);
	rFaceStructParam.faceParams[0].featureMap =faceRecognition->makeFeature(alignFace);
	// 赋值
	rFaceStructParam.faceParams[0].landmarks = landmarkPoints;
	// rFaceStructParam.faceParams[0].push_back(faceParam);
	// faceParam.landmarks = landmarkPoints;
	// 对齐
	// cv::Mat alignFace;
	//faceAlignment->alignFace(detectImg, landmarkPoints, alignFace);
	/*  先检查填写信息的完整性   */
	if (checkInformation() == false)
	{
		//qDebug() << "22OK!";
		QMessageBox message(QMessageBox::NoIcon, SQ("确定"), SQ("<font color = 'red'> 请输入完整信息 </font>"));
		message.exec();
		return;
	}
	/*    信息是完整的   */
	setImage2Label(detectImg, subShow);

	//qDebug() << "OK!";
	QMessageBox message(QMessageBox::NoIcon, SQ("确定"), SQ("<font color = 'blue'> 信息正确 </font>"));
	message.exec();
}

void RegisterWindow::paintEvent(QPaintEvent *e)
{
	
	if (capture.isOpened())
	{
		FaceStructParam faceStructParam;
		rFaceStructParam = faceStructParam;
		capture >> img;
		rFaceStructParam.image = img.clone();
		cv::Mat drawMat = img.clone();
		std::vector<cv::Point2f> points;
		double simScore;
		std::string name;
		std::vector<bbox_t> boxes = faceDetector->faceDetectBoxes(img);
		for (int i = 0; i < boxes.size(); i++) {
			struct FaceStructParam::FaceParam faceParam;
			faceParam.detectBox = boxes[i];
			rFaceStructParam.faceParams.push_back(faceParam);
			drawAllMessages(drawMat, boxes[i], points, simScore, name, false);
		}
		
		//draw_boxes(picture, result_vec, obj_names, spent.count());//画框
		setImage2Label(drawMat, mainShow);
	}
}
/**
确定按钮 - 确定
把用户的人脸信息和输入的相关信息存放入数据库中
*/
void RegisterWindow::confirmButtonSlot()
{
	if (rFaceStructParam.faceParams.size() == 0 || checkInformation() == false ) {
		QMessageBox message(QMessageBox::NoIcon, SQ("确定"), SQ("<font color = 'red'> 请先拍照和填写信息 </font>"));
		message.exec();
		return;
	}
	string savePath = "../faces/";
	if (_access(savePath.c_str(), 0) == -1) //不存在
		mkdir(savePath.c_str());
	//把截取的人脸图片放入到磁盘中
	std::string pictureRoute = savePath + QS(noEdit->text()) + ".png";
	cv::Mat rectImage;
	// 裁剪人脸
	bbox_t bbox = rFaceStructParam.faceParams[0].detectBox;
	cv::Rect rect(bbox.x, bbox.y, bbox.w, bbox.h);
	rFaceStructParam.image(rect).copyTo(rectImage);
	cv::imwrite(pictureRoute,rectImage);
	//TODO:  保存信息到数据库中

	infParam.setName(QS(QString(nameEdit->text())));
	infParam.setNo(QS(QString(noEdit->text())));
	infParam.setCollege(QS(QString(collegeEdit->text())));
	infParam.setClass(QS(QString(classEdit->text())));
	infParam.setPictureRoute(pictureRoute);

	//插入到数据库中
	//输入参数
	//1、featureMap
	//2、注册信息
	sqlSplite->insert(rFaceStructParam.faceParams[0].featureMap, infParam);//featureMap 和 基本的信息

															 ///////
															 //显示“保存成功”对话框
															 //if(success)
	QMessageBox message(QMessageBox::NoIcon, SQ("确定"), SQ("<font color = 'red'> 信息保存成功 </font>"));
	message.setIconPixmap(QPixmap(SQ(pictureRoute)));  //TODO::把人脸图片放在这里！
	message.exec();
}
/**
退出按钮 - 退出
退出此对话框
*/
void RegisterWindow::quitButtonSlot()
{
	cleanView();

	if (capture.isOpened())//一开始进来的时候，摄像头是关着的，所以出去也要释放掉
		this->capture.release();
	this->close();
	//this->close();
}

void RegisterWindow::quitSlot() {
	if (capture.isOpened()) {
		capture.release();
	}
	this->close();
}