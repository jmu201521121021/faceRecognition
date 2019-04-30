#include "MainWindow.h"
#include "RegisterWindow.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMenu>
#include <qmenubar.h>
#include <qDebug.h>
#include <QMessageBox>
#include <QInputDialog>
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	sqlSplite = new SqlSplite();
	
	/*   初始化界面  */

	initView();

	/*读取xml*/
	readXmlData("./params.xml");

	/*  初始化人脸检测器  */
	faceDetector = new FaceDetector(names_file, cfg_file,weights_file);
	/* 初始化人脸对齐 */
	faceAlignment = new FaceAlignment(alignType);
	/* 初始化人脸识别 */
	faceRecognition = new FaceRecognition(useGpuRecognize == 1 ? true: false);
	

}

MainWindow::~MainWindow()
{
	cleanMenu();  //释放菜单
	cleanLayout();  //释放布局
	cleanModel();   //释放检测器 & 识别器
}

/**
* @brief MainWindow::initView - 初始化界面：包括初始化菜单和布局
*/
void MainWindow::initView()
{
	/*  菜单  */
	initMenu();
	/*  布局  */
	initLayout();
}
/**
* @brief MainWindow::initMenu - 初始化菜单栏
*/
void MainWindow::initMenu()
{
	/* 菜单 - 模式选择mode*/
	openFileMenu = new QMenu(SQ("模式"));

	openPictureAction = new QAction(SQ("图片"), this);
	openPictureAction->setShortcut(tr("Ctrl+p"));
	connect(openPictureAction, SIGNAL(triggered()), this, SLOT(openPictureSlot()));
	openFileMenu->addAction(openPictureAction);

	openVideoAction = new QAction(SQ("视频"), this);
	openVideoAction->setShortcut(tr("Ctrl+V"));
	connect(openVideoAction, SIGNAL(triggered()), this, SLOT(openVideoSlot()));
	openFileMenu->addAction(openVideoAction);

	openCameraAction = new QAction(SQ("摄像头"), this);
	openCameraAction->setShortcut(tr("Ctrl+C"));
	connect(openCameraAction, SIGNAL(triggered()), this, SLOT(openCameraSlot()));
	openFileMenu->addAction(openCameraAction);

	registerAction = new QAction(SQ("用户注册"), this);
	connect(registerAction, SIGNAL(triggered()), this, SLOT(openRegisterSlot()));

	quitAction = new QAction(SQ("退出"), this);
	quitAction->setShortcut(tr("Ctrl+q"));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quitSlot()));
	openFileMenu->addSeparator();
	openFileMenu->addAction(quitAction);

	/*  菜单 - 功能选择func   */
	functionMenu = new QMenu(SQ("功能"));
	
	detAction = new QAction(SQ("检测"), this);
	//openPictureAction->setShortcut(tr("Ctrl+p"));
	connect(detAction, SIGNAL(triggered()), this, SLOT(detFunctionSlot()));
	functionMenu->addAction(detAction);

	detAliRecAction = new QAction(SQ("检测+识别"));
	connect(detAliRecAction, SIGNAL(triggered()), this, SLOT(detAliRecFunctionSlot()));
	functionMenu->addAction(detAliRecAction);

	/* 把菜单添加到菜单栏上  */
	bar = this->menuBar();
	bar->addMenu(openFileMenu);
	bar->addMenu(functionMenu);
	bar->addAction(registerAction);

	/* 标题 */
	this->setWindowTitle(SQ("人脸检测与识别系统"));
}

/**
* @brief MainWindow::cleanMenu - 回收菜单栏
*/
void MainWindow::cleanMenu()
{
	if (openFileMenu)
	{
		delete openFileMenu;
		openFileMenu = NULL;
	}
	if (openPictureAction)
	{
		delete openPictureAction;
		openPictureAction = NULL;
	}
	if (openVideoAction)
	{
		delete openVideoAction;
		openVideoAction = NULL;
	}
	if (openCameraAction)
	{
		delete openCameraAction;
		openCameraAction = NULL;
	}
	if (detAction)
	{
		delete detAction;
		detAction = NULL;
	}
	if (detAliRecAction)
	{
		delete detAliRecAction;
		detAliRecAction = NULL;
	}
	if (functionMenu)
	{
		delete functionMenu;
		functionMenu = NULL;
	}
}
/**
* @brief MainWindow::initLayout - 初始化控件布局
*/
void MainWindow::initLayout()
{
	centralWidget = new QWidget(); //中央容器
	//初始化 - 显示图片的QLabel
	mainShow = new QLabel();
	mainShow->setAlignment(Qt::AlignCenter);  //居中
	mainShow->setScaledContents(true);  //图片自适应
	QSize mainShowSize(1320, 960);
	mainShow->setFixedSize(mainShowSize);
	mainShow->setStyleSheet("border: 1px groove #242424");

	QSize upLabelSize(1920, 60);
	upLable = new QLabel();
	upLable->setFixedSize(upLabelSize);

	//初始化 - 边线
	/*leftLine = new QFrame();
	leftLine->setStyleSheet("border:2px groove #242424");
	leftLine->setFrameShape(QFrame::HLine);
	leftLine->setFrameShadow(QFrame::Raised);*/

	leftSplitLine = new QLabel();
	leftSplitLine->setFixedWidth(5);
	leftSplitLine->setStyleSheet("background-color:black");
	
	/*rightLine = new QFrame();
	rightLine->setFrameShape(QFrame::HLine);
	rightLine->setFrameShadow(QFrame::Sunken);*/
	rightSplitLine = new QLabel();
	rightSplitLine->setFixedWidth(5);
	rightSplitLine->setStyleSheet("background-color:black");

	centralLine = new QFrame();
	centralLine->setStyleSheet("border: 5px groove #242424");
	centralLine->setFrameShape(QFrame::HLine);
	centralLine->setFrameShadow(QFrame::Sunken);


	//初始化 - 右上角
	QSize subLabelSize(150, 150);
	QSize simiInfoSize(100, 100);
	rightTopLayout = new QGridLayout();
	rightTopLayout->setSpacing(20);
	rightTopLayout->setHorizontalSpacing(60);
	//rightTopLayout->setAlignment(Qt::AlignCenter);
	rightTopLayout->setMargin(5);

	
	// 字体设置
	QFont ft;
	ft.setPointSize(20);
	subShow11 = new QLabel();
	subShow11->setStyleSheet("border:1px groove #242424");
	subShow11->setFixedSize(subLabelSize);
	subShow11->setScaledContents(true);
	subShow12 = new QLabel();
	subShow12->setStyleSheet("border:1px groove #242424");
	subShow12->setFixedSize(subLabelSize);
	subShow12->setScaledContents(true);
	simiInfo1 = new QLabel();
	simiInfo1->setFixedSize(simiInfoSize);
	simiInfo1->setStyleSheet("border-radius: 50px; border: 1px solid black;");
	simiInfo1->setFont(ft);
	simiInfo1->setAlignment(Qt::AlignCenter);

	subShow21 = new QLabel();
	subShow21->setStyleSheet("border:1px groove #242424");
	subShow21->setFixedSize(subLabelSize);
	subShow21->setScaledContents(true);

	subShow22 = new QLabel();
	subShow22->setStyleSheet("border:1px groove #242424");
	subShow22->setFixedSize(subLabelSize);
	subShow22->setScaledContents(true);

	simiInfo2 = new QLabel();
	simiInfo2->setFixedSize(simiInfoSize);
	simiInfo2->setStyleSheet("border-radius: 50px; border: 1px solid black;");
	simiInfo2->setFont(ft);
	simiInfo2->setAlignment(Qt::AlignCenter);

	subShow31 = new QLabel();
	subShow31->setStyleSheet("border:1px groove #242424");
	subShow31->setFixedSize(subLabelSize);
	subShow31->setScaledContents(true);

	subShow32 = new QLabel();
	subShow32->setStyleSheet("border:1px groove #242424");
	subShow32->setFixedSize(subLabelSize);
	subShow32->setScaledContents(true);

	simiInfo3 = new QLabel();
	simiInfo3->setFixedSize(simiInfoSize);
	simiInfo3->setStyleSheet("border-radius: 50px; border: 1px solid black;");
	simiInfo3->setFont(ft);
	simiInfo3->setAlignment(Qt::AlignCenter);

	subShow41 = new QLabel();
	subShow41->setStyleSheet("border:1px groove #242424");
	subShow41->setFixedSize(subLabelSize);
	subShow41->setScaledContents(true);

	subShow42 = new QLabel();
	subShow42->setStyleSheet("border:1px groove #242424");
	subShow42->setFixedSize(subLabelSize);
	subShow42->setScaledContents(true);

	simiInfo4 = new QLabel();
	simiInfo4->setFixedSize(simiInfoSize);
	simiInfo4->setStyleSheet("border-radius: 50px; border: 1px solid black;");
	simiInfo4->setFont(ft);
	simiInfo4->setAlignment(Qt::AlignCenter);

	rightTopLayout->addWidget(subShow11, 0, 0);
	rightTopLayout->addWidget(simiInfo1, 0, 1);
	rightTopLayout->addWidget(subShow12, 0, 2);

	rightTopLayout->addWidget(subShow21, 1, 0);
	rightTopLayout->addWidget(simiInfo2, 1, 1);
	rightTopLayout->addWidget(subShow22, 1, 2);

	rightTopLayout->addWidget(subShow31, 2, 0);
	rightTopLayout->addWidget(simiInfo3, 2, 1);
	rightTopLayout->addWidget(subShow32, 2, 2);

	rightTopLayout->addWidget(subShow41, 3, 0);
	rightTopLayout->addWidget(simiInfo4, 3, 1);
	rightTopLayout->addWidget(subShow42, 3, 2);

	subShowWidget = new QWidget(centralWidget);
	subShowWidget->setLayout(rightTopLayout);
	
	//初始化 - 右下角
	rightDownLayout = new QGridLayout();
	rightDownLayout->setSpacing(10);
	rightDownLayout->setHorizontalSpacing(10);
	rightDownLayout->setMargin(5);

	addActButton = new QPushButton();
	addActButton->setFixedSize(60, 30);
	addActButton->setText(SQ("添加活动"));
	startRecordButton = new QPushButton();
	startRecordButton->setFixedSize(60, 30);
	startRecordButton->setText(SQ("开始考勤"));
	informationShow = new QLabel();
	informationShow->setStyleSheet("border:1px groove #242424");

	selectBox = new QComboBox();
	selectBox->setFixedSize(100, 30);
	std::vector<QString> comBoxStr = sqlSplite->selectActivity();
	for (auto str :comBoxStr) {
		selectBox->addItem(str);
	}
	// 设置活动
	// setActivityName(selectBox->currentText());
	//qDebug() <<"sdsdsd"<< getActivityName();
	connect(addActButton, SIGNAL(clicked()), this, SLOT(addActSlot()));
	connect(startRecordButton, SIGNAL(clicked()), this, SLOT(startRecordSlot()));
	connect(selectBox, SIGNAL(activated(int)), this, SLOT(selectBoxIndex(int)));

	rightDownLayout->addWidget(addActButton, 0, 0, 1, 1);
	rightDownLayout->addWidget(startRecordButton, 0, 2, 1, 1);
	rightDownLayout->addWidget(informationShow, 1, 0, 1, 3);
	rightDownLayout->addWidget(selectBox, 0, 1, 1, 1);
	rightDownWidget = new QWidget(centralWidget);
	rightDownWidget->setLayout(rightDownLayout);
	//初始化 - 主布局
	mainGridLayout = new QGridLayout(centralWidget);
	mainGridLayout->setSpacing(10);

	//布局整合
	mainGridLayout->addWidget(upLable, 0, 0, 1, 2);
	mainGridLayout->addWidget(mainShow, 1, 0, 3, 1);
	//mainGridLayout->addWidget(leftLine, 0, 1, 3, 1);
	//mainGridLayout->addWidget(leftSplitLine, 0, 1, 3, 1);

	//mainGridLayout->addLayout(rightTopLayout, 0, 2, 1, 1);
	mainGridLayout->addWidget(subShowWidget, 1, 1, 1, 1);
	mainGridLayout->addWidget(centralLine, 2, 1, 1, 1);
	mainGridLayout->addWidget(rightDownWidget, 3, 1, 1, 1);
	//mainGridLayout->addWidget(rightSplitLine, 0, 3, 3, 1);

	// centralWidget->setAutoFillBackground(true);
	QPalette pale;
	pale.setBrush(QPalette::Background, QBrush(QPixmap("./mainw.png")));
	//centralWidget->setPalette(pale);
	this->setCentralWidget(centralWidget);
	// this->showMaximized();//设置全屏显示
	this->setPalette(pale);
	this->setFixedSize(1920, 960);

}

/**
* @brief MainWindow::cleanLayout - 释放布局
*/
void MainWindow::cleanLayout()
{
	if (centralWidget)
	{
		delete centralWidget;
		centralWidget = NULL;
	}
	if (subShowWidget)
	{
		delete subShowWidget;
		subShowWidget = NULL;
	}
	if (leftSplitLine)
	{
		delete leftSplitLine;
		leftSplitLine = NULL;
	}
	if (rightSplitLine)
	{
		delete rightSplitLine;
		rightSplitLine = NULL;
	}
	if (centralLine)
	{
		delete centralLine;
		centralLine = NULL;
	}
	if (mainGridLayout)
	{
		delete mainGridLayout;
		mainGridLayout = NULL;
	}
	if (rightTopLayout)
	{
		delete rightTopLayout;
		rightTopLayout = NULL;
	}
	if (mainShow)
	{
		delete mainShow;
		mainShow = NULL;
	}
	if (subShow11)
	{
		delete subShow11;
		subShow11 = NULL;
	}
	if (subShow12)
	{
		delete subShow12;
		subShow12 = NULL;
	}
	if (subShow21)
	{
		delete subShow21;
		subShow21 = NULL;
	}
	if (subShow22)
	{
		delete subShow22;
		subShow22 = NULL;
	}
	if (subShow31)
	{
		delete subShow31;
		subShow31 = NULL;
	}
	if (subShow32)
	{
		delete subShow32;
		subShow32 = NULL;
	}
	if (subShow41)
	{
		delete subShow41;
		subShow41 = NULL;
	}
	if (subShow42)
	{
		delete subShow42;
		subShow42 = NULL;
	}
	if (simiInfo1)
	{
		delete simiInfo1;
		simiInfo1 = NULL;
	}
	if (simiInfo2)
	{
		delete simiInfo2;
		simiInfo2 = NULL;
	}
	if (simiInfo3)
	{
		delete simiInfo3;
		simiInfo3 = NULL;
	}
	if (simiInfo4)
	{
		delete simiInfo4;
		simiInfo4 = NULL;
	}
	if (informationShow)
	{
		delete informationShow;
		informationShow = NULL;
	}
}

void MainWindow::changeMode(int mode)
{
	if (this->mode == 3 || this->mode == 2) // 摄像头或者视频流都把capture释放掉
	{
		//TODO: 关摄像头
		if (capture.isOpened()) {
			capture.release();
		}
	}
	//改变状态
	this->mode = mode;

	//清空  mainShow   （不需要的操作）
	//this->mainShow->repaint();
	//this->mainShow->clear();
}

/*   释放 - 自己的检测器   */
void MainWindow::cleanModel()
{
	if (faceDetector)
	{
		delete faceDetector;
		faceDetector = NULL;
	}
	if (faceRecognition) {
		delete faceRecognition;
		faceRecognition = NULL;
	}
	
}

/**
* @brief MainWindow::openPictureSlot - 检测&识别 - 图片
*/
void MainWindow::openPictureSlot()
{
	this->setDataSetInfomations();
	//打开文件夹 - 选择图片
	QString fileName = QFileDialog::getOpenFileName(this, SQ("选择图片"), "", tr("Images (*.png *.bmp *.jpg)"));
	if (fileName.isEmpty())
		return;

	//读取图片
	//picture = cv::imread(QS(fileName));
	//imageAndParam.image = cv::imread(QS(fileName));

	faceStructParams.image = cv::imread(QS(fileName));
	//setImage2Label(imageAndParam.image, mainShow);  //处理后的图片挂载到mainShow上

	//改变模式 - 图片
	changeMode(1);
	//检查功能
	checkFunction();
	//setImage2Label(imageAndParam.image, mainShow);  //处理后的图片挂载到mainShow上
	setImage2Label(faceStructParams.image, mainShow);
}

/**
* @brief MainWindow::openPictureSlot - 检测&识别 - 视频
*/
void MainWindow::openVideoSlot()
{
	this->setDataSetInfomations();
	//打开文件夹 - 选择视频
	QString fileName = QFileDialog::getOpenFileName(this, SQ("选择视频"), "", tr("Images (*.avi *.mp4)"));
	if (fileName.isEmpty())
		return;
	//改变模式 - 视频流
	changeMode(2);

	capture.open(QS(fileName));
	if (!capture.isOpened())
		return;
	//int numberFrames = capture.get(CV_CAP_PROP_FRAME_COUNT);//获取视频的总帧数
}
/**
* @brief MainWindow::openPictureSlot - 检测&识别 - 摄像头
*/
void MainWindow::openCameraSlot()
{
	this->setDataSetInfomations();
	if (this->mode == 3 &&capture.isOpened()) {
		capture.release();
		openCameraAction->setText(SQ("打开摄像头"));
		changeMode(0);
		qDebug() << "mode change :" << mode;
	}
	else //否则：改变mode；打开摄像头；处理摄像头中读取的帧
	{
		openCameraAction->setText(SQ("关闭摄像头"));
		changeMode(3); //改变mode为3（摄像头模式）

		//capture.open(0);//打开第一个摄像头
		openVedio();

		if (!capture.isOpened()) {
			qDebug() << "open video erro";
			return;  //不能打开摄像头
		}
	}
}
struct InformationParam  MainWindow::predictFace(cv::Mat img, bool isRegister, struct FaceStructParam::FaceParam &faceParam) {
	struct InformationParam infParam;
	NDArray feature = faceRecognition->makeFeature(img);
	std::string strFeature = featureMapToString(feature);
	// 进行数据库比对
	if (!isRegister)
	{
		vector<InformationParam> detSet = this->getDataSetInfomations();

		int maxIndex = 0;
		float maxSim = -1, sim;

		if (detSet.size() == 0)
		{
			qDebug() << SQ("数据库中没有数据!");
			return infParam;
		}

		for (int i = 0; i < detSet.size(); i++)
		{
			sim = faceRecognition->caculateSim(strFeature, detSet[i].getStringFeature());
			qDebug() << "相似度:"<<sim;
			if (sim > maxSim)
			{
				maxSim = sim;
				maxIndex = i;
			}
		}
		faceParam.score = maxSim;
		infParam = detSet[maxIndex];
		qDebug() << "maxSim" << maxSim << " name:" << SQ(infParam.getName());
	}
	// 插入数据库表record
	if (this->isStartRecord) {
		sqlSplite->insertRecord(this->getRecordId(), SQ(infParam.getNo()), getActivityId());
	}
	return infParam;
}

/**
* @brief MainWindow::checkFunction
* 输出参数 - this->picture;
* 输出参数 - 经过处理后的this->picture;
*/
void MainWindow::checkFunction()
{
	cv::Mat drawMat = faceStructParams.image.clone();
	std::vector<cv::Point2f> points;
	double simScore;
	std::string name;
	qDebug() << this->func << endl;
	if (this->func == 1) //检测
	{
		std::vector<bbox_t> boxes = faceDetector->faceDetectBoxes(faceStructParams.image);
		for (int i = 0; i < boxes.size(); i++) {
			struct FaceStructParam::FaceParam faceParam;
			faceParam.detectBox = boxes[i];
			faceStructParams.faceParams.push_back(faceParam);
			drawAllMessages(drawMat, boxes[i], points, simScore, name,false);
		}
		
	}
	else if (this->func == 2) //检测 + 识别
	{
		//		qDebug() << this->func << endl;
		//
		//		//DONE:检测 - 获取bbox_t
		//		//test
		//		/*bbox_t tmp;
		//		tmp.x = 1;
		//		tmp.y = 1;
		//		tmp.w = 108;
		//		tmp.h = 154;
		//		std::vector<bbox_t> tt;
		//		tt.push_back(tmp);
		//		imageAndParam.setVBox(tt);*/
		//		/*tmp.x = 1;
		//		tmp.y = 1;
		//		tmp.w = 185;
		//		tmp.h = 239;*/
		//
		//		/*tmp.x = 1;
		//		tmp.y = 1;
		//		tmp.w = 108;
		//		tmp.h = 154;*/
		//
		//		//imageAndParam.vbox.push_back(tmp);
		//		
		qDebug() << this->func << endl;
		std::vector<bbox_t> boxes = faceDetector->faceDetectBoxes(faceStructParams.image);
		bool hasRe;
		int index;
		qDebug() << "boxesNumber" << boxes.size();
		for (int i = 0; i < boxes.size(); i++) {
			struct FaceStructParam::FaceParam faceParam;
			cv::Mat detectImg;
			faceParam.detectBox = boxes[i];
			std::vector<cv::Point2f> landmarkPoints;
			// 关键点检测
			faceAlignment->landmarkFace(faceStructParams.image, boxes[i], landmarkPoints);
			faceAlignment->transformImgAndPoint(faceStructParams.image, boxes[i],imageSize,detectImg, landmarkPoints);
			// 赋值
			faceParam.landmarks = landmarkPoints;
			// 对齐
			cv::Mat alignFace;
			faceAlignment->alignFace(detectImg, landmarkPoints, alignFace);
			// 识别
			reInfParam = this->predictFace(alignFace, false, faceParam);

			faceStructParams.faceParams.push_back(faceParam);

			double sim = (double)faceParam.score;
			hasRe = false;
			if (sim >= simTheta) {
				hasRe = true;
				index = findNoIndex(reInfParam.getNo());
				cv::Mat srcImg = cv::imread(reInfParam.getPictureRoute());
				switch (index)
				{
				case 0:
					if (!detectImg.empty()) {
						setImage2Label(detectImg, subShow12);
					}
					if (!srcImg.empty() && sim >= simTheta) {
						//simiInfo1->setText(QString::number((sim)));
						simiInfo1->setText(SQ(reInfParam.getName()));
						setImage2Label(srcImg, subShow11);
					}
					else {
						simiInfo1->setText(SQ("数据库无此人"));
					}
					break;
				case 1:
					if (!detectImg.empty()) {
						setImage2Label(detectImg, subShow22);

					}
					if (!srcImg.empty() && sim >= simTheta) {
						setImage2Label(srcImg, subShow21);
						simiInfo2->setText(SQ(reInfParam.getName()));
					}
					else {
						simiInfo2->setText(SQ("数据库无此人"));
					}
					break;
				case 2:
					if (!detectImg.empty()) {
						setImage2Label(detectImg, subShow32);

					}
					else {
						simiInfo3->setText(SQ("数据库无此人"));
					}
					if (!srcImg.empty() && sim >= simTheta) {
						setImage2Label(srcImg, subShow31);
						simiInfo3->setText(SQ(reInfParam.getName()));
					}
					else {
						simiInfo3->setText(SQ("数据库无此人"));
					}
					break;
				case 3:
					if (!detectImg.empty()) {
						setImage2Label(detectImg, subShow42);

					}
					if (!srcImg.empty() && sim >= simTheta) {
						setImage2Label(srcImg, subShow41);
						simiInfo4->setText(SQ(reInfParam.getName()));
					}
					else {
						simiInfo4->setText(SQ("数据库无此人"));
					}
					break;
				default:
					break;
				}
			}
			drawAllMessages(drawMat, boxes[i], points, simScore, name, hasRe);
		}
	}
	faceStructParams.image = drawMat.clone();

}
void MainWindow::paintEvent(QPaintEvent *e)
{

	if (this->mode == 3 || this->mode == 2)//摄像头模式   ||  视频模式
	{
		if (capture.isOpened()) {
			cv::Mat capImg;
			capture >> capImg;
            this->faceStructParams.image = capImg.clone();//核心数据结构


			//auto start = std::chrono::steady_clock::now();

			checkFunction(); //根据现在的 “功能” 做出相应的动作

			//auto end = std::chrono::steady_clock::now();
			//std::chrono::duration<double> spent = end - start;

			//setImage2Label(imageAndParam.image, mainShow);
			setImage2Label(faceStructParams.image, mainShow);
		}
		/*mode = 4;*/
	}
}

/**
* @brief MainWindow::openPictureSlot - 用户注册
*/
void MainWindow::openRegisterSlot()
{
	if (capture.isOpened()) {
		capture.release();
	}
	RegisterWindow *registerWindow = new RegisterWindow(faceDetector,faceAlignment, faceRecognition);
    registerWindow->show();
}

void MainWindow::quitSlot()
{
	this->close();
}

/**
* @brief MainWindow::detFunctionSlot - 检测
*/
void MainWindow::detFunctionSlot()
{
	this->func = 1;
	functionMenu->setTitle(SQ("检测"));
}
/**
* @brief MainWindow::detAliRecFunctionSlot - 检测 + 对齐 + 识别
*/
void MainWindow::detAliRecFunctionSlot()
{
	this->func = 2;
	functionMenu->setTitle(SQ("检测+识别"));
}
bool MainWindow::readXmlData(cv::String xmlFileName) {
	cv::FileStorage fs(xmlFileName, cv::FileStorage::READ | cv::FileStorage::FORMAT_XML);;
	if (fs.isOpened()) {
		float simTheta;
		std::string videoStreamRtsp; 
		int useUSBCamera;
		int USBPort;
		int useGpuRecognize;
		float detectTheta;
		fs["videoStreamRtsp"] >> videoStreamRtsp;
		fs["useUSBCamera"] >> useUSBCamera;
		fs["USBPort"] >> USBPort;
		fs["useGpuRecognize"] >> useGpuRecognize;
		fs["simTheta"] >> simTheta;
		fs["detectTheta"] >> detectTheta;
#ifdef DEBUG_PR
		qDebug() << "videoStreamRtsp:" << SQ(videoStreamRtsp);
		qDebug() << "useUSBCamera:" << useUSBCamera;
		qDebug() << "USBPort:" << USBPort;
		qDebug() << "useGpuRecognize:" << useGpuRecognize;
		qDebug() << "simTheta:" << simTheta;
#endif // DEBUG_PR
		setVideoStreamRtsp(videoStreamRtsp);
		setUseGpuRecognize(useGpuRecognize);
		setUSBPort(USBPort);
		setUseUSBCamera(useUSBCamera);
		setSimTheta(simTheta);
		setDetectTheta(detectTheta);
	}
	else {
		QMessageBox::information(this, SQ("加载参数文件——param.xml"), SQ("参数文件加载失败，参数文件不存在或格式错误"), QMessageBox::Ok); // QObject::trUtf8()
	}
	
}
void MainWindow::addActSlot() {      // 添加活动事件
	QString activityName = QInputDialog::getText(this, SQ("活动"),
		SQ("输入活动名称:"));
	qDebug() << activityName;
	bool success = sqlSplite->insertActivity(activityName);
	if (success) {
		selectBox->clear();
		std::vector<QString> comBoxStr = sqlSplite->selectActivity();
		for (auto str : comBoxStr) {
			selectBox->addItem(str);
		}
	}
}
void MainWindow::startRecordSlot() {
	if (isStartRecord) {
		startRecordButton->setText(SQ("开始考勤"));
		this->isStartRecord = false;
	}
	else {
		startRecordButton->setText(SQ("结束考勤"));
		this->isStartRecord = true;
	}
}
void MainWindow::selectBoxIndex(int index) {
	qDebug() << index;
	qDebug() << this->selectBox->currentIndex();
	// 活动ID
	setActivityId(this->selectBox->currentIndex() + 1);
	// 记录跟新
	setRecordId(sqlSplite->getRecordId() + 1);
	qDebug() << "id:"<<getRecordId();
}
