#pragma once
#include "stdafx.h"
#include <opencv2\highgui.hpp>
#include<opencv2\photo\photo.hpp>
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

	cv::Mat showCardInternal(CardVector  PlayerCards0, CardVector  PlayerCards1, CardVector  PlayerCards2, CardVector  extraCards);
	cv::Mat showPlayInternal(CardVector PlayerCards0, CardVector PlayerCards1, CardVector PlayerCards2, int lastIdentity, int playerIdentity, CardVector lastPlayCards, CardVector outPlayCards);
public:
	OpenCVEntry(const wchar_t* cardImagePath);
	std::string GetCardName(uint8_t cardValue);
	const cv::Mat GetCardImage(std::map<std::string, cv::Mat>& map, uint8_t cardValue) {
		return  map[GetCardName(cardValue)];
	}
	void Show(const cv::String& img1, const cv::String& img2);
	void ShowCard(CardVector  PlayerCards0, CardVector  PlayerCards1, CardVector  PlayerCards2, CardVector  extraCards, std::vector<TextInfo> infos = {});
	void ShowCard(CardSet* cardSet, std::vector<TextInfo> infos = {});

	void ShowPlay(CardSet* cardSet, int lastIdentity, int playerIdentity, CardVector lastPlayCards, CardVector outPlayCards);
	void ShowPlay(CardVector PlayerCards0, CardVector PlayerCards1, CardVector PlayerCards2, int lastIdentity, int playerIdentity, CardVector lastPlayCards, CardVector outPlayCards);
	void WritePlay(cv::String name, CardVector PlayerCards0, CardVector PlayerCards1, CardVector PlayerCards2, int lastIdentity, int playerIdentity, CardVector lastPlayCards, CardVector outPlayCards, std::vector<TextInfo> infos);

	void Wait(int delay);
	void DestroyAllWindow();
	~OpenCVEntry();
	const cv::Mat GetCardImage(uint8_t cardValue, bool rotate = false);
};

