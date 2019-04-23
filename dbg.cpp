#include <chrono>
#include <mutex>
#include <fstream>
#include <sstream>
#include "QSize"

#include "dbg.h"
#include "common.h"

using namespace std::chrono;

static const int LogBufSize = 256;

DbgLevel gDbgLevel = DbgLevelInfo;

bool gImageDbgShowOn = true;
bool gImageDbgWriteOn = true;
bool gDrawDbgOn = true;
bool gRecordLog = false;

static char gLogBuf[LogBufSize];
static std::mutex gLogMutex;
static std::ofstream gRecordFile;

void dbg(DbgLevel level, const char *file, int line, const char *fmt, ...)
{
	if (level >= gDbgLevel) {
		gLogMutex.lock();

		int len = snprintf(gLogBuf, LogBufSize, "<%s, %d>: ", file, line);
		va_list ap;
		va_start(ap, fmt);
		len += vsnprintf(gLogBuf + len, LogBufSize - len, fmt, ap);
		va_end(ap);

		printf("%s", gLogBuf);
		if (gRecordLog) {
			if (!gRecordFile.is_open()) {
				time_t time = system_clock::to_time_t(system_clock::now());
				std::stringstream ss;
				ss << "log_" << time << ".txt";
				gRecordFile.open(ss.str());
			}

			gRecordFile.write(gLogBuf, len);
			gRecordFile.flush();
		}

		gLogMutex.unlock();
	}
}

void imdbgShow(const std::string &name, const cv::Mat &img)
{
	if (gImageDbgShowOn) {
		cv::imshow(name, img);
	}
}

void imdbgWrite(const std::string &name, const cv::Mat &img)
{
	if (gImageDbgWriteOn) {
		cv::imwrite(name, img);
	}
}

void drawPoints(cv::Mat &image, const std::vector<cv::Point2f>  &points, const cv::Scalar &color)
{
	if (gDrawDbgOn) {
		for (std::vector<cv::Point2f>::const_iterator i = points.begin(), ie = points.end(); i != ie; ++i) {
			circle(image, *i, 2, color, 2);
		}
	}
}

void drawBox(cv::Mat &image, const cv::Rect &box, const cv::Scalar &color, int thick)
{
	if (gDrawDbgOn) {
		rectangle(image, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), color);
	}
}

void draw_boxes(cv::Mat mat_img, std::vector<bbox_t> result_vec, std::vector<std::string> obj_names, double spendTime)
{
	int const colors[6][3] = { { 1,0,1 },{ 0,0,1 },{ 0,1,1 },{ 0,1,0 },{ 1,1,0 },{ 1,0,0 } };

	for (auto &i : result_vec) {
		cv::Scalar color(0, 255, 0);
		cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), color, 2);
		std::string obj_name = "face";
		cv::Size const text_size = getTextSize(obj_name, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, 2, 0);
		int const max_width = (text_size.width > i.w + 2) ? text_size.width - 5 : (i.w + 2);
		cv::rectangle(mat_img, cv::Point2f(std::max((int)i.x - 1, 0), std::max((int)i.y - 30, 0)),
			cv::Point2f(std::min((int)i.x + max_width, mat_img.cols - 1), std::min((int)i.y, mat_img.rows - 1)),
			color, CV_FILLED, 8, 0);
		putText(mat_img, obj_name, cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);
	}
	std::string  spend = QS(QString::number(spendTime, 10, 4));
	putText(mat_img, "time:" + spend + "s", cv::Point2f(0, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);
/*	if (current_det_fps >= 0 && current_cap_fps >= 0) {
		std::string fps_str = "FPS detection: " + std::to_string(current_det_fps) + "   FPS capture: " + std::to_string(current_cap_fps);
		putText(mat_img, fps_str, cv::Point2f(10, 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(50, 255, 0), 2);
	}**/
	/*int const colors[6][3] = { { 1,0,1 },{ 0,0,1 },{ 0,1,1 },{ 0,1,0 },{ 1,1,0 },{ 1,0,0 } };

	for (auto &i : result_vec) {
	cv::Scalar color = obj_id_to_color(i.obj_id);
	cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), color, 2);
	if (obj_names.size() > i.obj_id) {
	std::string obj_name = obj_names[i.obj_id];
	if (i.track_id > 0) obj_name += " - " + std::to_string(i.track_id);
	cv::Size const text_size = getTextSize(obj_name, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, 2, 0);
	int const max_width = (text_size.width > i.w + 2) ? text_size.width : (i.w + 2);
	cv::rectangle(mat_img, cv::Point2f(std::max((int)i.x - 1, 0), std::max((int)i.y - 30, 0)),
	cv::Point2f(std::min((int)i.x + max_width, mat_img.cols - 1), std::min((int)i.y, mat_img.rows - 1)),
	color, CV_FILLED, 8, 0);
	putText(mat_img, obj_name, cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);
	}
	}
	if (current_det_fps >= 0 && current_cap_fps >= 0) {
	std::string fps_str = "FPS detection: " + std::to_string(current_det_fps) + "   FPS capture: " + std::to_string(current_cap_fps);
	putText(mat_img, fps_str, cv::Point2f(10, 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(50, 255, 0), 2);
	}*/
}

/**
* @brief setImage2Label    在label上显示图片
* @param image             图片
* @param qLabel            label
*/
void setImage2Label(const cv::Mat &image, QLabel *qLabel)
{
	cv::Mat showImg;
	cv::cvtColor(image, showImg, CV_BGR2RGB); //因为Mat的图像数据是BGR,所以要先将img转化位RGB
	QImage qImg = QImage((const uchar *)(showImg.data),
		showImg.cols, showImg.rows,
		showImg.cols * showImg.channels(),
		QImage::Format_RGB888);
	qLabel->clear();                                //清空控件label
	qLabel->setFrameShape(QFrame::NoFrame);
	qLabel->setAlignment(Qt::AlignCenter);
	qLabel->setPixmap(QPixmap::fromImage(qImg));    //label赋值
	qLabel->show();

	// qLabel->clear();
	// qLabel->setFrameShape(QFrame::NoFrame);
	//
	// cv::cvtColor(image, showImg, CV_BGR2RGB);
	// QImage qImg = QImage((const uchar *)(showImg.data),
	//                      showImg.cols, showImg.rows,
	//                      showImg.cols * showImg.channels(),
	//                      QImage::Format_RGB888);
	// QPixmap *pixmap = new QPixmap(QPixmap::fromImage(qImg));
	// QPixmap qScaledImage;
	// QSize qImageSize = pixmap->size();
	// QSize qLabelSize = qLabel->size();
	// double dWidthRatio = 1.0 * qImageSize.width() / qLabelSize.width();
	// double dHeightRatio = 1.0 * qImageSize.height() / qLabelSize.height();
	///* if (dWidthRatio > dHeightRatio) {
	//     qScaledImage = pixmap->scaledToWidth(qLabelSize.width());
	//     qScaledImage = pixmap->scaledToHeight(qImageSize.height() / dWidthRatio);
	// } else {
	//     qScaledImage = pixmap->scaledToHeight(qLabelSize.height());
	//     qScaledImage = pixmap->scaledToWidth(qImageSize.width() / dHeightRatio);
	// }*/
	// qScaledImage = pixmap->scaledToWidth(qLabelSize.width());
	// qScaledImage = pixmap->scaledToHeight(qLabelSize.height());

	// qLabel->setPixmap(qScaledImage);
	// qLabel->setAlignment(Qt::AlignLeft);
	qLabel->update();
}
void drawAllMessages(cv::Mat &image, bbox_t bbox, const std::vector<cv::Point2f> points, double simScore, std::string name, bool hasRe){
	int lenW = bbox.w / 4;
	int lenH = bbox.h / 4;
	int x2 = bbox.x + bbox.w;
	int y2 = bbox.y + bbox.h;
	cv::Point left1x(bbox.x, bbox.y);
	cv::Point left1y(bbox.x + lenW , bbox.y);
	cv::Point left2x(bbox.x, bbox.y);
	cv::Point left2y(bbox.x, bbox.y+lenH);

	cv::Point left3x(bbox.x, y2 - lenW);
	cv::Point left3y(bbox.x, y2);
	cv::Point left4x(bbox.x, y2);
	cv::Point left4y(bbox.x + lenH, y2);


	cv::Point right1x(x2 - lenW, bbox.y);
	cv::Point right1y(x2, bbox.y );
	cv::Point right2x(x2, bbox.y);
	cv::Point right2y(x2, bbox.y + lenH);

	cv::Point right3x(x2, y2-lenW);
	cv::Point right3y(x2, y2);
	cv::Point right4x(x2 - lenW,y2 );
	cv::Point right4y(x2, y2);
	cv::Scalar scalar;
	if (hasRe) {
		scalar = cv::Scalar(0, 255, 0);
	}
	else {
		scalar  = cv::Scalar(0, 0, 255);
	}
	cv::line(image, left1x,left1y, scalar, 2);
	cv::line(image, left2x, left2y, scalar, 2);
	cv::line(image, left3x, left3y, scalar, 2);
	cv::line(image, left4x, left4y, scalar, 2);
	cv::line(image, right1x, right1y, scalar, 2);
	cv::line(image, right2x, right2y, scalar, 2);
	cv::line(image, right3x, right3y, scalar, 2);
	cv::line(image, right4x, right4y, scalar, 2);


}
