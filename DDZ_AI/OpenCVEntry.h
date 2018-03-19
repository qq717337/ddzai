#ifndef _OPENCVENTRY_H_
#define _OPENCVENTRY_H_
#include "stdafx.h"
#include<opencv2/highgui.hpp>
#include<opencv2/photo.hpp>
#include<vector>
#include<string>
#include<map>
#include"CardSet.h"
class OpenCVEntry
{
public:
	struct TextInfo {
		CvPoint pos;
		CvScalar color;
		float size;
		std::string text;
		TextInfo(CvPoint& p, CvScalar& c, float s, const std::string& t) {
			pos = p;
			color = c;
			size = s;
			text = t;
		}
	};
private:
	std::wstring m_cardPath;
	std::map<std::string, cv::Mat>image_map;
public:
    cv::Mat showCardInternal(CardVector PlayerCards0, CardVector PlayerCards1, CardVector PlayerCards2, CardVector extraCards);
    cv::Mat showPlayInternal(CardVector PlayerCards0, CardVector PlayerCards1, CardVector PlayerCards2, int lastIdentity, int playerIdentity, CardVector lastPlayCards, CardVector outPlayCards);

	OpenCVEntry(const wchar_t* cardImagePath);
	std::string GetCardName(uint8_t cardValue);
	const cv::Mat ShowCardInternal(std::map<std::string, cv::Mat>& map, uint8_t cardValue) {
		return  map[GetCardName(cardValue)];
	}
	cv::Mat ShowCardInternal(CardSet* cardSet, std::vector<TextInfo> infos = {});
	void Show(const cv::String& img1, const cv::String& img2);
	void ShowCard(CardVector  PlayerCards0, CardVector  PlayerCards1, CardVector  PlayerCards2, CardVector  extraCards, std::vector<TextInfo> infos = {});
	void ShowCard(CardSet* cardSet, std::vector<TextInfo> infos = {});
	void WriteCard(cv::String name, CardSet* cardSet, std::vector<TextInfo> infos = {});

	void ShowPlay(CardSet* cardSet, int lastIdentity, int playerIdentity, CardVector lastPlayCards, CardVector outPlayCards);
	void ShowPlay(CardVector PlayerCards0, CardVector PlayerCards1, CardVector PlayerCards2, int lastIdentity, int playerIdentity, CardVector lastPlayCards, CardVector outPlayCards);
	void WritePlay(cv::String name, CardVector PlayerCards0, CardVector PlayerCards1, CardVector PlayerCards2, int lastIdentity, int playerIdentity, CardVector lastPlayCards, CardVector outPlayCards, std::vector<TextInfo> infos);

	void Wait(int delay);
	void DestroyAllWindow();
	~OpenCVEntry();
	const cv::Mat ShowCardInternal(uint8_t cardValue, int rotate = -1);
};

#endif
