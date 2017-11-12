#include "stdafx.h"
#include "common_windows.h"
#include "OpenCVEntry.h"

OpenCVEntry::OpenCVEntry(const wchar_t* cardImagePath):m_cardPath(cardImagePath)
{
	std::map<std::string, std::string> filesName;
	Common::GetFilesWindows(m_cardPath, filesName);
	for (auto & v : filesName) {
		cv::Mat m = cv::imread(v.second);
		image_map.emplace(v.first, m);
	}
}

std::string OpenCVEntry::GetCardName(uint8_t cardValue)
{
	const std::string	m_colorCollection[] = { "", "Club", "Diamond", "Heart", "Spade" };
	const std::string m_valueCollection[] = {
		"SJoker",
		"LJoker",
		"Three",
		"Four",
		"Five",
		"Six",
		"Seven",
		"Eight",
		"Nine",
		"Ten",
		"Jack",
		"Queen",
		"King",
		"One",
		"Two"
	};
	uint8_t color = (cardValue & 0xf0) >> 4;
	uint8_t value = cardValue & 0x0f;
	return std::string(m_colorCollection[color]).append( m_valueCollection[value - 1].c_str());
}

void OpenCVEntry::Show(const cv::String& img1, const cv::String& img2)
{
	auto test = cvLoadImage(img1.c_str());
	auto mat1 = cv::imread(img1);
	auto mat2 = cv::imread(img2);
	auto mat3 = mat1(cv::Rect(10, 10, mat2.cols, mat2.rows));

	mat2.copyTo(mat3);
	CvFont font2;
	cvInitFont(&font2, CV_FONT_ITALIC, 0.5, 0.5, 0, 1);

	cvPutText(test, "hello ", CvPoint(10, 10), &font2, cvScalar(1, 0.5, 0, 0.7));
	cvShowImage("opencv_demo", test);

	imshow("fs", mat1);
	//cvShowImage("fss", &mat1);
	//cv::waitKey(5000);
	//cv::destroyAllWindows();
}
void OpenCVEntry::ShowCard(CardVector  PlayerCards0, CardVector  PlayerCards1, CardVector  PlayerCards2, CardVector  extraCards)
{
	cv::Mat mBack = image_map["deskimg"];
	int extraCardX = 1280 / 2 - 120;
	int	extraCardY = 720 / 2 - 50;
	for (auto v : extraCards) {
		cv::Mat	copyContent = GetCardImage( v);
		auto mat3 = mBack(cv::Rect(extraCardX, extraCardY, copyContent.cols, copyContent.rows));
		copyContent.copyTo(mat3);
		extraCardX += 40;
	}
	int p0X = 280;
	int p0Y = 540;
	for (auto v : PlayerCards0) {
		cv::Mat	copyContent = GetCardImage( v);
		auto mat3 = mBack(cv::Rect(p0X, p0Y, copyContent.cols, copyContent.rows));
		copyContent.copyTo(mat3);
		p0X += 40;
	}

	int p1X = 60;
	int	p1Y = 80;
	for (auto v : PlayerCards1) {
		cv::Mat	copyContent = GetCardImage( v);
		auto mat3 = mBack(cv::Rect(p1X, p1Y, copyContent.cols, copyContent.rows));
		copyContent.copyTo(mat3);
		p1Y += 30;
	}

	int p2X = 1280 - 60 * 2 - 54;
	int p2Y = 80;
	for (auto v : PlayerCards2) {
		cv::Mat	copyContent = GetCardImage(v);
		auto mat3 = mBack(cv::Rect(p2X, p2Y, copyContent.cols, copyContent.rows));
		copyContent.copyTo(mat3);
		p2Y += 30;
	}

	imshow("fs", mBack);
}
void OpenCVEntry::Wait(int delay)
{
	cvWaitKey(delay);
}

void OpenCVEntry::DestroyAllWindow()
{
	cvDestroyAllWindows();
}


OpenCVEntry::~OpenCVEntry()
{
	DestroyAllWindow();
}

const cv::Mat OpenCVEntry::GetCardImage(uint8_t cardValue, bool rotate)
{
	if (rotate) {
		cv::Mat src = image_map[GetCardName(cardValue)];

		cv::Mat srcCopy(cvSize(src.rows, src.cols), src.depth(), src.channels());
		cvTranspose(&src, &srcCopy);
		//cv::flip(&srcCopy, NULL, 1);
		return srcCopy;
	}
	else {
		return image_map[GetCardName(cardValue)];
	}
}
