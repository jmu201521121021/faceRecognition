#include "common.h"
#include "RegisterWindow.h"
#include <QHBoxLayout>
#include <qmessagebox.h>
#include <QFileDialog>
#include <io.h>
#include <direct.h>

//QWidget *parent, 
RegisterWindow::RegisterWindow(FaceDetector *faceDetector,FaceAlignment *faceAlignment,FaceRecognition *faceRecognition) 
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
	cleanView();
	releaseModel();
}
void RegisterWindow::releaseModel() {
	if (faceDetector) {
		delete faceDetector;
		faceDetector = NULL;
	}
	if (faceAlignment) {
		delete faceAlignment;
		faceAlignment = NULL;
	}
	if (faceRecognition) {
		delete faceRecognition;
		faceRecognition = NULL;
	}
}
/**
初始化界面
*/
void RegisterWindow::initView()
{
	centralWidget = new QWidget(); //中央容器
	/*右边布局初始化*/
	rightTopLayout = new QGridLayout();
	rightTopLayout->setSpacing(5);
	rightTopLayout->setHorizontalSpacing(5);
	//rightTopLayout->setAlignment(Qt::AlignCenter);
	rightTopLayout->setMargin(5);
	mainGridLayout = new QGridLayout(centralWidget);
	mainGridLayout->setSpacing(5);

	this->setWindowTitle(SQ("注册"));
	this->setFixedSize(1000, 700);

	//左边
	mainShow = new QLabel();
	mainShow->setFixedSize(640, 480);
	mainShow->setStyleSheet("border:1px groove #242424");

	//右边
	//上
	subShow = new QLabel();
	subShow->setFixedSize(300, 300);
	subShow->setStyleSheet("border:1px groove #242424");
	//中
	nameLabel = new QLabel(SQ("姓名:"));
	nameLabel->setAlignment(Qt::AlignRight);
	noLabel = new QLabel(SQ("学号:"));
	noLabel->setAlignment(Qt::AlignRight);
	collegeLabel = new QLabel(SQ("学院:"));
	collegeLabel->setAlignment(Qt::AlignRight);
	classLabel = new QLabel(SQ("班级:"));
    classLabel->setAlignment(Qt::AlignRight);

	nameEdit = new QLineEdit();
	nameEdit->setFixedSize(180, 30);
	noEdit = new QLineEdit();
	noEdit->setFixedSize(180, 30);
	collegeEdit = new QComboBox();
	collegeEdit->setFixedSize(180, 30);
	for (auto name : collegeNames) {
		collegeEdit->addItem(name);
	}
	classEdit = new QLineEdit();
	classEdit->setFixedSize(180, 30);
	//下
	shotButton = new QPushButton();
	shotButton->setText(SQ("打开摄像头"));
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

	upFaceButton = new QPushButton();
	upFaceButton->setText(SQ("上传本地图片"));
	upFaceButton->setFixedSize(100, 30);
	connect(upFaceButton, SIGNAL(clicked()), this, SLOT(upFaceButtonSlot()));

	upBatchFaceButton = new QPushButton();
	upBatchFaceButton->setText(SQ("批量上传照片"));
	upBatchFaceButton->setFixedSize(100, 30);
	connect(upBatchFaceButton, SIGNAL(clicked()), this, SLOT(upBatchFaceButtonSlot()));

	rightTopLayout->addWidget(subShow, 0, 0, 1, 3);
	rightTopLayout->addWidget(nameLabel, 1, 0, 1, 1);
	rightTopLayout->addWidget(nameEdit, 1, 1, 1, 2);
	rightTopLayout->addWidget(noLabel, 2, 0, 1, 1);
	rightTopLayout->addWidget(noEdit, 2, 1, 1, 2);
	rightTopLayout->addWidget(collegeLabel, 3, 0, 1, 1);
	rightTopLayout->addWidget(collegeEdit, 3, 1, 1, 2);
	rightTopLayout->addWidget(classLabel, 4, 0, 1, 1);
	rightTopLayout->addWidget(classEdit, 4, 1, 1, 2);
	rightTopLayout->addWidget(shotButton, 5, 0, 1, 1);
	rightTopLayout->addWidget(upFaceButton, 5, 1, 1, 1);
	rightTopLayout->addWidget(upBatchFaceButton, 5, 2, 1, 1);
	rightTopLayout->addWidget(confirmButton, 6, 0,1, 1);
	rightTopLayout->addWidget(quitButton, 6, 1, 1, 1);

	subShowWidget = new QWidget(centralWidget);
	subShowWidget->setLayout(rightTopLayout);

	mainGridLayout->addWidget(mainShow, 0, 0, 1, 1);
	mainGridLayout->addWidget(subShowWidget, 0, 1, 1, 1);
	// this->showMaximized();//设置全屏显示
	this->setCentralWidget(centralWidget);
	this->setFixedSize(1000, 600);
}
/**
初始化摄像头
*/
void RegisterWindow::initCapture()
{
	if (capture.isOpened()) {
		capture.release();
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
	if (upFaceButton) {
		delete upFaceButton;
		upFaceButton = NULL;
	}
	if (upBatchFaceButton) {
		delete upBatchFaceButton;
		upBatchFaceButton = NULL;
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
	else if (nameEdit->text().isEmpty() || noEdit->text().isEmpty() || collegeEdit->currentText().isEmpty() || classEdit->text().isEmpty())
	{
		return false;
	}
	return true;
}

void RegisterWindow::upFaceButtonSlot() {
	registerType = 1;
	QString fileName = QFileDialog::getOpenFileName(this, SQ("选择图片"), "", tr("Images (*.png *.bmp *.jpg)"));
	if (fileName.isEmpty()) {
		QMessageBox message(QMessageBox::NoIcon, SQ("确定"), SQ("<font color = 'red'> 图片选择错误，请重新选择 </font>"));
		message.exec();
		return;
	}
	// 清洗
	FaceStructParam faceStructParam;
	rFaceStructParam = faceStructParam;

	cv::Mat image = cv::imread(QS(fileName));
	rFaceStructParam.image = image.clone();

	cv::Mat drawMat = image.clone();
	std::vector<cv::Point2f> points;
	double simScore;
	std::string name;
	std::vector<bbox_t> boxes = faceDetector->faceDetectBoxes(image);
	for (int i = 0; i < boxes.size(); i++) {
		struct FaceStructParam::FaceParam faceParam;
		faceParam.detectBox = boxes[i];
		rFaceStructParam.faceParams.push_back(faceParam);
		drawAllMessages(drawMat, boxes[i], points, simScore, name, false);
	}
	if (boxes.size() == 0) {
		QMessageBox message(QMessageBox::NoIcon, SQ("确定"), SQ("<font color = 'red'> 未检测到人脸，请重新上传图片 </font>"));
		message.exec();
		return;
	}
	//draw_boxes(picture, result_vec, obj_names, spent.count());//画框
	setImage2Label(drawMat, mainShow);
	cv::Mat detectImg;
	this->predictFace(detectImg);
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
void RegisterWindow::upBatchFaceButtonSlot() {
	registerType = 2;
	QString file = QFileDialog::getExistingDirectory(this, SQ("选择文件夹"), ""); //getOpenFileName(this, SQ("选择图片"), "", tr("Images (*.png *.bmp *.jpg)"));
	QDir dirPath(file);
	QStringList nameFilters;
	nameFilters << "*.jpg" << "*.png"<<"*.bmp";
	QStringList files = dirPath.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
	if (files.isEmpty()) {
		QMessageBox message(QMessageBox::NoIcon, SQ("确定"), SQ("<font color = 'red'> 请重新选择文件夹，只支持jpg、png、bmp格式图片 </font>"));
		message.exec();
		return;
	}
	for (auto path : files) {
		QString fileName = file +"/"+ path;
		FaceStructParam faceStructParam;
		rFaceStructParam = faceStructParam;

		cv::Mat image = cv::imread(QS(fileName));
		rFaceStructParam.image = image.clone();
		std::vector<cv::Point2f> points;
		std::vector<bbox_t> boxes = faceDetector->faceDetectBoxes(image);
		for (int i = 0; i < boxes.size(); i++) {
			struct FaceStructParam::FaceParam faceParam;
			faceParam.detectBox = boxes[i];
			rFaceStructParam.faceParams.push_back(faceParam);
		}
		if (boxes.size() > 0) {
			cv::Mat detectImg;
			this->predictFace(detectImg);
			this->saveMessage(fileName);
			qDebug() << fileName;
		}
	}
}
/**
拍摄按钮 - 拍摄
把 mainShow 中的人脸图片放到 subShow 中,提取特征
*/
void RegisterWindow::shotButtonSlot()
{
	//识别，主要是为了获取featureMap
	registerType = 0;
	if (this->shotButton->text()== SQ("打开摄像头")){
		shotButton->setText(SQ("拍照"));
		capture.open(0);
	}
	else if (this->shotButton->text() == SQ("拍照")) {
		capture.release();
		shotButton->setText(SQ("打开摄像头"));
		// mainShow->clear();
		if (rFaceStructParam.faceParams.size() == 0) {
			QMessageBox message(QMessageBox::NoIcon, SQ("确定"), SQ("<font color = 'red'> 请重新拍照 </font>"));
			message.exec();
			return;
		}
		cv::Mat detectImg;
		this->predictFace(detectImg);

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
}

void RegisterWindow::paintEvent(QPaintEvent *e)
{
	if (registerType == 0) {
		if (capture.isOpened()) {
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
	else {
		if (capture.isOpened()) {
			capture.release();
		}
		
	}
}

void RegisterWindow::saveMessage(const QString imagePath) {
	QFileInfo fileInfo = QFileInfo(imagePath);
	string savePath = "../faces/";
	if (_access(savePath.c_str(), 0) == -1) //不存在
		mkdir(savePath.c_str());
	//把截取的人脸图片放入到磁盘中
	std::string pictureRoute;
	if (registerType == 2) {
		pictureRoute = savePath + QS(fileInfo.fileName());
	}
	else {
		pictureRoute = savePath + QS(noEdit->text()) + ".png";
	}
	cv::Mat rectImage;
	// 裁剪人脸
	bbox_t bbox = rFaceStructParam.faceParams[0].detectBox;
	cv::Rect rect(bbox.x, bbox.y, bbox.w, bbox.h);
	rFaceStructParam.image(rect).copyTo(rectImage);
	cv::imwrite(pictureRoute, rectImage);
	//TODO:  保存信息到数据库中
	if (registerType == 2) { // 批量上传
		QString file_name = fileInfo.fileName();
		QString messagesList = file_name.split(".")[0]; // 信息
		QStringList datas = messagesList.split("_");
		infParam.setName(QS(datas[0]));
		infParam.setNo(QS(datas[1]));
		infParam.setCollege(QS(datas[2]));
		infParam.setClass(QS(datas[3]));
		infParam.setPictureRoute(pictureRoute);
	}
	else {
		infParam.setName(QS(QString(nameEdit->text())));
		infParam.setNo(QS(QString(noEdit->text())));
		infParam.setCollege(QS(QString(collegeEdit->currentText())));
		infParam.setClass(QS(QString(classEdit->text())));
		infParam.setPictureRoute(pictureRoute);
	}

	//插入到数据库中
	//输入参数
	//1、featureMap
	//2、注册信息
	sqlSplite->insert(rFaceStructParam.faceParams[0].featureMap, infParam);//featureMap 和 基本的信息

																		   ///////
																		   //显示“保存成功”对话框
																		   //if(success)
	
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
	saveMessage();
	QMessageBox message(QMessageBox::NoIcon, SQ("确定"), SQ("<font color = 'red'> 信息保存成功 </font>"));
	//message.setIconPixmap(QPixmap(SQ(pictureRoute)));  //TODO::把人脸图片放在这里！
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
void RegisterWindow::predictFace(cv::Mat &detectImg) {
	std::vector<cv::Point2f> landmarkPoints;
	
	// 关键点检测
	faceAlignment->landmarkFace(rFaceStructParam.image, rFaceStructParam.faceParams[0].detectBox, landmarkPoints);
	faceAlignment->transformImgAndPoint(rFaceStructParam.image, rFaceStructParam.faceParams[0].detectBox, 112, detectImg, landmarkPoints);

	// 对齐
	cv::Mat alignFace;
	faceAlignment->alignFace(detectImg, landmarkPoints, alignFace);
	rFaceStructParam.faceParams[0].featureMap = faceRecognition->makeFeature(alignFace);
	// 赋值
	rFaceStructParam.faceParams[0].landmarks = landmarkPoints;
}