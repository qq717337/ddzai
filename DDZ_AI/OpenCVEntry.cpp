#include "stdafx.h"
#ifdef _WIN32
#include "common_windows.h"
#else
#include "common.h"
#endif
#include "OpenCVEntry.h"

cv::Mat OpenCVEntry::showCardInternal(CardVector PlayerCards0, CardVector PlayerCards1, CardVector PlayerCards2, CardVector extraCards)
{
	cv::Mat mBack(image_map["deskimg"].rows, image_map["deskimg"].cols, 0);
	image_map["deskimg"].copyTo(mBack);
	int extraCardX = 1280 / 2 - 120;
	int	extraCardY = 720 / 2 - 50;
	for (auto v : extraCards) {
		cv::Mat	copyContent = GetCardImage(v);
		auto mat3 = mBack(cv::Rect(extraCardX, extraCardY, copyContent.cols, copyContent.rows));
		copyContent.copyTo(mat3);
		extraCardX += 40;
	}
	int p0X = 280;
	int p0Y = 540;
	for (auto v : PlayerCards0) {
		cv::Mat	copyContent = GetCardImage(v);
		auto mat3 = mBack(cv::Rect(p0X, p0Y, copyContent.cols, copyContent.rows));
		copyContent.copyTo(mat3);
		p0X += 40;
	}

	int p1X = 1280 - 60 * 2 - 54;
	int p1Y = 80;
	for (auto v : PlayerCards1) {
		cv::Mat	copyContent = GetCardImage(v, cv::ROTATE_90_CLOCKWISE);
		auto mat3 = mBack(cv::Rect(p1X, p1Y, copyContent.cols, copyContent.rows));
		copyContent.copyTo(mat3);
		p1Y += 30;
	}

	int p2X = 60;
	int	p2Y = 80;
	for (auto v : PlayerCards2) {
		cv::Mat	copyContent = GetCardImage(v, cv::ROTATE_90_COUNTERCLOCKWISE);
		auto mat3 = mBack(cv::Rect(p2X, p2Y, copyContent.cols, copyContent.rows));
		copyContent.copyTo(mat3);
		p2Y += 30;
	}
	return mBack;
}

cv::Mat OpenCVEntry::showPlayInternal(CardVector PlayerCards0, CardVector PlayerCards1, CardVector PlayerCards2, int lastIdentity, int playerIdentity, CardVector lastPlayCards, CardVector outPlayCards)
{
	static std::vector<std::string> IdentityString = { "lord", "Farmer 1", "Farmer 2" };
	auto mBack = showCardInternal(PlayerCards0, PlayerCards1, PlayerCards2, {});
	int p0X = 1280 / 2 - lastPlayCards.size() * 40;
	int	p0Y = 100;

	for (auto v : lastPlayCards) {
		cv::Mat	copyContent = GetCardImage(v);
		auto mat3 = mBack(cv::Rect(p0X, p0Y, copyContent.cols, copyContent.rows));
		copyContent.copyTo(mat3);
		p0X += 40;
	}
	if (outPlayCards.size() > 0) {
		p0X = 1280 / 2 - (outPlayCards.size() - 1) * 40;
		p0Y = 350;
		for (auto v : outPlayCards) {
			cv::Mat	copyContent = GetCardImage(v);
			auto mat3 = mBack(cv::Rect(p0X, p0Y, copyContent.cols, copyContent.rows));
			copyContent.copyTo(mat3);
			p0X += 40;
		}
	}
	else {
		cv::putText(mBack, "can not take", CvPoint(1280 / 2 - 100, 400.0), cv::FONT_HERSHEY_PLAIN, 2.0, cvScalar(255, 128, 0, 192));
	}

	cv::putText(mBack, "Last Player:" + IdentityString[lastIdentity], CvPoint(450, 60), cv::FONT_HERSHEY_PLAIN, 1.5, cvScalar(255, 255, 255, 255));
	auto getColor = [playerIdentity](int i) {
		return i == playerIdentity ? cvScalar(32, 255, 32, 255) : cvScalar(32, 255, 255, 255);
	};
	cv::putText(mBack, IdentityString[0], CvPoint(1280 / 2, 700), cv::FONT_HERSHEY_PLAIN, 1.5, getColor(0));
	cv::putText(mBack, IdentityString[1], CvPoint(1280 - 140, 40), cv::FONT_HERSHEY_PLAIN, 1.5, getColor(1));
	cv::putText(mBack, IdentityString[2], CvPoint(0, 40), cv::FONT_HERSHEY_PLAIN, 1.5, getColor(2));
	return mBack;
}

OpenCVEntry::OpenCVEntry(const wchar_t* cardImagePath) :m_cardPath(cardImagePath)
{
	std::map<std::string, std::string> filesName;
#ifdef _WIN32
	Common::GetFilesWindows(m_cardPath, filesName);
#else
	Common::GetFiles(m_cardPath, filesName);
#endif
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
	return std::string(m_colorCollection[color]).append(m_valueCollection[value - 1].c_str());
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

	cvPutText(test, "中文行不行 hello ", CvPoint(10, 10), &font2, cvScalar(1, 0.5, 0, 0.7));
	cvShowImage("opencv_demo", test);

	cv::imshow("fs", mat1);
	//cvShowImage("fss", &mat1);
	//cv::waitKey(5000);
	//cv::destroyAllWindows();
}
void OpenCVEntry::ShowCard(CardVector  PlayerCards0, CardVector  PlayerCards1, CardVector  PlayerCards2, CardVector  extraCards, std::vector<TextInfo> infos)
{
	auto mBack = showCardInternal(PlayerCards0, PlayerCards1, PlayerCards2, extraCards);
	for (auto& info : infos) {
		cv::putText(mBack, info.text, info.pos, 0, info.size, info.color);
	}
	cv::imshow("fs", mBack);
}

void OpenCVEntry::ShowCard(CardSet * cardSet, std::vector<TextInfo> infos)
{
	Show("C:\\Users\\liu\\Pictures\\Saved Pictures\\1.jpg", "C:\\Users\\liu\\Pictures\\Saved Pictures\\2.jpg");
	ShowCard(cardSet->PlayerCardSet[0]->ToCardValues(), cardSet->PlayerCardSet[1]->ToCardValues(), cardSet->PlayerCardSet[2]->ToCardValues(), cardSet->ExtraCard.ToCardValues(), infos);
}

void OpenCVEntry::ShowPlay(CardSet * cardSet, int lastIdentity, int playerIdentity, CardVector lastPlayCards, CardVector outPlayCards)
{
	ShowPlay(cardSet->PlayerCardSet[0]->ToCardValues(), cardSet->PlayerCardSet[1]->ToCardValues(), cardSet->PlayerCardSet[2]->ToCardValues(),
		lastIdentity, playerIdentity, lastPlayCards, outPlayCards);
}

void OpenCVEntry::ShowPlay(CardVector PlayerCards0, CardVector PlayerCards1, CardVector PlayerCards2, int lastIdentity, int playerIdentity, CardVector lastPlayCards, CardVector outPlayCards)
{
	auto mBack = showPlayInternal(PlayerCards0, PlayerCards1, PlayerCards2, lastIdentity, playerIdentity, lastPlayCards, outPlayCards);
	Show("C:\\Users\\liu\\Pictures\\Saved Pictures\\1.jpg", "C:\\Users\\liu\\Pictures\\Saved Pictures\\2.jpg");
	cv::imshow("play card", mBack);
}
void OpenCVEntry::WritePlay(cv::String name, CardVector PlayerCards0, CardVector PlayerCards1, CardVector PlayerCards2, int lastIdentity, int playerIdentity, CardVector lastPlayCards, CardVector outPlayCards, std::vector<TextInfo> infos)
{
	auto mBack = showPlayInternal(PlayerCards0, PlayerCards1, PlayerCards2, lastIdentity, playerIdentity, lastPlayCards, outPlayCards);
	for (auto& info : infos) {
		cv::putText(mBack, info.text, info.pos, 0, info.size, info.color);
	}
	cv::imwrite(name, mBack);
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
}

const cv::Mat OpenCVEntry::GetCardImage(uint8_t cardValue, int rotate)
{
	if (rotate>-1) {
		cv::Mat src = image_map[GetCardName(cardValue)];
		cv::rotate(src, src, rotate);
		return src;
	}
	else {
		return image_map[GetCardName(cardValue)];
	}
}
