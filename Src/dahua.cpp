﻿#include "Dahua.h"
#include "ui_cammerwidget.h"
#include "GenICam/System.h"
#include "GenICam/Camera.h"
#include "GenICam/StreamSource.h"
#include <qdebug.h>
#include <qmetatype.h>
#include <qtimeline.h>

#define DEFAULT_SHOW_RATE (30)
#define TIMESTAMPFREQUENCY 125000000	//大华相机的时间戳频率固定为125,000,000Hz

using namespace Dahua::GenICam;
using namespace Dahua::Infra;

CammerWidget::CammerWidget(QWidget *parent) :
    QWidget(parent)
    ,ui(new Ui::CammerWidget)
	, m_thdDisplayThread(CThreadLite::ThreadProc(&CammerWidget::DisplayThreadProc, this), "Display")
	, m_nDisplayInterval(0)
	, m_nFirstFrameTime(0)
	, m_nLastFrameTime(0)
	, m_handler(NULL)
{
    ui->setupUi(this);
	m_hWnd = (VR_HWND)this->winId();  

	// 默认显示30帧
	setDisplayFPS(30);

	m_elapsedTimer.start();

	// 启动显示线程
	if (!m_thdDisplayThread.isThreadOver())
	{
		m_thdDisplayThread.destroyThread();
	}

	if (!m_thdDisplayThread.createThread())
	{
        //MessageBoxA(NULL, "Create Display Thread Failed.", "", 0);
		//return FALSE;
	}
}

CammerWidget::~CammerWidget()
{
    delete ui;
}

/*******************************************************以下部分为修改添加部分***************************************************/
// 显示
bool CammerWidget::ShowImage(uint8_t* pRgbFrameBuf, int pRgbFrameBufSize, int nWidth, int nHeight, uint64_t pixelFormat)
{
	QImage image;
	if (NULL == pRgbFrameBuf ||
		nWidth == 0 ||
		nHeight == 0)
	{
		printf("%s image is invalid.\n", __FUNCTION__);
		return false;
	}
	if (Dahua::GenICam::gvspPixelMono8 == pixelFormat)
	{
		image = QImage(pRgbFrameBuf ,nWidth, nHeight, QImage::Format_Indexed8);
	}
	else
	{
		image = QImage(pRgbFrameBuf,nWidth, nHeight, QImage::Format_RGB888);
	}

	//显示整幅图片
	QPixmap pixmap = QPixmap::fromImage(image);
	ui->label->setPixmap(pixmap);

	//显示裁剪后并且识别后的图片
	if (true == Mode_of_trig) {
		try
		{
			Mat img = QImage2cvMat(image);
			img = img(Rect(89, 897, 2633, 521)); //裁剪
			cv::Mat out;
			cv::Mat in[] = { img, img, img };
			cv::merge(in, 3, out);
			ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(out)));
			//QMessageBox::information(NULL, "img channels", QString::number(out.channels()));
			if (!bookdetection(out))//识别判断
			{
				//QMessageBox::information(NULL, "Wrong", "Book order is wrong!");
				ui->textEdit->append("Wrong");
			}
			else {
				ui->textEdit->setText("Correct!");
			}
		}
		catch (const std::exception& e)
		{
			QMessageBox::information(NULL, "识别部分出错", e.what());
			return true;
		}		
	}
	return true;
}

bool sortFun(Rect p1, Rect p2);
bool CammerWidget::bookdetection(Mat imagefile) {
	//outfile = "F:\\图书\\20191214\\20191217\\" + get_datetime() + ".bmp";
	string outfile = "E:\\pic\\label\\" + get_datetime() + ".bmp";
	String modelConfiguration = "D:/yolov3.cfg";
	/*String model_label_Weights = "D:/yolov3-voc_last1024.weights";*/
	String model_label_Weights = "D:/yolov3-voc_9000.weights";

	vector<Rect> boxes = detect_image(imagefile, model_label_Weights, modelConfiguration);
	//排序，根据得到的方框的中点的纵坐标进行排序，按照y从小到大的顺序排
	sort(boxes.begin(), boxes.end(), sortFun);
	vector<Point> points;
	int max_point_x = 0, max_point_y = 0;
	double average_piexl_value = 0;

	//for循环获取每个黑块的中点，并存储到points中
	for (int i = 0; i < boxes.size(); i++) {
		points.push_back(Point(boxes[i].x + 0.5*boxes[i].width, boxes[i].y + 0.5*boxes[i].height));
		average_piexl_value = average_piexl_value + imagefile.at<uchar>(points[i]);
		//if (boxes[i].x > max_point_x) {
		//	max_point_x = boxes[i].x;
		//	max_point_y = boxes[i].y;
		//}
		//if (i > 0)
		//	cout << "Rect(" << boxes[i].x << ", " << boxes[i].y << ", " << boxes[i].width << ", " << boxes[i].height << ")" << "Mid-points " << ":" << points[i].x << "," << points[i].y << " 两点间距：" << points[i - 1].x - points[i].x << ", " << points[i].y - points[i - 1].y << endl;
		//else
		//	cout << "Rect(" << boxes[i].x << ", " << boxes[i].y << ", " << boxes[i].width << ", " << boxes[i].height << ")" << "Mid-points " << ":" << points[i].x << "," << points[i].y << endl;
		//cout << "Mid-points " << i << "," << points[i].x << "," << points[i].y << endl;
		//cout << i << ": boxes[i].x " << boxes[i].x << " boxes[i].y " << boxes[i].y << " boxes[i].width " << boxes[i].width << " boxes[i].height " << boxes[i].height << endl;
	}
	//if (boxes.size() < block_nums) {
	//	int this_boxes_size = boxes.size();
	//	//计算各中点的相对位置
	//	for (int i = 0; i <= points.size(); i++) {
	//		//如果当前框的中点与下一个框的中点的相对位置的y相差＜5，则认为此框的位置正确
	//		if (abs(abs(points[i].y - points[i + 1].y) - relative_locations[i].y) <= 10) {
	//			cout << "Correct point" << i << endl;
	//			continue;
	//		}
	//		//否则，有误，是否是缺失？
	//		else
	//		{
	//			//;如果是缺失，则补一个框。
	//			//找到下一个框的左上角位置,x是从大到小，y是从小到大
	//			int x_1 = points[i].x - relative_locations[i].x - 0.5*locations[i + 1].width;
	//			int y_1 = points[i].y + relative_locations[i].y - 0.5*locations[i + 1].height;
	//			Rect r = Rect(x_1, y_1, locations[i + 1].width, locations[i + 1].height);
	//			average_piexl_value = average_piexl_value / boxes.size() + 10;
	//			Mat imagefile1 = imagefile(r);
	//			double value = 0;
	//			for (int i = 0; i < imagefile1.rows; ++i)
	//				for (int j = 0; j < imagefile1.cols; ++j)
	//					value += imagefile1.at<uchar>(i, j);
	//			value = value / (imagefile1.cols*imagefile1.rows);
	//			if (value < average_piexl_value)
	//			{
	//				this_boxes_size++;
	//				rectangle(imagefile, r, Scalar(255, 178, 50), 2);
	//				cout << "缺失框位置像素正确" << endl;
	//				imwrite(outfile, imagefile);
	//				cout << "文件写入：" + outfile << endl << endl;
	//				if (this_boxes_size < points.size()) 
	//					continue;
	//				else
	//					return 1;
	//			}
	//			else
	//				//cout << "缺失框位置像素有误!" << endl;
	//				continue;
	//				//return false;				
	//		}
	//	}
	//	return 1;
	//}
	//前十个用来自适应调整截距斜率跟相关系数参数
	//if (total_number < 11) {
	//	//v_block_locations.push_back((boxes));
	//}
	//if (total_number == 10) {
	//	double sum_k = 0, sum_b = 0, sum_r_q = 0;
	//	for (int i = 0; i < 11; i++) {
	//		sum_b += v_b[i];
	//		sum_k += v_k[i];
	//		sum_r_q += v_r_q[i];
	//	}
	//	cout << "sum_b: " << sum_b << " sum_k：" << sum_k << " sum_r_q: " << sum_r_q << endl;
	//	k = sum_k / 10;
	//	b = sum_b / 10;
	//	r_q = sum_r_q / 10;
	//	cout << "k: " << k << " b: " << b << " r_q: " << r_q;
	//}
	//前十个用来自适应调整截距斜率跟相关系数参数
	//if (boxes.size() == block_nums && total_number < 11) {
	//	LinearFitting(points, 0, 0, 0);
	//}

	//cout << "本书黑色标记数量： " << boxes.size() << endl;
	int this_block_nums = boxes.size();
	int val = 0;
	//if (this_block_nums > block_nums) {
	//	for (int i = 0; i < boxes.size(); i++) {
	//		//if (boxes[i].width > 100) {
	//		//	boxes.erase(boxes.begin() + i);
	//		//}
	//	}
	//	//if (boxes.size() == block_nums) {
	//	//	return 1;
	//	//}
	//}	
	//if (boxes.size() < block_nums) {
	//	//计算各中点的相对位置
	//	for (int i = 0; i <= points.size(); i++) {
	//		//如果当前框的中点与下一个框的中点的相对位置的y相差＜5，则认为此框的位置正确
	//		if (abs(abs(points[i].y - points[i + 1].y) - relative_locations[i].y) <= 10) {
	//			cout << "Correct point" << i << endl;
	//			continue;
	//		}
	//		//否则，有误，是否是缺失？
	//		else
	//		{
	//			//;如果是缺失，则补一个框。
	//			////找到下一个框的左上角位置,x是从大到小，y是从小到大
	//			//int x_1 = points[i].x - relative_locations[i].x - 0.5*locations[i + 1].width;
	//			//int y_1 = points[i].y + relative_locations[i].y - 0.5*locations[i + 1].height;
	//			//按照y从小到大的顺序排序
	//			int x_1 = points[i].x - relative_locations[i].x - 0.5*locations[i + 1].width + 0.5*locations[i].width;
	//			int y_1 = points[i].y + relative_locations[i].y - 0.5*locations[i + 1].height + 0.5*locations[i].height;
	//			Rect r = Rect(x_1, y_1, locations[i + 1].width, locations[i + 1].height);
	//			average_piexl_value = average_piexl_value / boxes.size() + 10;
	//			Mat imagefile1 = imagefile(r);
	//			double value = 0;
	//			//循环遍历补得黑框中的所有像素计算平局灰度值
	//			for (int i = 0; i < imagefile1.rows; ++i) for (int j = 0; j < imagefile1.cols; ++j) value += imagefile1.at<uchar>(i, j);
	//			value = value / (imagefile1.cols*imagefile1.rows);
	//			if (value < average_piexl_value)
	//			{  
	//				rectangle(imagefile, r, Scalar(255, 108, 50), 2);
	//				cout << "缺失框位置像素正确" << endl;
	//				boxes.push_back(r);
	//				continue;
	//			}
	//			else
	//			{
	//				cout << "缺失框位置像素有误!" << endl;
	//				continue;
	//			}
	//		}
	//	}			
	//}
	ui->textEdit->append("this_block_nums is " + boxes.size());
	switch (this_block_nums)
	{
	case 11:
		cout << "黑色标志点数量正确" << endl;
		imwrite(outfile, imagefile);
		cout << "文件写入：" + outfile << endl;
		return LinearFitting(points, -0.34, 0, 0.98);
	case 12:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED);
		cout << "黑色标志点数为12" << endl;
		outfile = "E:\\pic\\label\\12-" + get_datetime() + ".bmp";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		imwrite(outfile, imagefile);
		cout << "文件写入：" + outfile << endl;
		return LinearFitting(points, -0.34, 0, 0.98);
	case 10:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED);
		cout << "黑色标志点数为10" << endl;
		outfile = "E:\\pic\\label\\少" + get_datetime() + ".bmp";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		imwrite(outfile, imagefile);
		cout << "文件写入：" + outfile << endl;
		return LinearFitting(points, -0.34, 0, 0.98) && 0;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "黑色标志数量错误：  " << this_block_nums << endl;
		outfile = "E:\\pic\\label\\有误" + get_datetime() + ".bmp";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		imwrite(outfile, imagefile);
		cout << "文件写入：" + outfile << endl;
		return 0;
	}

	//if (this_block_nums == block_nums)
	//	cout << "黑色标志点数量正确" << endl;
	//else if (this_block_nums == 12) {
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED);
	//	cout << "黑色标志点数为12" << endl;
	//	outfile = "D:\\20200110\\label\\9000\\12-" + get_datetime() + ".bmp";
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//}
	//else
	//{
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	if (boxes.size() > block_nums) {
	//		cout << "黑色标志多：  " << boxes.size() << "\t" << block_nums << endl;
	//		outfile = "D:\\20200110\\label\\9000\\多" + get_datetime() + ".bmp";
	//	}
	//	else
	//	{
	//		if (boxes.size() < block_nums)
	//		{
	//			cout << "黑色标志少：  " << boxes.size() << "\t" << block_nums << endl;
	//			outfile = "D:\\20200110\\label\\9000\\少" + get_datetime() + ".bmp";
	//		}
	//	}
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//}
	//if (boxes.size() == block_nums) {
	//	return LinearFitting(points, 0, 0, 0); //检测是否乱序
	//}
	//else if (boxes.size() > block_nums) {
	//	cout << "Error" << endl;
	//	return false;
	//}
	//else if (boxes.size() < block_nums)
	//{
	//	for (int i = 0; i < block_nums; i++) {
	//		if (iscontained(templateRect[i], points[i]))
	//			continue;
	//		else
	//		{
	//			average_piexl_value = average_piexl_value / boxes.size() + 10;
	//			imagefile = imagefile(templateRect[i]);
	//			double value = 0;
	//			for (int i = 0; i < imagefile.rows; ++i)
	//				for (int j = 0; j < imagefile.cols; ++j)
	//					value += imagefile.at<uchar>(i, j);
	//			value = value / (imagefile.cols*imagefile.rows);
	//			if (value < average_piexl_value)
	//			{
	//				cout << "Normal" << endl;
	//				return true;
	//			}
	//			else
	//			{
	//				cout << "Abnormal!" << endl;
	//				return false;					
	//			}
	//		}
	//	}		
	//}
	return true;
}

//OpenCV Mat与QImage相互转换函数
Mat CammerWidget::QImage2cvMat(QImage &image, bool clone, bool rb_swap)
{
	cv::Mat mat;
	//qDebug() << image.format();
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void *)image.constBits(), image.bytesPerLine());
		if (clone)  mat = mat.clone();
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void *)image.constBits(), image.bytesPerLine());
		if (clone)  mat = mat.clone();
		if (rb_swap) cv::cvtColor(mat, mat, CV_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
	case QImage::Format_Grayscale8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void *)image.bits(), image.bytesPerLine());
		if (clone)  mat = mat.clone();
		break;
	}
	return mat;
}

QImage CammerWidget::cvMat2QImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		return QImage();
	}
}

bool CammerWidget::LinearFitting(const vector<Point> points, double slope, double intercept, double r_square)
{
	int length = points.size();
	double xmean = 0.0;
	double ymean = 0.0;
	for (int i = 0; i < length; i++)
	{
		xmean += points[i].x;
		ymean += points[i].y;
	}
	xmean /= length;
	ymean /= length;

	double sumx2 = 0.0;
	double sumy2 = 0.0;
	double sumxy = 0.0;
	for (int i = 0; i < length; i++)
	{
		sumx2 += (points[i].x - xmean) * (points[i].x - xmean);
		sumy2 += (points[i].y - ymean) * (points[i].y - ymean);
		sumxy += (points[i].y - ymean) * (points[i].x - xmean);
	}
	double slope1 = sumxy / sumx2;  //斜率
	double intercept1 = ymean - slope * xmean; //截距
	double r_square1 = sumxy * sumxy / (sumx2 * sumy2); //相关系数
	cout << "y = " << slope1 << "x+ " << intercept1 << "  r_square1 is " << r_square1 << endl;

	if (abs(r_square - r_square1) <= 0.1 && abs(slope - slope1) <= 0.1) {
		cout << "相关系数正确" << endl;

		QString str = "y=" + QString::number(slope) + "x+ " + QString::number(intercept1) + "  r_square1 is " + QString::number(r_square1) + "Correct";
		ui->label_3->setText(str);
		return true;
	}
	else
	{

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | BACKGROUND_RED);
		cout << "相关系数错误！" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		QString str = "y=" + QString::number(slope) + "x+ " + QString::number(intercept1) + "  r_square1 is " + QString::number(r_square1) + "Wrong";
		ui->label_3->setText(str);
		return false;
	}

}

string CammerWidget::get_datetime()
{
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);

	char temp[100];
	sprintf_s(temp, "%d-%02d-%02d-%02d-%02d-%02d",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond
	);
	std::string params = string(temp);
	return params;
}

vector<Rect> CammerWidget::detect_image(Mat frame, string modelWeights, string modelConfiguration) {
	// Load the network
	Net net = readNetFromDarknet(modelConfiguration, modelWeights);
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	//net.setPreferableTarget(DNN_TARGET_OPENCL);
	net.setPreferableTarget(DNN_TARGET_CPU);

	Mat blob;
	blobFromImage(frame, blob, 1 / 255.0, cvSize(inpWidth, inpHeight), Scalar(0, 0, 0), true, false);

	//Sets the input to the network
	net.setInput(blob);

	// Runs the forward pass to get output of the output layers
	vector<Mat> outs;
	net.forward(outs, getOutputsNames(net));

	// Remove the bounding boxes with low confidence
	vector<Rect> boxes = postprocess_return(frame, outs);
	// Put efficiency information. The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
	vector<double> layersTimes;
	double freq = getTickFrequency() / 1000;
	double t = net.getPerfProfile(layersTimes) / freq;
	string label = format("Inference time for a frame : %.2f ms", t);
	//cout << label<<endl;
	//putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
	// Write the frame with the detection boxes
	//imshow(kWinName, frame);
	//imwrite(classesFile, frame);
	return boxes;
	//cv::waitKey(30);
}

bool sortFun(Rect p1, Rect p2) {
	return p1.y + 0.5*p1.height < p2.y + 0.5* p2.height;
}

vector<Rect> CammerWidget::postprocess_return(Mat& frame, const vector<Mat>& outs)
{
	vector<int> classIds;
	vector<float> confidences;
	vector<Rect> boxes;
#pragma omp parallel for
	for (int i = 0; i < outs.size(); ++i)
	{
		// Scan through all the bounding boxes output from the network and keep only the
		// ones with high confidence scores. Assign the box's class label as the class
		// with the highest score for the box.
		float* data = (float*)outs[i].data;
		for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
		{
			Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
			Point classIdPoint;
			double confidence;
			// Get the value and location of the maximum score
			minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
			if (confidence > confThreshold)
			{
				int centerX = (int)(data[0] * frame.cols);
				int centerY = (int)(data[1] * frame.rows);
				int width = (int)(data[2] * frame.cols);
				int height = (int)(data[3] * frame.rows);
				int left = centerX - width / 2;
				int top = centerY - height / 2;

				classIds.push_back(classIdPoint.x);
				confidences.push_back((float)confidence);
				boxes.push_back(Rect(left, top, width, height));
			}
		}
	}

	// Perform non maximum suppression to eliminate redundant overlapping boxes with
	// lower confidences
	vector<int> indices;
	NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
	//cout << "indices.size(匹配得到的目标数):" << indices.size() << endl;
	vector<Rect> boxes_for_return;

	Rect box = boxes[0];

	for (size_t i = 0; i < indices.size(); ++i)
	{
		int idx = indices[i];
		Rect box = boxes[idx];
		if (box.x < 0)
		{
			box.width = box.width + box.x;
			box.x = 0;
		}
		if (box.width > frame.cols) {
			box.width = frame.cols - box.x;
		}
		if (box.x + box.width > frame.cols) {
			box.width = frame.cols - box.x;
		}
		boxes_for_return.push_back(box);
		//drawPred(classIds[idx], confidences[idx], box.x, box.y, box.x + box.width, box.y + box.height, frame);
		//画框
		rectangle(frame, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), Scalar(255, 178, 50), 2);
	}

	return boxes_for_return;
}

vector<String> CammerWidget::getOutputsNames(const Net& net)
{
	static vector<String> names;
	if (names.empty())
	{
		//Get the indices of the output layers, i.e. the layers with unconnected outputs
		vector<int> outLayers = net.getUnconnectedOutLayers();

		//get the names of all the layers in the network
		vector<String> layersNames = net.getLayerNames();

		// Get the names of the output layers in names
		names.resize(outLayers.size());
		for (size_t i = 0; i < outLayers.size(); ++i)
			names[i] = layersNames[outLayers[i] - 1];
	}
	return names;
}


/*******************************************************以上部分为添加部分***************************************************/


//回调函数
void CammerWidget::DahuaCallback(const CFrame& frame)
{
	CFrameInfo frameInfo;
	frameInfo.m_nWidth = frame.getImageWidth();
	frameInfo.m_nHeight = frame.getImageHeight();
	frameInfo.m_nBufferSize = frame.getImageSize();
	frameInfo.m_nPaddingX = frame.getImagePadddingX();
	frameInfo.m_nPaddingY = frame.getImagePadddingY();
	frameInfo.m_PixelType = frame.getImagePixelFormat();
	frameInfo.m_pImageBuf = (BYTE *)malloc(sizeof(BYTE)* frameInfo.m_nBufferSize);
	frameInfo.m_nTimeStamp = frame.getImageTimeStamp();

	/* 内存申请失败，直接返回 */
	if (frameInfo.m_pImageBuf != NULL)
	{
		memcpy(frameInfo.m_pImageBuf, frame.getImage(), frame.getImageSize());

		if (m_qDisplayFrameQueue.size() > 16)
		{
			CFrameInfo frameOld;
			m_qDisplayFrameQueue.get(frameOld);
			free(frameOld.m_pImageBuf);
		}

		m_qDisplayFrameQueue.push_back(frameInfo);
	}
}

//设置曝光
bool CammerWidget::SetExposeTime(double exposureTime)
{
	if (NULL == m_pCamera)
	{
		printf("Set ExposureTime fail. No camera or camera is not connected.\n");
		return false;
	}

	CDoubleNode nodeExposureTime(m_pCamera, "ExposureTime");

	if (false == nodeExposureTime.isValid())
	{
		printf("get ExposureTime node fail.\n");
		return false;
	}

	if (false == nodeExposureTime.isAvailable())
	{
		printf("ExposureTime is not available.\n");
		return false;
	}

	if (false == nodeExposureTime.setValue(exposureTime))
	{
		printf("set ExposureTime value = %f fail.\n", exposureTime);
		return false;
	}

	return true;
}

//设置增益
bool CammerWidget::SetAdjustPlus(double gainRaw)
{
	if (NULL == m_pCamera)
	{
		printf("Set GainRaw fail. No camera or camera is not connected.\n");
		return false;
	}

	CDoubleNode nodeGainRaw(m_pCamera, "GainRaw");

	if (false == nodeGainRaw.isValid())
	{
		printf("get GainRaw node fail.\n");
		return false;
	}

	if (false == nodeGainRaw.isAvailable())
	{
		printf("GainRaw is not available.\n");
		return false;
	}

	if (false == nodeGainRaw.setValue(gainRaw))
	{
		printf("set GainRaw value = %f fail.\n", gainRaw);
		return false;
	}

	return true;
}

//打开相机
bool CammerWidget::CameraOpen(void)
{
	if (NULL == m_pCamera)
	{
		printf("connect camera fail. No camera.\n");
		return false;
	}

	if (true == m_pCamera->isConnected())
	{
		printf("camera is already connected.\n");
		return false;
	}

	if (false == m_pCamera->connect())
	{
		printf("connect camera fail.\n");
		return false;
	}

	return true;
}

//关闭相机
bool CammerWidget::CameraClose(void)
{
	if (NULL == m_pCamera)
	{
		printf("disconnect camera fail. No camera.\n");
		return false;
	}

	if (false == m_pCamera->isConnected())
	{
		printf("camera is already disconnected.\n");
		return false;
	}

	if (false == m_pCamera->disConnect())
	{
		printf("disconnect camera fail.\n");
		return false;
	}

	return true;
}

//开始采集
bool CammerWidget::CameraStart()
{
	if (m_pStreamSource != NULL)
		return true;

	m_pStreamSource = CSystem::getInstance().createStreamSource(m_pCamera);
	if (NULL == m_pStreamSource)
	{
		MessageBoxA(NULL, "Get Stream Failed.", "", 0);
		return false;
	}

	bool isSuccess = m_pStreamSource->attachGrabbing(IStreamSource::Proc(&CammerWidget::DahuaCallback, this));
	if (!isSuccess)
	{
		return false;
	}

	if (!m_pStreamSource->startGrabbing())
	{
		m_pStreamSource.reset();
		MessageBoxA(NULL, "Start Stream Grabbing Failed.", "", 0);
		return false;
	}

	return true;
}

//停止采集
bool CammerWidget::CameraStop()
{
	if (m_pStreamSource == NULL)
		return true;

	bool isSuccess = m_pStreamSource->detachGrabbing(IStreamSource::Proc(&CammerWidget::DahuaCallback, this));
	if (!isSuccess)
	{
		return false;
	}

	m_pStreamSource->stopGrabbing();
	m_pStreamSource.reset();

	/* 清空显示队列 */
	//CGuard guard(m_mutexQue);
	m_qDisplayFrameQueue.clear();

	return true;
}

//切换采集方式、触发方式 （连续采集、外部触发、软件触发）
void CammerWidget::CameraChangeTrig(ETrigType trigType)
{
	if (NULL == m_pCamera)
	{
		printf("Change Trig fail. No camera or camera is not connected.\n");
		return;
	}

	if (trigContinous == trigType)
	{
		//设置触发模式
		CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
		if (false == nodeTriggerMode.isValid())
		{
			printf("get TriggerMode node fail.\n");
			return;
		}
		if (false == nodeTriggerMode.setValueBySymbol("Off"))
		{
			printf("set TriggerMode value = Off fail.\n");
			return;
		}
		Mode_of_trig = false;
	}
	else if (trigSoftware == trigType)
	{
		//设置触发源为软触发
		CEnumNode nodeTriggerSource(m_pCamera, "TriggerSource");
		if (false == nodeTriggerSource.isValid())
		{
			printf("get TriggerSource node fail.\n");
			return;
		}
		if (false == nodeTriggerSource.setValueBySymbol("Software"))
		{
			printf("set TriggerSource value = Software fail.\n");
			return;
		}

		//设置触发器
		CEnumNode nodeTriggerSelector(m_pCamera, "TriggerSelector");
		if (false == nodeTriggerSelector.isValid())
		{
			printf("get TriggerSelector node fail.\n");
			return;
		}
		if (false == nodeTriggerSelector.setValueBySymbol("FrameStart"))
		{
			printf("set TriggerSelector value = FrameStart fail.\n");
			return;
		}

		//设置触发模式
		CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
		if (false == nodeTriggerMode.isValid())
		{
			printf("get TriggerMode node fail.\n");
			return;
		}
		if (false == nodeTriggerMode.setValueBySymbol("On"))
		{
			printf("set TriggerMode value = On fail.\n");
			return;
		}
		Mode_of_trig = false;
	}
	else if (trigLine == trigType)
	{
		//设置触发源为Line1触发
		CEnumNode nodeTriggerSource(m_pCamera, "TriggerSource");
		if (false == nodeTriggerSource.isValid())
		{
			printf("get TriggerSource node fail.\n");
			return;
		}
		if (false == nodeTriggerSource.setValueBySymbol("Line1"))
		{
			printf("set TriggerSource value = Line1 fail.\n");
			return;
		}

		//设置触发器
		CEnumNode nodeTriggerSelector(m_pCamera, "TriggerSelector");
		if (false == nodeTriggerSelector.isValid())
		{
			printf("get TriggerSelector node fail.\n");
			return;
		}
		if (false == nodeTriggerSelector.setValueBySymbol("FrameStart"))
		{
			printf("set TriggerSelector value = FrameStart fail.\n");
			return;
		}

		//设置触发模式
		CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
		if (false == nodeTriggerMode.isValid())
		{
			printf("get TriggerMode node fail.\n");
			return;
		}
		if (false == nodeTriggerMode.setValueBySymbol("On"))
		{
			printf("set TriggerMode value = On fail.\n");
			return;
		}

		// 设置外触发为上升沿（下降沿为FallingEdge）
		CEnumNode nodeTriggerActivation(m_pCamera, "TriggerActivation");
		if (false == nodeTriggerActivation.isValid())
		{
			printf("get TriggerActivation node fail.\n");
			return;
		}
		if (false == nodeTriggerActivation.setValueBySymbol("RisingEdge"))
		{
			printf("set TriggerActivation value = RisingEdge fail.\n");
			return;
		}
		Mode_of_trig = true;
	}
}

//执行一次软触发
void CammerWidget::ExecuteSoftTrig(void)
{
	if (NULL == m_pCamera)
	{
		printf("Set GainRaw fail. No camera or camera is not connected.\n");
		return;
	}

	CCmdNode nodeTriggerSoftware(m_pCamera, "TriggerSoftware");
	if (false == nodeTriggerSoftware.isValid())
	{
		printf("get TriggerSoftware node fail.\n");
		return;
	}
	if (false == nodeTriggerSoftware.execute())
	{
		printf("set TriggerSoftware fail.\n");
		return;
	}

	//等待完成一次软触发
	Sleep(100);

	printf("ExecuteSoftTrig success.\n");
}

//检测像机数、序列号
void CammerWidget::CameraCheck(void)
{
	CSystem &systemObj = CSystem::getInstance();

	bool bRet = systemObj.discovery(m_vCameraPtrList);
	if (false == bRet)
	{
		printf("discovery fail.\n");
		return;
	}

	// 打印相机基本信息（key, 制造商信息, 型号, 序列号）
	for (int i = 0; i < m_vCameraPtrList.size(); i++)
	{
		ICameraPtr cameraSptr = m_vCameraPtrList[i];

		printf("Camera[%d] Info :\n", i);
		printf("    key           = [%s]\n", cameraSptr->getKey());
		printf("    vendor name   = [%s]\n", cameraSptr->getVendorName());
		printf("    model         = [%s]\n", cameraSptr->getModelName());
		printf("    serial number = [%s]\n", cameraSptr->getSerialNumber());
	}

	if (m_vCameraPtrList.size() < 1)
	{
		printf("no camera.\n");
	}
	else
	{
		//默认设置列表中的第一个相机为当前相机，其他操作比如打开、关闭、修改曝光都是针对这个相机。
		m_pCamera = m_vCameraPtrList[0];
	}
}

// 显示线程
void CammerWidget::DisplayThreadProc(Dahua::Infra::CThreadLite& lite)
{
	while (lite.looping())
	{
		CFrameInfo frameInfo;

		if (false == m_qDisplayFrameQueue.get(frameInfo, 500))
		{
			continue;
		}

		// 判断是否要显示。超过显示上限（30帧），就不做转码、显示处理
		if (!isTimeToDisplay(frameInfo.m_nTimeStamp))
		{
			/* 释放内存 */
			free(frameInfo.m_pImageBuf);
			continue;
		}

		/* mono8格式可不做转码，直接显示，其他格式需要经过转码才能显示 */
		if (Dahua::GenICam::gvspPixelMono8 == frameInfo.m_PixelType)
		{
			/* 显示 */
			if (false == ShowImage(frameInfo.m_pImageBuf, frameInfo.m_nBufferSize, frameInfo.m_nWidth, frameInfo.m_nHeight, frameInfo.m_PixelType))
			{
				printf("_render.display failed.\n");
			}
			/* 释放内存 */
			free(frameInfo.m_pImageBuf);
		}
		else
		{
			/* 转码 */
			uint8_t *pRGBbuffer = NULL;
			int nRgbBufferSize = 0;
			nRgbBufferSize = frameInfo.m_nWidth * frameInfo.m_nHeight * 3;
			pRGBbuffer = (uint8_t *)malloc(nRgbBufferSize);
			if (pRGBbuffer == NULL)
			{
				/* 释放内存 */
				free(frameInfo.m_pImageBuf);
				printf("RGBbuffer malloc failed.\n");
				continue;
			}

			IMGCNV_SOpenParam openParam;
			openParam.width = frameInfo.m_nWidth;
			openParam.height = frameInfo.m_nHeight;
			openParam.paddingX = frameInfo.m_nPaddingX;
			openParam.paddingY = frameInfo.m_nPaddingY;
			openParam.dataSize = frameInfo.m_nBufferSize;
			openParam.pixelForamt = frameInfo.m_PixelType;

			IMGCNV_EErr status = IMGCNV_ConvertToRGB24(frameInfo.m_pImageBuf, &openParam, pRGBbuffer, &nRgbBufferSize);
			if (IMGCNV_SUCCESS != status)
			{
				/* 释放内存 */
				printf("IMGCNV_ConvertToRGB24 failed.\n");
				free(frameInfo.m_pImageBuf);
				free(pRGBbuffer);
				return;
			}

			/* 释放内存 */
			free(frameInfo.m_pImageBuf);

			/* 显示 */
			if (false == ShowImage(pRGBbuffer, nRgbBufferSize, openParam.width, openParam.height, openParam.pixelForamt))
			{
				printf("_render.display failed.");
			}
			free(pRGBbuffer);
		}
	}
}

bool CammerWidget::isTimeToDisplay(uintmax_t nCurTime)
{
	CGuard guard(m_mxTime);

	// 不显示
	if (m_nDisplayInterval <= 0)
	{
		return false;
	}

	// 第一帧必须显示
	if (m_nFirstFrameTime == 0 || m_nLastFrameTime == 0)
	{
		m_nFirstFrameTime = m_elapsedTimer.nsecsElapsed();
		m_nLastFrameTime = m_nFirstFrameTime;

		return true;
	}

	// 当前帧和上一帧的间隔如果大于显示间隔就显示
	uint64_t nCurTimeTmp = m_elapsedTimer.nsecsElapsed();
	double dAcquisitionInterval = nCurTimeTmp * 1000 / TIMESTAMPFREQUENCY;
	uint64_t nAcquisitionInterval = nCurTimeTmp - m_nLastFrameTime;
	if (nAcquisitionInterval > m_nDisplayInterval)
	{
		m_nLastFrameTime = nCurTimeTmp;
		return true;
	}


	// 当前帧相对于第一帧的时间间隔
	uint64_t nPre = (m_nLastFrameTime - m_nFirstFrameTime) % m_nDisplayInterval;
	if (nPre + nAcquisitionInterval > m_nDisplayInterval)
	{
		m_nLastFrameTime = nCurTimeTmp;
		return true;
	}

	return false;
}

void CammerWidget::setDisplayFPS(int nFPS)
{
	if (nFPS > 0)
	{
		CGuard guard(m_mxTime);

		m_nDisplayInterval = 1000 * 1000 * 1000.0 / nFPS;
	}
	else
	{
		CGuard guard(m_mxTime);
		m_nDisplayInterval = 0;
	}
}

bool CammerWidget::open(uint32_t width, uint32_t height)
{
	if (NULL != m_handler ||
		0 == width ||
		0 == height)
	{
		return false;
	}

	memset(&m_params, 0, sizeof(m_params));
	m_params.eVideoRenderMode = VR_MODE_GDI;
	m_params.hWnd = m_hWnd/*(VR_HWND)this->winId()*/;
	m_params.nWidth = width;
	m_params.nHeight = height;

	VR_ERR_E ret = VR_Open(&m_params, &m_handler);
	if (ret == VR_NOT_SUPPORT)
	{
		printf("%s cant't display YUV on this computer", __FUNCTION__);
		return false;
	}

	printf("%s open failed. error code[%d]", __FUNCTION__, ret);
	return false;
}

bool CammerWidget::close()
{
	if (m_handler != NULL)
	{
		VR_Close(m_handler);
		m_handler = NULL;
	}
	return true;
}

void CammerWidget::closeEvent(QCloseEvent * event)
{
	if (NULL != m_pStreamSource && m_pStreamSource->isGrabbing())
		m_pStreamSource->stopGrabbing();
	if (NULL != m_pCamera && m_pCamera->isConnected())
		m_pCamera->disConnect();
}